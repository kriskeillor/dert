#ifndef DERT_HEADER
#define DERT_HEADER

enum State{Startup, SenseSoil, SenseAir, SenseLight, ReportData};

#define GPIO_I2C_SDA 10
#define GPIO_I2C_SCL 11

#define GPIO_LED0 14
#define GPIO_LED1 15

#define GPIO_HVR 18
#define GPIO_LVR1 19
#define GPIO_LVR2 20

// Unused in board rev 0.9.3
#define GPIO_DB1 22
#define GPIO_DB2 23
#define GPIO_DB3 24

bool i2c_res_addr(uint8_t addr);
void i2c_scan(void);

// Sensors
#define BH1750_ADDR 0x23
#define SHT30_ADDR 0x44

#endif
