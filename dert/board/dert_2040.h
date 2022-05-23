#ifndef DERT_HEADER
#define DERT_HEADER

// I2C Bus
#define GPIO_I2C_SDA 10
#define GPIO_I2C_SCL 11

// Status indicator GPIOs
#define GPIO_LED0 14
#define GPIO_LED1 15

// Relay GPIOs
#define GPIO_HVR 18
#define GPIO_LVR1 19
#define GPIO_LVR2 20

// Debug bit GPIOs
#define GPIO_DB1 22
#define GPIO_DB2 23
#define GPIO_DB3 24

// SENSORS
// Air Sensor
#define SHT30_ADDR 0x44
// Soil Sensors 1 & 2
#define MOISTURE_SNS_1_ADDR 0x20
#define MOISTURE_SNS_2_ADDR 0x21

#endif
