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

void vDertReportData(void *pvParameters) {
    for ( ;; ) {
        printf("    DERT state: Reporting data!\n");
        printf("                Reporting errors!\n");
        gpio_put(GPIO_HVR, 0); // Disable light relay
    }
}