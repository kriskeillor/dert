static void vDertSenseSoil(void *pvParameters) {
    for ( ;; ) {
        printf("    DERT state: Sensing soil!\n");
        // Sensor 1
        moisture_sns_dat_tx = 0x02;
        moisture_sns_dat_err = i2c_write_blocking(&i2c1_inst, MOISTURE_SNS_1_ADDR, &moisture_sns_dat_tx, 1, false);
        if (moisture_sns_dat_err < 0)
            printf("                Error %d writing to Moisture Sensor 1!\n", moisture_sns_dat_err);
        else
            printf("                Wrote %d byte to Moisture Sensor 1.\n", moisture_sns_dat_err);

        moisture_sns_dat_err = i2c_read_blocking(&i2c1_inst, MOISTURE_SNS_1_ADDR, moisture_sns_dat_rx, 1, false);
        if (moisture_sns_dat_err < 0)
            printf("                Error %d reading from Moisture Sensor 1!\n", moisture_sns_dat_err);
        else {
            printf("                Wrote %d byte to Moisture Sensor 1.\n", moisture_sns_dat_err);
            printf("                Raw address reading: %d\n", moisture_sns_dat_rx);
        }

        // Sensor 2
        moisture_sns_dat_tx = 0x02;
        moisture_sns_dat_err = i2c_write_blocking(&i2c1_inst, MOISTURE_SNS_2_ADDR, &moisture_sns_dat_tx, 1, false);
        if (moisture_sns_dat_err < 0)
            printf("                Error %d writing to Moisture Sensor 2!\n", moisture_sns_dat_err);
        else
            printf("                Wrote %d byte to Moisture Sensor 2.\n", moisture_sns_dat_err);

        moisture_sns_dat_err = i2c_read_blocking(&i2c1_inst, MOISTURE_SNS_2_ADDR, moisture_sns_dat_rx, 1, false);
        if (moisture_sns_dat_err < 0)
            printf("                Error %d reading from Moisture Sensor 2!\n", moisture_sns_dat_err);
        else {
            printf("                Wrote %d byte to Moisture Sensor 2.\n", moisture_sns_dat_err);
            printf("                Raw address reading: %d\n", moisture_sns_dat_rx);
        }
    }
}