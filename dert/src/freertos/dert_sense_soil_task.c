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
#include "dert_i2c.h"
#include "dert.h"
#include "dert_sense_soil_task.h"

void vDertSenseSoil(void *pvParameters) {
    uint8_t moisture_sns_dat_tx;
    uint8_t moisture_sns_dat_rx[2];
    int moisture_sns_dat_err;

    for ( ;; ) {
        printf("TASK:    Sense Soil!\n");

        // Sensor 2
        // Soil Capacitance Reading
        moisture_sns_dat_tx = CHIRP_GET_CAP;
        moisture_sns_dat_err = dert_i2c_sns_wr(MOISTURE_SNS_2_ADDR,
            &moisture_sns_dat_tx,
            &moisture_sns_dat_rx[0],
            CHIRP_GET_CAP_LEN,
            dertSNS_TIMEOUT_MS,
            (char *)CHIRP_NAME);

        if (moisture_sns_dat_err > 0)
            printf("Read 0x%x%x capacitance from %s.\n", moisture_sns_dat_rx[0], moisture_sns_dat_rx[1], CHIRP_NAME);
        else
            printf("Error: Read 0 bytes from %s.\n", CHIRP_NAME); 

        // Soil Temperature Reading
        moisture_sns_dat_tx = CHIRP_GET_TEMP;
        moisture_sns_dat_err = dert_i2c_sns_wr(MOISTURE_SNS_2_ADDR,
            &moisture_sns_dat_tx,
            &moisture_sns_dat_rx[0],
            CHIRP_GET_TEMP_LEN,
            dertSNS_TIMEOUT_MS,
            (char *)CHIRP_NAME);

        if (moisture_sns_dat_err > 0)
            printf("Read 0x%x%x temperature from %s.\n", moisture_sns_dat_rx[0], moisture_sns_dat_rx[1], CHIRP_NAME);
        else
            printf("Error: Read 0 bytes from %s.\n", CHIRP_NAME); 

        // (Placeholder) Enable Low-Voltage Relays
        printf("Controlling pumps!\n");
        gpio_put(GPIO_LVR1, 0);
        gpio_put(GPIO_LVR2, 0);

        vTaskDelay( dertSENSE_SOIL_TASK_PERIOD );
    }
}