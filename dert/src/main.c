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

#include "hardware/uart.h"

/* Dert includes */
#include "dert_2040.h"
#include "dert.h"
#include "dert_sense_air_task.h"
#include "dert_sense_light_task.h"
#include "dert_sense_soil_task.h"
#include "dert_toggle_relays_task_test.h"

int main() {
	// Initialize serial interface
    stdio_init_all();

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

    // UART Initialization
    uart_init(UART_ID, UART_BAUD_RATE);
    gpio_set_function(UART_TX, GPIO_FUNC_UART);
    gpio_set_function(UART_RX, GPIO_FUNC_UART);
    // * Disable RTS/CTS
    uart_set_hw_flow(UART_ID, false, false);


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

    // Indicate RP2040 init is complete
    if (dertVERBOSE_LOGS) {
        printf("\nRP-2040 initialization completed.\n");
    }
    gpio_put(GPIO_LED1, 1);
    sleep_ms(500);
    gpio_put(GPIO_LED1, 0);

    // Variables for creating tasks
    BaseType_t xTaskCreateRet;
    TaskHandle_t xLightHandle = NULL;
    TaskHandle_t xAirHandle = NULL;
    TaskHandle_t xSoilHandle = NULL;
    TaskHandle_t xReportHandle = NULL;

    // Relay debugging
    TaskHandle_t xRelayHandle = NULL;

    // Create dert_sense_air_task
    xTaskCreateRet = xTaskCreate(
                        vDertSenseAir,
                        "SENSE_AIR",
                        dTASK_SIZE,
                        (void * ) 0,
                        dertSENSE_AIR_TASK_PRIORITY,
                        &xAirHandle);
    // Debug Output (two checks)
    if (xTaskCreateRet != pdPASS) {
        printf("! Error creating task vDertSenseAir.\n");
    } else if (dertVERBOSE_LOGS) {
        printf("Created task vDertSenseAir.\n");
    } else { }

    // Create dert_sense_air_task
    xTaskCreateRet = xTaskCreate(
                        vDertSenseLight,
                        "SENSE_LIGHT",
                        dTASK_SIZE,
                        (void * ) 0,
                        dertSENSE_LIGHT_TASK_PRIORITY,
                        &xLightHandle);
    // Debug Output (two checks)
    if (xTaskCreateRet != pdPASS) {
        printf("! Error creating task vDertSenseLight.\n");
    } else if (dertVERBOSE_LOGS) {
        printf("Created task vDertSenseLight.\n");
    } else { }

    // Create dert_sense_,soil_task
    xTaskCreateRet = xTaskCreate(
                        vDertSenseSoil,
                        "SENSE_SOIL",
                        dTASK_SIZE,
                        (void * ) 0,
                        dertSENSE_SOIL_TASK_PRIORITY,
                        &xSoilHandle);
    // Debug Output (two checks)
    if (xTaskCreateRet != pdPASS) {
        printf("! Error creating task vDertSenseSoil.\n");
    } else if (dertVERBOSE_LOGS) {
        printf("Created task vDertSenseSoil.\n");
    } else { }

    // Create dert_toggle_relays_task
    xTaskCreateRet = xTaskCreate(
                        vDertToggleRelays,
                        "TOGGLE_RELAYS_TEST",
                        dTASK_SIZE,
                        (void * ) 0,
                        dertTOGGLE_RELAYS_TASK_PRIORITY,
                        &xRelayHandle);
    // Debug Output (two checks)
    if (xTaskCreateRet != pdPASS) {
        printf("! Error creating task vDertToggleRelays.\n");
    } else if (dertVERBOSE_LOGS) {
        printf("Created task vDertToggleRelays.\n");
    } else { }

    // Indicate FreeRTOS init is complete
    if (dertVERBOSE_LOGS) {
        printf("\nDert initialization completed.\n");
    }
    gpio_put(GPIO_LED0, 1);
    sleep_ms(500);
    gpio_put(GPIO_LED0, 0);

	// RTOS should never return from this call
    vTaskStartScheduler();

	// If it did...
    for ( ;; ) {
        printf("! Error: vTaskScheduler lost control.");
        gpio_put(GPIO_LED0, 1);
        sleep_ms(500);
        gpio_put(GPIO_LED0, 0);
    }
}