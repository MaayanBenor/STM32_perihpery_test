#pragma once

#include <stdint.h>

/**
 * @file [records.h](http://_vscodecontentref_/2)
 * @brief SQLite-backed persistent storage for PC-side test records.
 */

/**
 * @brief Open/create DB file and ensure schema exists.
 * @brief If fails. It closes the record it opened if necessary.
 * @return 0 on success, non-zero on failure.
 */
int records_db_init(void);

/**
 * @brief Close DB if open.
 */
void records_db_close(void);

/**
 * @brief Save one test record.
 * @param test_id Unique TEST-ID (uint32_t domain).
 * @param sent_at_utc Timestamp string like "YYYY-MM-DD HH:MM:SS".
 * @param duration_sec Duration in seconds from send to receive.
 * @param result Protocol result as char*: Success, Failure, NetworkError.
 * @return 0 on success, non-zero on failure.
 */
int save_test_record(uint32_t test_id,
                     const char *sent_at,
                     double duration_sec,
                     uint8_t raw_result);

/**
 * @brief Print one record by TEST-ID.
 * @return 0 on success, non-zero on failure.
 */
int print_test_record(uint32_t test_id);

/**
 * @brief Print all records.
 * @return 0 on success, non-zero on failure.
 */
int print_all_test_records(void);