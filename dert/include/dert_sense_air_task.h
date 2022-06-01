/**
 * Copyright (c) 2022 Kris Keillor
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

// Air Sensor Address
#define SHT30_ADDR 0x44
// Returned Data Size (bytes)
#define BH1750_DAT_RX_SZ 6

// Command opecodes
// Clock stretched read (MSB)
#define SHT30_GET_DATA_CLK_STR 0x2C
// Medium repeatability single measurement (LSB)
#define SHT30_GET_DATA_MED_RPT_CLK_STR 0x0D
// Length of combined command
#define SHT30_GET_DATA_PAIR_CMD_LEN 2

// Read opecodes
// Reads 2B RH + CRC checksum + 2B T + CRC checksum
#define SHT30_GET_DATA_PAIR_LEN 6

// Time to wait between commands in ms
#define SHT30_MEAS_TIME 1

// Constants used to calculate Relative Humidity (RH)
#define BH1750_RH_SCALE 625
#define BH1750_RH_SHIFT 12

// Constants used to calculate Temperature (T)
#define BH1750_T_SCALE 4375
#define BH1750_T_SHIFT 14
#define BH1750_T_OFFSET 4500

static const char SHT30_NAME[] = "SHT30 Air Sensor";

void vDertSenseAir(void *pvParameters);