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
    for ( ;; ) {
        if (dertVERBOSE_LOGS) {
            printf("\nDert task: Toggling relays (demo).\n");
        } else { }

        // Disable HV relay (active low) if Debug Bit 1 is being driven
        if (gpio_get(GPIO_DB1)) {
            gpio_put(GPIO_HVR, 1);
        } else {
            gpio_put(GPIO_HVR, 0);
        }

        // Enable LV relay 1 if Debug Bit 2 is being driven
        if (gpio_get(GPIO_DB2)) {
            gpio_put(GPIO_LVR1, 1);
        } else {
            gpio_put(GPIO_LVR1, 0);
        }

        // Enable LV relay 1 if Debug Bit 2 is being driven
        if (gpio_get(GPIO_DB3)) {
            gpio_put(GPIO_LVR2, 1);
        } else {
            gpio_put(GPIO_LVR2, 0);
        }

        // Debug output
        if (dertVERBOSE_LOGS) {
            printf("HVR GPIO output: %d\n", gpio_get(GPIO_HVR));
            printf("LVR1 GPIO output: %d\n", gpio_get(GPIO_LVR1));
            printf("LVR2 GPIO output: %d\n", gpio_get(GPIO_LVR2));
        }

        vTaskDelay( dertTOGGLE_RELAYS_TASK_PERIOD );
    }
}
