#pragma once

#include <stdint.h>

/**
 * @file records.h
 * @brief Persistent storage helpers for PC-side test records.
 */

/**
 * @brief One saved test record.
 */
typedef struct {
    uint32_t test_id;
    char timestamp[32]; // Human-readable timestamp when the test was sent.
    float duration_sec; // Time from send to result receipt, in seconds.
    uint8_t result;     // Result code: 1 = success, 255 = failure, 0 = network error.

} test_record_t;

/**
 * @brief Append one test record to persistent storage.
 * @param record Record to save.
 * @return 0 on success, non-zero on failure.
 */
int save_test_record(const test_record_t *record);

/**
 * @brief Print the record for one specific test ID.
 * @param test_id The test identifier to print.
 * @return 0 on success, non-zero on failure.
 */
int print_test_record(uint32_t test_id);

/**
 * @brief Print every saved test record.
 * @return 0 on success, non-zero on failure.
 */
int print_all_test_records(void);