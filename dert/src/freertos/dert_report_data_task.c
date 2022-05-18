static void vDertReportData(void *pvParameters) {
    for ( ;; ) {
        printf("    DERT state: Reporting data!\n");
        printf("                Reporting errors!\n");
        gpio_put(GPIO_HVR, 0); // Disable light relay
        state = SenseSoil;
    }
}