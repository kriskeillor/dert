static void vDertSenseLight(void *pvParameters) {
    for ( ;; ) {
        printf("    DERT state: Sensing light!\n");
        bh1750_dat_tx = 0b00010000; // Continuous H-Res Mode instruction
        bh1750_dat_err = i2c_write_blocking(&i2c1_inst, BH1750_ADDR, &bh1750_dat_tx, 1, false);
        if (bh1750_dat_err == -1 || bh1750_dat_err == -2 || bh1750_dat_err == -3)
            printf("                Error %d writing to BH1750!\n", bh1750_dat_err);
        else
            printf("                Wrote %d byte to BH1750.\n", bh1750_dat_err);

        sleep_ms(185);

        bh1750_dat_err = i2c_read_blocking(&i2c1_inst, BH1750_ADDR, bh1750_dat_rx, 2, false);
        if (bh1750_dat_err == -1 || bh1750_dat_err == -2 || bh1750_dat_err == -3)
            printf("                Error %d reading from BH1750!\n", bh1750_dat_err);
        else {
            printf("                Read %d bytes from BH1750.\n", bh1750_dat_err);
            printf("                Raw lux measurement: %d\n", bh1750_dat_rx);
        }

        printf("                Controlling lights!\n");
        gpio_put(GPIO_HVR, 1);
        state = ReportData;
    }
}