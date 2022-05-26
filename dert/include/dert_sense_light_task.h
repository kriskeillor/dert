/**
 * Copyright (c) 2022 Kris Keillor
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

// Light Sensor Address (preset)
#define BH1750_ADDR 0x23

// Danger: These opecodes introduce unwanted behavior
#define BH1750_PWR_OFF 0x00
#define BH1750_PWR_ON 0x01
#define BH1750_RESET 0x07

// Safe: Continuous Measurement opecodes
#define BH1750_MEAS_CONT_1LX 0x10
#define BH1750_MEAS_CONT_05LX 0x11
#define BH1750_MEAS_CONT_4LX 0x13
// Safe: One-Time Measurement Opecodes
#define BH1750_MEAS_INST_1LX 0x20
#define BH1750_MEAS_INST_05LX 0x21
#define BH1750_MEAS_INST_4LX 0x23

// FreeRTOS measurement task
void vDertSenseLight(void *pvParameters);