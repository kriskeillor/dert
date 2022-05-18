/* Includes */
#include "Sync.h"

/* Stubs for all DERT tasks. */
/* void dert( void ); */
static void vDertSenseSoil(void *pvParameters);
static void vDertSenseLight(void *pvParameters);
static void vDertSenseAir(void *pvParameters);
static void vDertReportData(void *pvParameters);