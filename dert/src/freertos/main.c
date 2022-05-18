/* Includes */

/* Priorities for DERT tasks. */
#define dertREPORT_DATA_TASK_PRIORITY       (tskIDLE_PRIORITY + 1UL)
#define dertSENSE_SOIL_TASK_PRIORITY        (tskIDLE_PRIORITY + 2UL)
#define dertSENSE_LIGHT_TASK_PRIORITY       (tskIDLE_PRIORITY + 3UL)
#define dertSENSE_AIR_TASK_PRIORITY         (tskIDLE_PRIORITY + 4UL)

/* Means "don't block" */
#define dertDONT_BLOCK                      ( 0UL )

/* Periods for DERT tasks. */
#define dertREPORT_DATA_TASK_PRIORITY       pdMS_TO_TICKS( 10000UL )
#define dertSENSE_SOIL_TASK_PRIORITY        pdMS_TO_TICKS( 10000UL )
#define dertSENSE_LIGHT_TASK_PRIORITY       pdMS_TO_TICKS( 10000UL )
#define dertSENSE_AIR_TASK_PRIORITY         pdMS_TO_TICKS( 10000UL )
