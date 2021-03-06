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
#include "dert_sense_air_task.h"

void vDertSenseAir(void *pvParameters) {
    uint8_t air_sns_dat_tx[SHT30_GET_DATA_PAIR_CMD_LEN];
    uint8_t air_sns_dat_rx[BH1750_DAT_RX_SZ];
    int8_t air_sns_dat_err;
    uint32_t air_sns_humidity;
    float_t air_sns_humidity_f;
    int32_t air_sns_temp;
    float_t air_sns_temp_f;

    // Set opecode to request one time, medium repeatability, clock-stretching measurement 
    air_sns_dat_tx[0] = SHT30_GET_DATA_CLK_STR;
    air_sns_dat_tx[1] = SHT30_GET_DATA_MED_RPT_CLK_STR;

    for ( ;; ) {
        if (dertVERBOSE_LOGS) {
            printf("\nDERT task: Sensing air.\n");
        } else { }

        air_sns_dat_err = dert_i2c_sns_wr(SHT30_ADDR,
            &air_sns_dat_tx[0],
            SHT30_GET_DATA_PAIR_CMD_LEN,
            SHT30_MEAS_TIME,
            &air_sns_dat_rx[0],
            SHT30_GET_DATA_PAIR_LEN,
            dertSNS_TIMEOUT_MS * 10,
            (char *)SHT30_NAME);

        if (air_sns_dat_err == BH1750_DAT_RX_SZ) {
            // Calculate humidity and print to serial output
            // First, combine raw data into one variable
            air_sns_humidity = ((air_sns_dat_rx[0] << 8) & 0xFF00) | air_sns_dat_rx[1];
            // Then, scale and shift
            air_sns_humidity = ((BH1750_RH_SCALE * air_sns_humidity) >> BH1750_RH_SHIFT);
            // Finally, convert to decimal
            air_sns_humidity_f = (float_t)air_sns_humidity / 100.0f;
            // Print
            printf("+ARH %f\n", air_sns_humidity_f);
            // Flash blue LED to indicate UART TX
            gpio_put(GPIO_LED1, 1);
            sleep_ms(150);
            gpio_put(GPIO_LED1, 0);
            sleep_ms(50);

            // Calculate temperature (in Farenheit) and print to serial output
            // First, combine data
            air_sns_temp = ((air_sns_dat_rx[3] << 8) & 0xFF00) | air_sns_dat_rx[4];
            // Then scale, shift, and subtract
            air_sns_temp = ((BH1750_T_SCALE * air_sns_temp) >> BH1750_T_SHIFT) - BH1750_T_OFFSET;
            // Convert to decimal
            air_sns_temp_f = (float_t) air_sns_temp / 100.0f;
            // Print
            printf("+ATF %f\n", air_sns_temp_f);
            // Flash blue LED to indicate UART TX
            gpio_put(GPIO_LED1, 1);
            sleep_ms(150);
            gpio_put(GPIO_LED1, 0);
            sleep_ms(50);

            // Detailed logging (raw readings)
            if (dertVERBOSE_LOGS) {
                printf("Read 0x%x%x Temperature from %s.\n", air_sns_dat_rx[0], air_sns_dat_rx[1], SHT30_NAME);
                printf("Read 0x%x%x Relative Humidity from %s.\n", air_sns_dat_rx[3], air_sns_dat_rx[4], SHT30_NAME);
            } else { }
        }
        else {
            printf("! Error %d from %s.\n", air_sns_dat_err, SHT30_NAME);
            gpio_put(GPIO_LED0, 1);
            sleep_ms(150);
            gpio_put(GPIO_LED0, 0);
            sleep_ms(50);
        }

        vTaskDelay( dertSENSE_AIR_TASK_PERIOD );
    }
}