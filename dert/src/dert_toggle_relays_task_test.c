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
#include "dert_toggle_relays_task_test.h"

void vDertToggleRelays(void *pvParameters) {
    bool hvr_en = false;

    for ( ;; ) {
        printf("\nDert task: Toggling relays (test).\n");

        hvr_en = !hvr_en;
        gpio_put(GPIO_HVR, hvr_en);

        printf("HVR state: %d\n", gpio_get(GPIO_HVR));

        vTaskDelay( dertTOGGLE_RELAYS_TASK_PERIOD );
    }
}