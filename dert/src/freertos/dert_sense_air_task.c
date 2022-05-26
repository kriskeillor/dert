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
#include "dert_sense_air_task.h"

void vDertSenseAir(void *pvParameters) {
    uint8_t sht30_dat_tx;
    uint8_t sht30_dat_rx[2];
    int8_t sht30_dat;

    for ( ;; ) { 
        printf("    DERT state: Sensing air!");
        // (Placeholder) Disable All Relays
        printf("                Disabling pumps and lights!");
        gpio_put(GPIO_LVR1, 1);
        gpio_put(GPIO_LVR2, 1);
        gpio_put(GPIO_HVR, 1);

        vTaskDelay( dertSENSE_AIR_TASK_PERIOD );
    }
}