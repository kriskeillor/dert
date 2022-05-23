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

void vDertSenseSoil(void *pvParameters) {
    uint8_t moisture_sns_dat_tx;
    uint8_t moisture_sns_dat_rx[2];
    int moisture_sns_dat_err;

    for ( ;; ) {
        printf("    DERT state: Sensing soil!");
        // Soil sensors DISABLED to troubleshoot light sensor
        if (false) {  
            // Sensor 1
            moisture_sns_dat_tx = 0x02;
            moisture_sns_dat_err = i2c_write_blocking(&i2c1_inst, MOISTURE_SNS_1_ADDR, &moisture_sns_dat_tx, 1, false);
            if (moisture_sns_dat_err < 0)
                printf("                Error %d writing to Moisture Sensor 1!", moisture_sns_dat_err);
            else
                printf("                Wrote %d byte to Moisture Sensor 1.", moisture_sns_dat_err);

            moisture_sns_dat_err = i2c_read_blocking(&i2c1_inst, MOISTURE_SNS_1_ADDR, &moisture_sns_dat_rx[0], 1, false);
            if (moisture_sns_dat_err < 0)
                printf("                Error %d reading from Moisture Sensor 1!", moisture_sns_dat_err);
            else {
                printf("                Wrote %d byte to Moisture Sensor 1.", moisture_sns_dat_err);
                printf("                Raw address reading: %d", moisture_sns_dat_rx);
            }

            // Sensor 2
            moisture_sns_dat_tx = 0x02;
            moisture_sns_dat_err = i2c_write_blocking(&i2c1_inst, MOISTURE_SNS_2_ADDR, &moisture_sns_dat_tx, 1, false);
            if (moisture_sns_dat_err < 0)
                printf("                Error %d writing to Moisture Sensor 2!", moisture_sns_dat_err);
            else
                printf("                Wrote %d byte to Moisture Sensor 2.", moisture_sns_dat_err);

            moisture_sns_dat_err = i2c_read_blocking(&i2c1_inst, MOISTURE_SNS_2_ADDR, &moisture_sns_dat_rx[0], 1, false);
            if (moisture_sns_dat_err < 0)
                printf("                Error %d reading from Moisture Sensor 2!", moisture_sns_dat_err);
            else {
                printf("                Wrote %d byte to Moisture Sensor 2.", moisture_sns_dat_err);
                printf("                Raw address reading: %d", moisture_sns_dat_rx);
            }
        }

        // (Placeholder) Enable Low-Voltage Relays
        printf("                Controlling pumps!");
        gpio_put(GPIO_LVR1, 0);
        gpio_put(GPIO_LVR2, 0);

        vTaskDelay( dertSENSE_SOIL_TASK_PERIOD );
    }
}