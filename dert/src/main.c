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
#include "dert_report_data_task.h"
#include "dert_toggle_relays_task_test.h"

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

    // UART Initialization
    uart_init(UART_ID, UART_BAUD_RATE);
    gpio_set_function(UART_TX, GPIO_FUNC_UART);
    gpio_set_function(UART_RX, GPIO_FUNC_UART);

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

    // Indicate wait is finished, starting FreeRTOS
    gpio_put(GPIO_LED0, 0);
    gpio_put(GPIO_LED1, 1);

    // FreeRTOS Initialization
    // Variables for queue handles
    QueueHandle_t xQLuxHandle = NULL;
    QueueHandle_t xQAirHumHandle = NULL;
    QueueHandle_t xQAirTempHandle = NULL;
    QueueHandle_t xQSoilTempHandle = NULL;
    QueueHandle_t xQSoilMoisHandle = NULL;

    xQLuxHandle = xQueueCreate(dQUEUE_SIZE, BH1750_DAT_SZ);
    if (xQLuxHandle != NULL) {
        if (dertVERBOSE_LOGS)
            printf("Created xQLuxHandle.\n");
            printf("xQLuxHandle Address: %p", xQLuxHandle);
    } else 
        printf("! Error creating xQLuxHandle: Result null.\n");

    xQAirHumHandle = xQueueCreate(dQUEUE_SIZE, BH1750_DAT_SZ);
    if (xQAirHumHandle != NULL) {
        if (dertVERBOSE_LOGS)
            printf("Created xQAirHumHandle.\n");
    } else 
        printf("! Error creating xQAirHumHandle: Result null.\n");

    xQAirTempHandle = xQueueCreate(dQUEUE_SIZE, BH1750_DAT_SZ);
    if (xQAirTempHandle != NULL) {
        if (dertVERBOSE_LOGS)
            printf("Created xQAirTempHandle.\n");
    } else 
        printf("! Error creating xQAirTempHandle: Result null.\n");

    xQSoilTempHandle = xQueueCreate(dQUEUE_SIZE, BH1750_DAT_SZ);
    if (xQSoilTempHandle != NULL) {
        if (dertVERBOSE_LOGS)
            printf("Created xQSoilTempHandle.\n");
    } else 
        printf("! Error creating xQSoilTempHandle: Result null.\n");

    xQSoilMoisHandle = xQueueCreate(dQUEUE_SIZE, BH1750_DAT_SZ);
    if (xQSoilMoisHandle != NULL) {
        if (dertVERBOSE_LOGS)
            printf("Created xQSoilMoisHandle.\n");
    } else 
        printf("! Error creating xQSoilMoisHandle: Result null.\n");

    // Queue Initializations complete
    if (dertVERBOSE_LOGS) {
        printf("\nDert queue initializations completed.\n\n");
    }

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
    if (xAirHandle == NULL) {
        printf("! Error: xReportHandle null.\n");
    } else if (dertVERBOSE_LOGS) {
        printf("xReportHandle not null.\n");
    } else { }

    // Create dert_sense_light_task
    xTaskCreateRet = xTaskCreate(
                        vDertSenseLight,
                        "SENSE_LIGHT",
                        dTASK_SIZE,
                        (void * ) xQLuxHandle,
                        dertSENSE_LIGHT_TASK_PRIORITY,
                        &xLightHandle);
    // Debug Output (two checks)
    if (xTaskCreateRet != pdPASS) {
        printf("! Error creating task vDertSenseLight.\n");
    } else if (dertVERBOSE_LOGS) {
        printf("Created task vDertSenseLight.\n");
    } else { }
    if (xLightHandle == NULL) {
        printf("! Error: xReportHandle null.\n");
    } else if (dertVERBOSE_LOGS) {
        printf("xReportHandle not null.\n");
    } else { }

    // Create dert_sense_soil_task
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
    if (xSoilHandle == NULL) {
        printf("! Error: xReportHandle null.\n");
    } else if (dertVERBOSE_LOGS) {
        printf("xReportHandle not null.\n");
    } else { }

    // Create dert_report_data_task
    xTaskCreateRet = xTaskCreate(
                        vDertReportData,
                        "REPORT_DATA",
                        dTASK_SIZE,
                        (void * ) 0,
                        dertREPORT_DATA_TASK_PRIORITY,
                        &xReportHandle);
    // Debug Output (two checks)
    if (xTaskCreateRet != pdPASS) {
        printf("! Error creating task vDertReportData.\n");
    } else if (dertVERBOSE_LOGS) {
        printf("Created task vDertReportData.\n");
    } else { }
    if (xReportHandle == NULL) {
        printf("! Error: xReportHandle null.\n");
    } else if (dertVERBOSE_LOGS) {
        printf("xReportHandle not null.\n");
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
    if (xRelayHandle == NULL) {
        printf("! Error: xReportHandle null.\n");
    } else if (dertVERBOSE_LOGS) {
        printf("xReportHandle not null.\n");
    } else { }

    if (dertVERBOSE_LOGS) {
        printf("\nDert task initialization completed.\n\n");
    }

	// RTOS should never return from this call
    vTaskStartScheduler();
	// If it did...
    printf("! Error: vTaskScheduler lost control.");
    // Indicate error state
    gpio_put(GPIO_LED0, 1);
}
