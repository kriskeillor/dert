/* Defines */
#ifndef mainRUN_FREE_RTOS_ON_CORE
#define mainRUN_FREE_RTOS_ON_CORE 0
#endif
/* FreeROTS "don't block" indicator */
#define DONT_BLOCK                          ( 0UL )

/* Dert Task Definitions */
#define dTASK_SIZE                          ( 8192 )

/* Priorities for DERT tasks. */
#define dertREPORT_DATA_TASK_PRIORITY       (tskIDLE_PRIORITY + 1UL)
#define dertSENSE_SOIL_TASK_PRIORITY        (tskIDLE_PRIORITY + 1UL)
#define dertSENSE_LIGHT_TASK_PRIORITY       (tskIDLE_PRIORITY + 1UL)
#define dertSENSE_AIR_TASK_PRIORITY         (tskIDLE_PRIORITY + 1UL)

/* Periods for DERT tasks. */
#define dertREPORT_DATA_TASK_PERIOD         pdMS_TO_TICKS( 10000UL )
#define dertSENSE_SOIL_TASK_PERIOD          pdMS_TO_TICKS( 10000UL )
#define dertSENSE_LIGHT_TASK_PERIOD         pdMS_TO_TICKS( 10000UL )
#define dertSENSE_AIR_TASK_PERIOD           pdMS_TO_TICKS( 10000UL )

/* TODO: Refactor I2C */ 
bool i2c_res_addr(uint8_t addr);
void i2c_scan(void);