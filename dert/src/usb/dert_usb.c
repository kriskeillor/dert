/**
 * Copyright (c) 2022 Kris Keillor
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>

#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"

#include "dert_usb.h"

//#include "boards/dert.h" // Note: was successfully located, but it caused glitches

int main() {
    stdio_init_all();
    int lifespan = 0;
    int state = Startup;

    // Light sensor fields
    uint8_t bh1750_dat_tx;
    uint8_t bh1750_dat_rx[2];
    int bh1750_dat_err;

    // Air sensor fields
    int8_t sht30_dat;

    // Wait for PuTTY to connect (USB-only)
    sleep_ms(5000);

    while (true) {
        printf("\nDERT Reporting! %d\n", lifespan);
        lifespan = lifespan + 1;

        switch (state) {
            // TODO: move functionality in this case to separate method
            case Startup:
                printf("    DERT state: Starting up!\n");
                gpio_init(GPIO_I2C_SDA);
                gpio_init(GPIO_I2C_SCL);
                i2c_init(i2c_default, 100*1000);
                gpio_set_function(GPIO_I2C_SDA, GPIO_FUNC_I2C);
                gpio_set_function(GPIO_I2C_SCL, GPIO_FUNC_I2C);
                //gpio_pull_up(GPIO_I2C_SDA); // Hardware pullups
                //gpio_pull_up(GPIO_I2C_SCL); // Hardware pullups
                bi_decl(bi_2pins_with_func(GPIO_I2C_SDA, GPIO_I2C_SCL, GPIO_FUNC_I2C));

                // gpio_init(GPIO_LED0); ///!\ No current-limiting resistor in board rev 0.9.3
                // gpio_init(GPIO_LED1); ///!\ No resistor in board rev 0.9.3

                gpio_init(GPIO_HVR);
                gpio_init(GPIO_LVR1);
                gpio_init(GPIO_LVR2);
                gpio_set_dir(GPIO_HVR, GPIO_OUT);
                gpio_set_dir(GPIO_LVR1, GPIO_OUT);
                gpio_set_dir(GPIO_LVR2, GPIO_OUT);

                //gpio_init(GPIO_DB1); ///!\ Not brought out in board rev 0.9.3
                //gpio_init(GPIO_DB2); ///!\ Not used in rev 0.9.3
                //gpio_init(GPIO_DB3); ///!\ Not used in rev 0.9.3

                state = SenseSoil;
                break;

            case SenseSoil:
                printf("    DERT state: Sensing soil!\n");
                printf("                Controlling pumps!\n");
                gpio_put(GPIO_LVR1, 1);
                gpio_put(GPIO_LVR2, 1);
                i2c_scan();
                state = SenseAir;
                break;

            case SenseAir:
                printf("    DERT state: Sensing air!\n");
                gpio_put(GPIO_LVR1, 0); // Disable pump relay
                gpio_put(GPIO_LVR2, 0); // Disable pump relay
                state = SenseLight;
                break;

            case SenseLight:
                printf("    DERT state: Sensing light!\n");
                bh1750_dat_tx = 0b00010000; // Continous H-Res Mode instruction
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
                break;

            case ReportData:
                printf("    DERT state: Reporting data!\n");
                printf("                Reporting errors!\n");
                gpio_put(GPIO_HVR, 0); // Disable light relay
                state = SenseSoil;
                break;

            default:
                printf("/!\\ DERT state out-of-bounds at %d\n", state);
                break;
        }

        sleep_ms(1000);
    }
    return 0;
}

// Exclude reserved addresses, of the form 000 0xxx or 
// 111 1xxx, from the search.
bool i2c_res_addr(uint8_t addr) {
    return (addr & 0x78) == 0 || (addr & 0x78) == 0x78;
}

void i2c_scan() {
    printf("    I2C Bus Scan\n");
    printf("    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n");
    for (int addr = 0; addr < (1 << 7); ++addr) {
        if (addr % 16 == 0) {
            printf("%02x  ", addr);
        }

        // Attempt read from I2C address. If acknowledged,
        // the function returns the #/bytes transferred. If ignored,
        // the function returns -2.
        int ret;
        uint8_t rxdata;
        if (i2c_res_addr(addr))
            ret = -1;//PICO_ERROR_GENERIC;
        else
            // TODO: Configure dert board file with dert_default_i2c define
            //       Currently bad practice
            ret = i2c_read_blocking(&i2c1_inst, addr, &rxdata, 1, false);

        printf(ret < 0 ? "." : "@");
        printf(addr % 16 == 15 ? "\n" : "  ");
    }
    printf("I2C scan completed.\n");
    return;
}
