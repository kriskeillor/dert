/**
 * Copyright (c) 2022 Kris Keillor
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"

#include "dert_2040.h"
#include "dert.h"

void vDertSenseLight(void *pvParameters) {
    uint8_t bh1750_dat_tx;
    uint8_t bh1750_dat_rx[2];
    int bh1750_dat_err;

    for ( ;; ) {
        // Turn on debug bit 1 for task
        gpio_put(GPIO_DB1, 1);

        printf("    DERT state: Sensing light!");
        bh1750_dat_tx = 0b00010000; // Continuous H-Res Mode instruction
        bh1750_dat_err = i2c_write_blocking(&i2c1_inst, BH1750_ADDR, &bh1750_dat_tx, 1, false);
        if (bh1750_dat_err == -1 || bh1750_dat_err == -2 || bh1750_dat_err == -3)
            printf("                Error %d writing to BH1750!", bh1750_dat_err);
        else
            printf("                Wrote %d byte to BH1750.", bh1750_dat_err);

        // Turn on debug bit 3 to indicate waiting
        gpio_put(GPIO_DB3, 1);
        sleep_ms(185);
        gpio_put(GPIO_DB3, 0);

        bh1750_dat_err = i2c_read_blocking(&i2c1_inst, BH1750_ADDR, bh1750_dat_rx, 2, false);
        if (bh1750_dat_err == -1 || bh1750_dat_err == -2 || bh1750_dat_err == -3)
            printf("                Error %d reading from BH1750!", bh1750_dat_err);
        else {
            printf("                Read %d bytes from BH1750.", bh1750_dat_err);
            printf("                Raw lux measurement: %d", bh1750_dat_rx);
        }

        // (Placeholder) Enable Low-Voltage Relays
        printf("                Controlling lights!");
        gpio_put(GPIO_HVR, 0);
        gpio_put(GPIO_DB2, 1);

        // Turn off debug bit 1
        gpio_put(GPIO_DB1, 0);
        vTaskDelay( dertSENSE_LIGHT_TASK_PERIOD );
    }
}