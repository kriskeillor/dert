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
    bool lvr1_en = false;
    bool lvr2_en = false;

    for ( ;; ) {
        if (dertVERBOSE_LOGS) {
            printf("\nDert task: Toggling relays (test).\n");
        } else { }

        hvr_en = !hvr_en;
        gpio_put(GPIO_HVR, hvr_en);

        lvr1_en = !lvr1_en;
        lvr2_en = !lvr2_en;
        gpio_put(GPIO_LVR1, lvr1_en);
        gpio_put(GPIO_LVR2, lvr2_en);

        // Debug output
        if (dertVERBOSE_LOGS) {
            printf("HVR GPIO output: %d, state: %d\n", hvr_en, gpio_get(GPIO_HVR));
            printf("LVR1 GPIO output: %d, state: %d\n", lvr1_en, gpio_get(GPIO_LVR1));
            printf("LVR2 GPIO output: %d, state: %d\n", lvr2_en, gpio_get(GPIO_LVR2));
        }

        vTaskDelay( dertTOGGLE_RELAYS_TASK_PERIOD );
    }
}