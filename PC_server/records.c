#include "records.h"

#include <inttypes.h>
#include <sqlite3.h>
#include <stdio.h>
#include <time.h>

#define DB_FILE "test_records"

static sqlite3 *global_db = NULL;

static const char *result_to_text(uint8_t result)
{
    if (result == 1) {
        return "Success";
    }
    if (result == 255) {
        return "Failure";
    }
    return "NetworkError";
}

int records_db_init(void)
{
    const char *create_sql =
        "CREATE TABLE IF NOT EXISTS records ("
        "  test_id INTEGER PRIMARY KEY,"
        "  sent_at INTEGER NOT NULL,"
        "  duration_sec REAL NOT NULL,"
        "  result TEXT NOT NULL CHECK(result IN ('Success','Failure','NetworkError'))"
        ");";

    if (sqlite3_open(DB_FILE, &global_db) != SQLITE_OK) {
        fprintf(stderr, "%s:%d: sqlite3_open failed: %s\n", __FILE__, __LINE__, sqlite3_errmsg(global_db));
        records_db_close();
        return 1;
    }

    if (sqlite3_exec(global_db, create_sql, NULL, NULL, NULL) != SQLITE_OK) {
        fprintf(stderr, "%s:%d: CREATE TABLE failed: %s\n", __FILE__, __LINE__, sqlite3_errmsg(global_db));
        records_db_close();
        return 1;
    }

    return 0;
}

void records_db_close(void)
{
    if (global_db != NULL) {
        sqlite3_close(global_db);
        global_db = NULL;
    }
}

int save_test_record(uint32_t test_id,
                     time_t sent_at,
                     double duration_sec,
                     uint8_t raw_result)
{
    sqlite3_stmt *stmt = NULL;
    const char *insert_sql =
        "INSERT INTO records(test_id, sent_at, duration_sec, result) "
        "VALUES(?, ?, ?, ?);";

    if (global_db == NULL) {
        fprintf(stderr, "%s:%d: global_db == NULL\n", __FILE__, __LINE__);
        return 1;
    }
    

    if (sqlite3_prepare_v2(global_db, insert_sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "%s:%d: sqlite3_prepare_v2 failed: %s\n", __FILE__, __LINE__, sqlite3_errmsg(global_db));
        return 1;
    }

    sqlite3_bind_int64(stmt, 1, (sqlite3_int64)test_id);
    sqlite3_bind_int64(stmt, 2, (int64_t)sent_at);
    sqlite3_bind_double(stmt, 3, duration_sec);
    sqlite3_bind_text(stmt, 4, result_to_text(raw_result), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        int ext = sqlite3_extended_errcode(global_db);
        if (ext == SQLITE_CONSTRAINT_PRIMARYKEY || ext == SQLITE_CONSTRAINT_UNIQUE) {
            fprintf(stderr, "%s:%d: Duplicate TEST-ID=%" PRIu32 " (must be unique)\n", __FILE__, __LINE__, test_id);
        } else {
            fprintf(stderr, "%s:%d: INSERT failed: %s\n", __FILE__, __LINE__, sqlite3_errmsg(global_db));
        }
        sqlite3_finalize(stmt);
        return 1;
    }

    sqlite3_finalize(stmt);
    return 0;
}

int print_test_record(uint32_t test_id)
{
    char human_readable_time[32];
    struct tm *temp_time;
    sqlite3_stmt *stmt = NULL;
    const char *select_sql =
        "SELECT test_id, sent_at, duration_sec, result "
        "FROM records WHERE test_id = ?;";

    if (global_db == NULL) {
        fprintf(stderr, "%s:%d: global_db == NULL\n", __FILE__, __LINE__);
        return 1;
    }

    if (sqlite3_prepare_v2(global_db, select_sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "%s:%d: sqlite3_prepare_v2 failed: %s\n", __FILE__, __LINE__, sqlite3_errmsg(global_db));
        return 1;
    }

    sqlite3_bind_int64(stmt, 1, (sqlite3_int64)test_id);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        uint32_t test_id = (uint32_t)sqlite3_column_int64(stmt, 0);
        const time_t time_sent = sqlite3_column_int64(stmt, 1);
        double duration_sec = sqlite3_column_double(stmt, 2);
        const unsigned char *result = sqlite3_column_text(stmt, 3);

        temp_time = localtime(&time_sent);
        if(temp_time == NULL){
            perror("localtime");
            return 1;
        }
        
        if(strftime(human_readable_time, sizeof(human_readable_time),
                               "%Y/%m/%d %H:%M:%S", temp_time) == 0){
            fprintf(stderr, "%s:%d: strftime returned 0", __FILE__, __LINE__);
            return 1;
        }

        printf("TEST-ID=%" PRIu32 " DateTime=%s Duration=%.6f sec Result=%s\n",
               test_id, human_readable_time, duration_sec, result);
    } else {
        fprintf(stderr, "No record found for TEST-ID=%" PRIu32 "\n",test_id);
    }

    sqlite3_finalize(stmt);
    return 0;
}

int print_all_test_records(void)
{
    char human_readable_time[32];
    struct tm *temp_time;
    sqlite3_stmt *stmt = NULL;
    int step_rc;
    int found = 0;
    const char *select_sql =
        "SELECT test_id, sent_at, duration_sec, result "
        "FROM records ORDER BY test_id;";

    if (global_db == NULL) {
        return 1;
    }

    if (sqlite3_prepare_v2(global_db, select_sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "%s:%d: sqlite3_prepare_v2 failed: %s\n", __FILE__, __LINE__, sqlite3_errmsg(global_db));
        return 1;
    }

    while ((step_rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        found = 1;
        uint32_t test_id = (uint32_t)sqlite3_column_int64(stmt, 0);
        const time_t time_sent = sqlite3_column_int64(stmt, 1);
        double duration_sec = sqlite3_column_double(stmt, 2);
        const unsigned char *result = sqlite3_column_text(stmt, 3);

        temp_time = localtime(&time_sent);
        if(temp_time == NULL){
            perror("localtime");
            return 1;
        }
        
        if(strftime(human_readable_time, sizeof(human_readable_time),
                               "%Y/%m/%d %H:%M:%S", temp_time) == 0){
            fprintf(stderr, "%s:%d: strftime returned 0", __FILE__, __LINE__);
            return 1;
        }

        printf("TEST-ID=%" PRIu32 " DateTime=%s Duration=%.10f sec Result=%s\n",
               test_id, human_readable_time, duration_sec, result);
    }

    if (step_rc != SQLITE_DONE) {
        fprintf(stderr, "%s:%d: sqlite3_step failed: %s\n", __FILE__, __LINE__, sqlite3_errmsg(global_db));
        sqlite3_finalize(stmt);
        return 1;
    }

    if (!found) {
        fprintf(stderr, "%s:%d: No tests saved\n", __FILE__, __LINE__);
    }

    sqlite3_finalize(stmt);
    return 0;
}