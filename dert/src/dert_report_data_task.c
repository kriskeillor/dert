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

void vDertReportData(void *pvParameters) {
    for ( ;; ) {
        printf("    DERT state: Reporting data!");
        printf("                Reporting errors!");

        vTaskDelay( dertREPORT_DATA_TASK_PERIOD );
    }
}