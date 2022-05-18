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

int main() {
    stdio_init_all();

    // Wait for PuTTY to connect (USB-only)
    sleep_ms(5000);

    // Initialize RP2040 MCU
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

    // Initialize FreeRTOS application
    vTaskStartScheduler();
    BaseType_t xTaskCreateRet;
    TaskHandle_t xLightHandle = NULL;
    xTaskCreateRet = xTaskCreate(
                        vDertSenseAir,
                        "SENSE_AIR",
                        dTASK_SIZE,
                        (void * ) 0,
                        dertSENSE_LIGHT_TASK_PRIORITY,
                        &xLightHandle);

    if (xTaskCreateRet == pdPASS) {
        printf("Task created!");
    } else {
        printf("Task not created.");
    }
}
