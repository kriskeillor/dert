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
#include "dert_sense_light_task.h"

void vDertSenseLight(void *pvParameters) {
    uint8_t light_sns_dat_tx;
    uint8_t light_sns_dat_rx[BH1750_DAT_SZ];
    int8_t light_sns_dat_err;
    uint16_t light_sns_lux;

    for ( ;; ) {
        if (dertVERBOSE_LOGS) {
             printf("\nDERT task: Sensing light.\n");
        }

        // Lux reading
        light_sns_dat_tx = BH1750_MEAS_INST_1LX;
        light_sns_dat_err = dert_i2c_sns_wr(BH1750_ADDR,
			&light_sns_dat_tx,
            BH1750_CMD_LEN,
            BH1750_MEAS_TIME,
			&light_sns_dat_rx[0],
			BH1750_MEAS_LUX_LEN,
			dertSNS_TIMEOUT_MS,
			(char *)BH1750_NAME);

        // Data/Error Logging
		if (light_sns_dat_err > 0) {
            // Calculate lux and print to serial output
            light_sns_lux = (((light_sns_dat_rx[0] << BH1750_SHIFT) & 0xFF00) | light_sns_dat_rx[1]) / BH1750_DIV_FACTOR;
            printf("+LUX %d\n", light_sns_lux);
            // Flash blue LED to indicate UART TX
            gpio_put(GPIO_LED1, 1);
            sleep_ms(150);
            gpio_put(GPIO_LED1, 0);
            sleep_ms(50);

            // Detailed logging (raw readings)
            if (dertVERBOSE_LOGS) {
			    printf("Read 0x%x%x lux from %s.\n", light_sns_dat_rx[0], light_sns_dat_rx[1], BH1750_NAME);
            } else { }
        }
		else {
			printf("! Error: Read 0 bytes from %s.\n", BH1750_NAME);
            gpio_put(GPIO_LED0, 1);
            sleep_ms(150);
            gpio_put(GPIO_LED0, 0);
            sleep_ms(50);
        }

        // Delay task
        vTaskDelay( dertSENSE_LIGHT_TASK_PERIOD );
    }
}