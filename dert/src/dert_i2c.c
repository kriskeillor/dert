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

// Currently only supports 1B opecodes with stops
#define DERT_I2C_CMD_LEN 1

// Supports up to 2B return data
int dert_i2c_sns_wr(uint8_t addr, 
        const uint8_t *sns_dat_tx,
        uint8_t *sns_dat_rx,
        size_t rx_len,
        uint timeout_us,
        const char *sns_name) {
    // Field for error code or num/bytes written
    int sns_dat_err;

    // Write command to I2C Bus
    sns_dat_err = i2c_write_timeout_us(&i2c1_inst, addr, sns_dat_tx, DERT_I2C_CMD_LEN , false, timeout_us);
    if (dertVERBOSE_LOGS) {
        if (sns_dat_err < 0)
            printf("Error %d writing to %c!\n", sns_dat_err, sns_name);
        else
            printf("Wrote %dB to %s.\n", sns_dat_err, sns_name);
    }

    // If no error, read result from I2C Bus
    if (sns_dat_err >= PICO_ERROR_NONE) {
        sns_dat_err = i2c_read_timeout_us(&i2c1_inst, addr, sns_dat_rx, rx_len, false, timeout_us);
        if (dertVERBOSE_LOGS) {
            if (sns_dat_err < 0)
                printf("Error %d reading from %c!\n", sns_dat_err, sns_name);
            else
                printf("Read %dB from %s.\n", sns_dat_err, sns_name);
        }
    } else 
        printf("Due to write to %s error, read not attempted.\n", sns_name);

    return sns_dat_err;
}