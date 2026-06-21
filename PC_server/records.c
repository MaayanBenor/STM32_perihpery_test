#include "records.h"

#include <inttypes.h>
#include <stdio.h>

#define RECORDS_FILE "test_records.log"
#define RECORD_FIELDS_COUNT 4

/**
 * @brief Convert an internal result code into text for storage and printing.
 * @param result Result code from the eth_protocol_result_t.
 * @return A human-readable result string.
 */
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

/**
 * @brief Save one test record as a single line in the records file.
 * @param record Record to append.
 * @return 0 on success, non-zero on failure.
 */
int save_test_record(const test_record_t *record)
{
    if (record == NULL) {
        return 1;
    }

    FILE *file;

    file = fopen(RECORDS_FILE, "a");
    if (file == NULL) {
        perror("fopen failed");
        return 1;
    }

    //TODO: test that it looks good in file (not missing /n)
    /* One line per record so the file can be scanned back later. */
    if (fprintf(file, "%" PRIu32 "|%s|%.3f|%s\n",
                record->test_id,
                record->timestamp,
                record->duration_sec,
                result_to_text(record->result)) < 0) {
        perror("fprintf failed");
        fclose(file);
        return 1;
    }

    if (fclose(file) != 0) {
        perror("fclose failed");
        return 1;
    }

    return 0;
}

/**
 * @brief Read the records file and print the first record matching one test ID.
 * @param test_id The test identifier to search for.
 * @return 0 on success, non-zero on failure.
 */
int print_test_record(uint32_t test_id)
{
    FILE *file;
    char line[256];
    uint32_t current_test_id;
    char timestamp[32];
    float duration_sec;
    char result_text[16];

    file = fopen(RECORDS_FILE, "r");
    if (file == NULL) {
        perror("fopen failed");
        return 1;
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        if (sscanf(line, "%" SCNu32 "|%31[^|]|%f|%15[^\n]",
                   &current_test_id,
                   timestamp,
                   &duration_sec,
                   result_text) == RECORD_FIELDS_COUNT &&
                   current_test_id == test_id) {
            printf("TEST-ID=%" PRIu32 " DateTime=%s Duration=%.3f sec Result=%s\n",
                   current_test_id,
                   timestamp,
                   duration_sec,
                   result_text);
            fclose(file);
            return 0;
        }
    }

    fclose(file);
    fprintf(stderr, "No record found for TEST-ID=%" PRIu32 "\n", test_id);
    return 0;
}

/**
 * @brief Read the records file and print every saved record.
 * @return 0 on success, non-zero on failure.
 */
int print_all_test_records(void)
{
    FILE *file;
    char line[256];
    uint32_t current_test_id;
    char timestamp[32];
    float duration_sec;
    char result_text[16];

    file = fopen(RECORDS_FILE, "r");
    if (file == NULL) {
        perror("fopen failed");
        return 1;
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        if (sscanf(line, "%" SCNu32 "|%31[^|]|%f|%15[^\n]",
                &current_test_id,
                timestamp,
                &duration_sec,
                result_text) == RECORD_FIELDS_COUNT) {
            printf("TEST-ID=%" PRIu32 " DateTime=%s Duration=%.3f sec Result=%s\n",
                   current_test_id,
                   timestamp,
                   duration_sec,
                   result_text);
        }
    }

    fclose(file);
    return 0;
}