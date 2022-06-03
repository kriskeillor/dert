/* Defines */
#ifndef mainRUN_FREE_RTOS_ON_CORE
#define mainRUN_FREE_RTOS_ON_CORE 0
#endif
/* FreeROTS "don't block" indicator */
#define DONT_BLOCK                          ( 0UL )

// RP-2040 Definitions
#define UART_ID                             ( uart0 )
#define UART_BAUD_RATE                      ( 115200 )

// Dert Config Definitions
#define dertSNS_TIMEOUT_MS                  ( 100000 )
#define dertVERBOSE_LOGS                    ( 0UL )

/* Dert Task Definitions */
#define dTASK_SIZE                          ( 4096 )

/* Priorities for DERT tasks. */
#define dertSENSE_SOIL_TASK_PRIORITY        (tskIDLE_PRIORITY + 3UL)
#define dertSENSE_LIGHT_TASK_PRIORITY       (tskIDLE_PRIORITY + 2UL)
#define dertSENSE_AIR_TASK_PRIORITY         (tskIDLE_PRIORITY + 1UL)

/* Periods for DERT tasks. */
#define dertSENSE_SOIL_TASK_PERIOD          pdMS_TO_TICKS( 10000UL )
#define dertSENSE_LIGHT_TASK_PERIOD         pdMS_TO_TICKS( 10000UL )
#define dertSENSE_AIR_TASK_PERIOD           pdMS_TO_TICKS( 10000UL )

/* Priority/period for relay toggle test/demo. */
#define dertTOGGLE_RELAYS_TASK_PRIORITY     (tskIDLE_PRIORITY + 4UL)
#define dertTOGGLE_RELAYS_TASK_PERIOD       pdMS_TO_TICKS( 50UL )