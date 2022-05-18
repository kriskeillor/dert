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

int main() {
    stdio_init_all();
    int lifespan = 0;
    int state = Startup;

    // Light sensor fields
    uint8_t bh1750_dat_tx;
    uint8_t bh1750_dat_rx[2];
    int bh1750_dat_err;

    // Air sensor fields
    uint8_t sht30_dat_tx;
    uint8_t sht30_dat_rx[2];
    int8_t sht30_dat;

    // Soil sensor fields
    uint8_t moisture_sns_dat_tx;
    uint8_t moisture_sns_dat_rx[2];
    int moisture_sns_dat_err;

    // Wait for PuTTY to connect (USB-only)
    sleep_ms(5000);

    printf("    DERT state: Starting up!\n");
    gpio_init(GPIO_I2C_SDA);
    gpio_init(GPIO_I2C_SCL);
    i2c_init(i2c_default, 100*1000);
    gpio_set_function(GPIO_I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(GPIO_I2C_SCL, GPIO_FUNC_I2C);
    bi_decl(bi_2pins_with_func(GPIO_I2C_SDA, GPIO_I2C_SCL, GPIO_FUNC_I2C));

    gpio_init(GPIO_LED0);
    gpio_init(GPIO_LED1);

    gpio_init(GPIO_HVR);
    gpio_init(GPIO_LVR1);
    gpio_init(GPIO_LVR2);
    gpio_set_dir(GPIO_HVR, GPIO_OUT);
    gpio_set_dir(GPIO_LVR1, GPIO_OUT);
    gpio_set_dir(GPIO_LVR2, GPIO_OUT);

    gpio_init(GPIO_DB1);
    gpio_init(GPIO_DB2);
    gpio_init(GPIO_DB3);

    // Initialize FreeRTOS application
    vTaskStartScheduler();
}

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
