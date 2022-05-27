/**
 * Copyright (c) 2022 Kris Keillor
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>

#include "pico/types.h"

int dert_i2c_sns_wr(uint8_t addr, 
    const uint8_t *sns_dat_tx, 
    size_t tx_len,
    uint8_t pause_ms,
    uint8_t *sns_dat_rx,
    size_t rx_len, 
    uint timeout_us, 
    const char *sns_name);