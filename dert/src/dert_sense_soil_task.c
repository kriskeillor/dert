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
#include "pico/float.h"
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
    uint16_t moisture_sns_moisture;
    float_t moisture_sns_moisture_f;
    float_t moisture_sns_temp_f;

    for ( ;; ) {
        if (dertVERBOSE_LOGS) {
            printf("\nDert task: Sensing Soil.\n");
        }

        // Soil Capacitance Reading
        moisture_sns_dat_tx = CHIRP_GET_CAP;
        moisture_sns_dat_err = dert_i2c_sns_wr(MOISTURE_SNS_2_ADDR,
            &moisture_sns_dat_tx,
            CHIRP_CMD_LEN,
            CHIRP_MEAS_TIME,
            &moisture_sns_dat_rx[0],
            CHIRP_GET_CAP_LEN,
            dertSNS_TIMEOUT_MS,
            (char *)CHIRP_NAME);
        // Print Capacitive Water Content if we got the expected #/bytes returned
        if (moisture_sns_dat_err == CHIRP_GET_CAP_LEN) {
            moisture_sns_moisture = ((moisture_sns_dat_rx[0] << 8) & 0xFF00) | moisture_sns_dat_rx[1];
            // Subtract minimum moisture from reading (without negative overflow)
            if (moisture_sns_moisture < CHIRP_MIN_MOIST) {
                moisture_sns_moisture = 0;
            } else {
                moisture_sns_moisture = moisture_sns_moisture - CHIRP_MIN_MOIST;
            }
            // Calculate moisture as a percentage
            moisture_sns_moisture_f = (moisture_sns_moisture * 100.0f) / (CHIRP_MAX_MOIST - CHIRP_MIN_MOIST);
            printf("+ Soil CWC: %f\n", moisture_sns_moisture_f);

            // Detailed logging (raw readings)
            if (dertVERBOSE_LOGS) {
                printf("Read 0x%x%x capacitance from %s.\n", moisture_sns_dat_rx[0], moisture_sns_dat_rx[1], CHIRP_NAME);
            } else { }
        }
        else {
            printf("Error: Read 0 bytes from %s.\n", CHIRP_NAME);
        }

        // Soil Temperature Reading
        moisture_sns_dat_tx = CHIRP_GET_TEMP;
        moisture_sns_dat_err = dert_i2c_sns_wr(MOISTURE_SNS_2_ADDR,
            &moisture_sns_dat_tx,
            CHIRP_CMD_LEN,
            CHIRP_MEAS_TIME,
            &moisture_sns_dat_rx[0],
            CHIRP_GET_TEMP_LEN,
            dertSNS_TIMEOUT_MS,
            (char *)CHIRP_NAME);
        // Print Temperature if we got the expected #/bytes returned
        if (moisture_sns_dat_err == CHIRP_GET_TEMP_LEN) {
            moisture_sns_temp_f = (((moisture_sns_dat_rx[0] << 8) & 0xFF00) | moisture_sns_dat_rx[1]) / 10.0f;
            printf("+ Soil Temp: %f\n", moisture_sns_temp_f);

            if (dertVERBOSE_LOGS) {
                printf("Read 0x%x%x temperature from %s.\n", moisture_sns_dat_rx[0], moisture_sns_dat_rx[1], CHIRP_NAME);
            } else { }
        }
        else {
            printf("Error: Read 0 bytes from %s.\n", CHIRP_NAME); 
        }

        // Delay task
        vTaskDelay( dertSENSE_SOIL_TASK_PERIOD );
    }
}