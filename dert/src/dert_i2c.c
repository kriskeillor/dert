/**
 * Copyright (c) 2022 Kris Keillor
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <string.h>

#include "hardware/i2c.h"
#include "pico/types.h"

#include "dert.h"
#include "dert_i2c.h"

// Supports up to 2B return data
int dert_i2c_sns_wr(uint8_t addr, 
        const uint8_t *sns_dat_tx,
        size_t tx_len,
        uint8_t pause_ms,
        uint8_t *sns_dat_rx,
        size_t rx_len,
        uint timeout_us,
        const char *sns_name) {
    // Field for error code or num/bytes written
    int sns_dat_err;

    // Write command to I2C Bus
    sns_dat_err = i2c_write_timeout_us(&i2c1_inst, addr, sns_dat_tx, tx_len, false, timeout_us);
    if (sns_dat_err < 0)
        printf("! Error %d writing to %s.\n", sns_dat_err, sns_name);
    else if (dertVERBOSE_LOGS)
        printf("Wrote %dB to %s.\n", sns_dat_err, sns_name);
    else { }

    // Give device time to respond
    if (pause_ms > 0)
        sleep_ms(pause_ms);
    else { }

    // If no error, read result from I2C Bus
    if (sns_dat_err >= PICO_ERROR_NONE) {
        sns_dat_err = i2c_read_timeout_us(&i2c1_inst, addr, sns_dat_rx, rx_len, false, timeout_us);
        if (sns_dat_err < 0)
            printf("! Error %d reading from %s.\n", sns_dat_err, sns_name);
        else if (dertVERBOSE_LOGS)
            printf("Read %dB from %s.\n", sns_dat_err, sns_name);
        else { }
    } else
        printf("! Due to write error, read not attempted.\n");

    return sns_dat_err;
}