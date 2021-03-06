
/*
 * Copyright (C) 2018 Tianyuan Yu, Zhiyi Zhang, Edward Lu
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#ifndef AES_TESTS_H
#define AES_TESTS_H

#include <stdbool.h>
#include <stdint.h>

// returns true if all tests passed, false otherwise
bool run_aes_tests(void);

typedef struct {
  char **test_names;
  uint32_t test_name_index;
  const uint8_t *iv;
  uint32_t iv_size;
  const uint8_t *key;
  uint32_t key_size;
  const uint8_t *data;
  uint32_t data_size;
  bool *passed;
} aes_test_t;


#endif // AES_TESTS_H
