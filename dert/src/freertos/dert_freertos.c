/**
 * Copyright (c) 2022 Kris Keillor
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>

#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"

#include "dert_2040.h"

// Exclude reserved addresses, of the form 000 0xxx or 
// 111 1xxx, from the search.
bool i2c_res_addr(uint8_t addr) {
    return (addr & 0x78) == 0 || (addr & 0x78) == 0x78;
}

void i2c_scan() {
    printf("    I2C Bus Scan\n");
    printf("    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n");
    for (int addr = 0; addr < (1 << 7); ++addr) {
        if (addr % 16 == 0) {
            printf("%02x  ", addr);
        }

        // Attempt read from I2C address. If acknowledged,
        // the function returns the #/bytes transferred. If ignored,
        // the function returns -2.
        int ret;
        uint8_t rxdata;
        if (i2c_res_addr(addr))
            ret = -1;//PICO_ERROR_GENERIC;
        else
            // TODO: Configure dert board file with dert_default_i2c define
            //       Currently bad practice
            ret = i2c_read_blocking(&i2c1_inst, addr, &rxdata, 1, false);

        printf(ret < 0 ? "." : "@");
        printf(addr % 16 == 15 ? "\n" : "  ");
    }
    printf("I2C scan completed.\n");
    return;
}
