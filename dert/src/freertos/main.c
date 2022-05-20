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
    stdio_init_all();

    // Wait for PuTTY to connect (USB-only)
    sleep_ms(5000);

    // RP2040 MCU Initialization
    printf("    DERT state: Starting up!\n");
    gpio_init(GPIO_I2C_SDA);
    gpio_init(GPIO_I2C_SCL);
    i2c_init(i2c_default, 100*1000);
    gpio_set_function(GPIO_I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(GPIO_I2C_SCL, GPIO_FUNC_I2C);
    bi_decl(bi_2pins_with_func(GPIO_I2C_SDA, GPIO_I2C_SCL, GPIO_FUNC_I2C));

    gpio_init(GPIO_LED0);
    gpio_init(GPIO_LED1);

    gpio_init(GPIO_HVR);
    gpio_init(GPIO_LVR1);
    gpio_init(GPIO_LVR2);
    gpio_set_dir(GPIO_HVR, GPIO_OUT);
    gpio_set_dir(GPIO_LVR1, GPIO_OUT);
    gpio_set_dir(GPIO_LVR2, GPIO_OUT);

    gpio_init(GPIO_DB1);
    gpio_init(GPIO_DB2);
    gpio_init(GPIO_DB3);

    // FreeRTOS Initialization
    // Initialize FreeRTOS application
    //printf("    DERT state: Starting task scheduler!\n");
    //vTaskStartScheduler();
    //printf("    DERT state: Task scheduler started!\n");

    // Variables for creating tasks
    BaseType_t xTaskCreateRet;
    TaskHandle_t xLightHandle = NULL;
    TaskHandle_t xAirHandle = NULL;
    TaskHandle_t xSoilHandle = NULL;
    TaskHandle_t xReportHandle = NULL;

    //printf("    DERT state: creating sense_air_task!\n");
    // Create dert_sense_air_task
    xTaskCreateRet = xTaskCreate(
                        vDertSenseAir,
                        "SENSE_AIR",
                        dTASK_SIZE,
                        NULL, //(void * ) 0,
                        dertSENSE_AIR_TASK_PRIORITY,
                        NULL); //&xAirHandle);
    if (xTaskCreateRet == pdPASS) {
        //printf("Air sensing task created!\n");
    } else {
        //printf("Air sensing task not created.\n");
    }
    
    //printf("    DERT state: creating sense_light_task!\n");
    // Create dert_sense_air_task
    xTaskCreateRet = xTaskCreate(
                        vDertSenseLight,
                        "SENSE_LIGHT",
                        dTASK_SIZE,
                        (void * ) 0,
                        dertSENSE_LIGHT_TASK_PRIORITY,
                        NULL); //&xLightHandle);
    if (xTaskCreateRet == pdPASS) {
        //printf("Light sensing task created!\n");
    } else {
        //printf("Light sensing task not created.\n");
    }
    
    //printf("    DERT state: creating sense_soil_task!\n");
    // Create dert_sense_,soil_task
    xTaskCreateRet = xTaskCreate(
                        vDertSenseSoil,
                        "SENSE_SOIL",
                        dTASK_SIZE,
                        NULL, //(void * ) 0,
                        dertSENSE_SOIL_TASK_PRIORITY,
                        &xSoilHandle);
    if (xTaskCreateRet == pdPASS) {
        //printf("Soil sensing task created!\n");
    } else {
        //printf("Soil sensing task not created.\n");
    }
    
    //printf("    DERT state: creating report_data_task!\n");
    // Create dert_report_data_task
    xTaskCreateRet = xTaskCreate(
                        vDertReportData,
                        "REPORT_DATA",
                        dTASK_SIZE,
                        (void * ) 0,
                        dertREPORT_DATA_TASK_PRIORITY,
                        &xReportHandle);
    if (xTaskCreateRet == pdPASS) {
        //printf("Data reporting task created!\n");
    } else {
        //printf("Data reporting task not created.\n");
    }
    
    printf("    End of dert init");
    vTaskStartScheduler();

    printf("    vTaskScheduler lost control.");
}
