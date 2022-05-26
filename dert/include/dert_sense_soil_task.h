/**
 * Copyright (c) 2022 Kris Keillor
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

// Chirp Soil Sensors 1 & 2 Addresses
#define MOISTURE_SNS_1_ADDR 0x20
#define MOISTURE_SNS_2_ADDR 0x21


// Read opecodes
#define CHIRP_GET_CAP 0x00
#define CHIRP_GET_CAP_LEN 2
#define CHIRP_GET_ADDR 0x02
#define CHIRP_GET_ADDR_LEN 1
#define CHIRP_GET_TEMP 0x05
#define CHIRP_GET_TEMP_LEN 2
#define CHIRP_GET_VER 0x07
#define CHIRP_GET_VER_LEN 1
#define CHIRP_GET_BUSY 0x09
#define CHIRP_GET_BUSY_LEN 1

// Write opecodes
#define CHIRP_SET_ADDR 0x01
#define CHIRP_SET_ADDR_LEN 1
#define CHIRP_RESET 0x06
#define CHIRP_RESET_LEN 0
#define CHIRP_SET_SLEEP 0x08
#define CHIRP_SET_SLEEP_LEN 0

#define CHIRP_CMD_LEN 1

static const char CHIRP_NAME[] = "Chirp Soil Sensor";

// Unused opecodes
// * Writes
// #define CHIRP_MEAS_LIGHT 0x03    // light sensor obscured on 'Rugged' Chirp Sensors
// #define CHIRP_MEAS_LIGHT_LEN 0   // light sensor obscured
// * Reads
// #define CHIRP_GET_LIGHT 0x04     // light sensor obscured
// #define CHIRP_GET_LIGHT_LEN 2    // light sensor obscured

void vDertSenseSoil(void *pvParameters);