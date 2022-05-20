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

void vDertSenseAir(void *pvParameters) {
    uint8_t sht30_dat_tx;
    uint8_t sht30_dat_rx[2];
    int8_t sht30_dat;

    for ( ;; ) { 
        printf("    DERT state: Sensing air!");
        // (Placeholder) Disable Loww-Voltage Relays
        gpio_put(GPIO_LVR1, 0); // Disable pump 1 relay
        gpio_put(GPIO_LVR2, 0); // Disable pump 2 relay

        vTaskDelay( dertSENSE_AIR_TASK_PERIOD );
    }
}