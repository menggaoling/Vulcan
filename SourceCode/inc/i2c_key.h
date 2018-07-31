#ifndef I2C_KEY_H_
#define I2C_KEY_H_

#include  "General.h"

#define ADDR               0x70
#define MAX_KEY_NUM        4  // must less than 16
#define INT_STATUS()       HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_11)

void i2cInit(void);
bool i2cWriteBytes(uint8_t dev, uint8_t reg, uint8_t len, uint8_t *data);
bool i2cReadBytes(uint8_t dev, uint8_t reg, uint8_t len, uint8_t *data);
bool i2cWriteOneByte(uint8_t dev, uint8_t reg, uint8_t data);


#endif // I2C_KEY_H_