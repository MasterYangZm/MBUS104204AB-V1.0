#ifndef _ADS1110_H_
#define _ADS1110_H_
/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

#define I2C_WR                  0  /* write enable */
#define I2C_RD                  1  /* read enable */

enum ENUM_I2C_REPLY
{
  I2C_NACK = 0,
  I2C_ACK = 1,
};

enum ENUM_I2C_BUS_STATE
{
  I2C_READY = 0,
  I2C_BUS_BUSY = 1,
  I2C_BUS_ERROR = 2
};
/* Private define ------------------------------------------------------------*/
#define I2C_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOB_CLK_ENABLE()
#define I2C_GPIO_PORT0          GPIOB
#define I2C_GPIO_PORT1          GPIOB
#define I2C_GPIO_PORT2          GPIOB
#define I2C_GPIO_PORT3          GPIOB
#define I2C_SCL0_PIN            GPIO_PIN_8
#define I2C_SDA0_PIN            GPIO_PIN_9
#define I2C_SCL1_PIN            GPIO_PIN_10
#define I2C_SDA1_PIN            GPIO_PIN_11
#define I2C_SCL2_PIN            GPIO_PIN_12
#define I2C_SDA2_PIN            GPIO_PIN_13
#define I2C_SCL3_PIN            GPIO_PIN_14
#define I2C_SDA3_PIN            GPIO_PIN_15
#define I2C_SCL_HIGH()          HAL_GPIO_WritePin(I2C_GPIO_PORT, I2C_SCL_PIN, GPIO_PIN_SET)
#define I2C_SCL_LOW()           HAL_GPIO_WritePin(I2C_GPIO_PORT, I2C_SCL_PIN, GPIO_PIN_RESET)
#define I2C_SDA_HIGH()          HAL_GPIO_WritePin(I2C_GPIO_PORT, I2C_SDA_PIN, GPIO_PIN_SET)
#define I2C_SDA_LOW()           HAL_GPIO_WritePin(I2C_GPIO_PORT, I2C_SDA_PIN, GPIO_PIN_RESET)
#define I2C_SDA_STATE()         HAL_GPIO_ReadPin(I2C_GPIO_PORT, I2C_SDA_PIN)

#define ADS1110_WR_ADRRESS      0x92
#define ADS1110_RD_ADRRESS      0x93
#define ADS1110_CONFIG_REG      0x8C

/* Private function prototypes -----------------------------------------------*/
uint8_t I2C_START(void);
void I2C_STOP(void);
uint8_t I2C_SendByte(uint8_t Byte);
uint8_t I2C_ReadByte(void);
uint8_t I2C_WaitACK(void);
void I2C_SendACK(void);
void I2C_SendNACK(void);
void ADS1110_Config(void);
uint16_t ReadADS1110(void);
uint16_t Get_ADC_Value(void);
void ADS1110_Init(void);

#endif
