static void vDertSenseAir(void *pvParameters) {
    for ( ;; ) { 
        printf("    DERT state: Sensing air!\n");
        // (Placeholder) Disable Loww-Voltage Relays
        gpio_put(GPIO_LVR1, 0); // Disable pump 1 relay
        gpio_put(GPIO_LVR2, 0); // Disable pump 2 relay
        state = SenseLight;
    }
}