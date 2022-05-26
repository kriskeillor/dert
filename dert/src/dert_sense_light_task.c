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
    uint8_t light_sns_dat_rx[2];
    int light_sns_dat_err;

    for ( ;; ) {
        printf("DERT state: Requesting light measurement. \n");
        gpio_put(GPIO_DB1, 1); // Debug bit 1 set to indicate sending Opecode

		// Lux reading
        light_sns_dat_tx = BH1750_MEAS_INST_1LX;
		light_sns_dat_err = dert_i2c_sns_wr(BH1750_ADDR,
			&light_sns_dat_tx,
			&light_sns_dat_rx[0],
			BH1750_MEAS_LUX_LEN,
			dertSNS_TIMEOUT_MS,
			(char *)BH1750_NAME);

		if (light_sns_dat_err > 0)
			printf("Read 0x%x%x lux from %s.\n", light_sns_dat_rx[0], light_sns_dat_rx[1], BH1750_NAME);
		else 
			printf("Error: Read 0 bytes from %s.\n", BH1750_NAME);

/*
        light_sns_dat_err = i2c_write_blocking(&i2c1_inst, BH1750_ADDR, &light_sns_dat_tx, 1, false);
        if (light_sns_dat_err == -1 || light_sns_dat_err == -2 || light_sns_dat_err == -3) {
            printf("                Error %d writing to BH1750!", light_sns_dat_err);
            gpio_put(GPIO_DB3, 1); // Debug bit 3 set to indicate I2C Error
        }
        else {
            printf("                Wrote %d byte to BH1750.", light_sns_dat_err);
        }
        gpio_put(GPIO_DB1, 0); // Clear Opecode tx debug bit

        // Wait for the measurement to be complete
        // Typical time-to-measure is 120ms, max is 180ms
        sleep_ms(200);*/

		/*
        printf("    DERT state: Requesting light data.");
        gpio_put(GPIO_DB2, 1); // Debug bit 2 set to indicate sending Request for Data
        light_sns_dat_err = i2c_read_blocking(&i2c1_inst, BH1750_ADDR, &light_sns_dat_rx[0], 2, false);
        if (light_sns_dat_err == -1 || light_sns_dat_err == -2 || light_sns_dat_err == -3) {
            printf("                Error %d reading from BH1750!", light_sns_dat_err);
            gpio_put(GPIO_DB3, 1); // Debug bit 3 set to indicate I2C Error
        }
        else {
            printf("                Read %d bytes from BH1750.", light_sns_dat_err);
            printf("                Raw lux measurement: %x%x", light_sns_dat_rx[0], light_sns_dat_rx[1]);
        }*/
        gpio_put(GPIO_DB1, 0); // Clear data request debug bit

        // (Placeholder) Enable Low-Voltage Relays
        printf("                Controlling lights!");
        gpio_put(GPIO_HVR, 0);

        // Debugging
        gpio_put(GPIO_DB3, 0); // Debug bit 3 cleared to indicate no I2C Error (yet)
        light_sns_dat_err = 0;    // Clear I2C error variable

        vTaskDelay( dertSENSE_LIGHT_TASK_PERIOD );
    }
}