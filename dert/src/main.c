/* Library Includes */
#include <stdio.h>

/* FreeRTOS Includes */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

/* RP-2040 Includes */
#include "pico/stdio.h"
#include "pico/stdlib.h"

#include "pico/sync.h"
#include "pico/multicore.h"

#include "pico/binary_info.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"

/* Dert includes */
#include "dert_2040.h"
#include "dert.h"
#include "dert_sense_air_task.h"
#include "dert_sense_light_task.h"
#include "dert_sense_soil_task.h"
#include "dert_report_data_task.h"

int main() {
	// Initialize serial interface
    stdio_init_all();
    printf("DERT initializing...\n");

    // RP2040 MCU Initialization
    // I2C Initialization
    gpio_init(GPIO_I2C_SDA);
    gpio_init(GPIO_I2C_SCL);
    i2c_init(&i2c1_inst, 100*1000);
    gpio_set_function(GPIO_I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(GPIO_I2C_SCL, GPIO_FUNC_I2C);
    bi_decl(bi_2pins_with_func(GPIO_I2C_SDA, GPIO_I2C_SCL, GPIO_FUNC_I2C));
    gpio_pull_up(GPIO_I2C_SCL);
    gpio_pull_up(GPIO_I2C_SDA);

	// Initialize indicator LED pins
    gpio_init(GPIO_LED0);
    gpio_init(GPIO_LED1);
    gpio_set_dir(GPIO_LED0, GPIO_OUT);
    gpio_set_dir(GPIO_LED1, GPIO_OUT);

	// Initialize relay control pins
    gpio_init(GPIO_HVR);
    gpio_init(GPIO_LVR1);
    gpio_init(GPIO_LVR2);
    gpio_set_dir(GPIO_HVR, GPIO_OUT);
    gpio_set_dir(GPIO_LVR1, GPIO_OUT);
    gpio_set_dir(GPIO_LVR2, GPIO_OUT);

	// Initialize debug bits 
    gpio_init(GPIO_DB1);
    gpio_init(GPIO_DB2);
    gpio_init(GPIO_DB3);
    gpio_set_dir(GPIO_DB1, GPIO_OUT);
    gpio_set_dir(GPIO_DB2, GPIO_OUT);
    gpio_set_dir(GPIO_DB3, GPIO_OUT);

    // Indicate startup is finished, waiting 5s for PuTTY connection
    gpio_put(GPIO_LED0, 1);

    // Wait for PuTTY to connect (USB-only)
    sleep_ms(5000);

    // Debug
    // Checking pullup status
    bool scl_pulled_up = gpio_is_pulled_up(GPIO_I2C_SCL);
    bool scl_pulled_down = gpio_is_pulled_down(GPIO_I2C_SCL);
    bool sda_pulled_up = gpio_is_pulled_up(GPIO_I2C_SDA);
    bool sda_pulled_down = gpio_is_pulled_down(GPIO_I2C_SDA);
    printf("    SCL pulled up: %s; pulled down: %s.", scl_pulled_up ? "true" : "false", scl_pulled_down ? "true" : "false");
    printf("    SDA pulled up: %s; pulled down: %s.", sda_pulled_up ? "true" : "false", sda_pulled_down ? "true" : "false");

    // Indicate wait is finished, starting FreeRTOS
    gpio_put(GPIO_LED0, 0);
    gpio_put(GPIO_LED1, 1);

    // Variables for creating tasks
    BaseType_t xTaskCreateRet;
    TaskHandle_t xLightHandle = NULL;
    TaskHandle_t xAirHandle = NULL;
    TaskHandle_t xSoilHandle = NULL;
    TaskHandle_t xReportHandle = NULL;

    // Create dert_sense_air_task
    xTaskCreateRet = xTaskCreate(
                        vDertSenseAir,
                        "SENSE_AIR",
                        dTASK_SIZE,
                        NULL, //(void * ) 0,
                        dertSENSE_AIR_TASK_PRIORITY,
                        &xAirHandle);
    if (xTaskCreateRet != pdPASS) {
        // error state
    }

    // Create dert_sense_air_task
    xTaskCreateRet = xTaskCreate(
                        vDertSenseLight,
                        "SENSE_LIGHT",
                        dTASK_SIZE,
                        (void * ) 0,
                        dertSENSE_LIGHT_TASK_PRIORITY,
                        &xLightHandle);
    if (xTaskCreateRet != pdPASS) {
        // error state
    }

    // Create dert_sense_,soil_task
    xTaskCreateRet = xTaskCreate(
                        vDertSenseSoil,
                        "SENSE_SOIL",
                        dTASK_SIZE,
                        (void * ) 0,
                        dertSENSE_SOIL_TASK_PRIORITY,
                        &xSoilHandle);
    if (xTaskCreateRet != pdPASS) {
        // error state
    }

    // Create dert_report_data_task
    xTaskCreateRet = xTaskCreate(
                        vDertReportData,
                        "REPORT_DATA",
                        dTASK_SIZE,
                        (void * ) 0,
                        dertREPORT_DATA_TASK_PRIORITY,
                        &xReportHandle);
    if (xTaskCreateRet == pdPASS) {
        // error state
    }

	// RTOS should never return from this call
    vTaskStartScheduler();
	// If it did...
    printf("    vTaskScheduler lost control.");
    // Indicate error state
    gpio_put(GPIO_LED0, 1);
}
