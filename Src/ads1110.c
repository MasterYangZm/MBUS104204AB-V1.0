#include "ads1110.h"

GPIO_TypeDef* I2C_GPIO_PORT;
uint16_t I2C_SCL_PIN;
uint16_t I2C_SDA_PIN;

/* delay */
void I2C_Delay(uint8_t i)
{
  while(i--);
}

/* i2c start signal */
uint8_t I2C_START(void)
{
  I2C_SDA_HIGH();
  I2C_SCL_HIGH();
  I2C_Delay(15);
  
  if(!I2C_SDA_STATE())
    return I2C_BUS_BUSY;
  I2C_SDA_LOW();
  I2C_Delay(15);
  if(I2C_SDA_STATE())
    return I2C_BUS_ERROR;
  I2C_SCL_LOW();
  I2C_Delay(15);
  
  return I2C_READY;
}
	
/* i2c stop signal */
void I2C_STOP(void)
{
  I2C_SDA_LOW();
  I2C_SCL_HIGH();
  I2C_Delay(15);
  I2C_SDA_HIGH();
}

/* send a byte, Bit7 first */
uint8_t I2C_SendByte(uint8_t Byte)
{
  uint8_t i;

  for(i=0; i<8; i++)
  {
    if(Byte&0x80)
      I2C_SDA_HIGH();	
    else
      I2C_SDA_LOW();    
    
    I2C_Delay(15);
    I2C_SCL_HIGH();
    I2C_Delay(15);
    I2C_SCL_LOW();
    
    if(i==7)
      I2C_SDA_HIGH();
    Byte <<= 1;
    I2C_Delay(15);
  }
  
  if(I2C_SDA_STATE())
    return I2C_NACK;
  return I2C_ACK; 
}

/* read a byte, Bit7 first */
uint8_t I2C_ReadByte(void)
{
  uint8_t i;
  uint8_t Data = 0;

  for(i=0; i<8; i++)
  {
    Data <<= 1;
    I2C_SCL_HIGH();
    I2C_Delay(15);
    if(I2C_SDA_STATE())
      Data++;
    I2C_SCL_LOW();
    I2C_Delay(15);
  }
  return Data;
}

/* read an ack signal */
uint8_t I2C_WaitACK(void)
{  
  I2C_SDA_HIGH();
  I2C_Delay(15);
  I2C_SCL_HIGH();
  I2C_Delay(15);
  
  if(I2C_SDA_STATE())
  {
    //I2C_SCL_LOW();
    return I2C_NACK;
  }
  I2C_SCL_LOW();
  return I2C_ACK;
}

/* send a ack signal */
void I2C_SendACK(void)
{
  I2C_SDA_LOW();
  I2C_Delay(15);
  I2C_SCL_HIGH();
  I2C_Delay(15);
  I2C_SCL_LOW();
  I2C_Delay(15);
  I2C_SDA_HIGH();  
}

/* send a nack signal */
void I2C_SendNACK(void)
{
  I2C_SDA_HIGH();
  I2C_Delay(15);
  I2C_SCL_HIGH();
  I2C_Delay(15);
  I2C_SCL_LOW();
  I2C_Delay(15);
}

/* ads1110 config */
void ADS1110_Config(void)
{
  while(I2C_START());
  I2C_SendByte(ADS1110_WR_ADRRESS);
  while(!I2C_WaitACK());
//  I2C_SendNACK();
  
  I2C_SendByte(ADS1110_CONFIG_REG);
  while(!I2C_WaitACK());
//  I2C_SendNACK();
  I2C_STOP();  
}

/* read ads1110 value */
uint16_t ReadADS1110(void)
{
  uint8_t ADC_Byte_High;
  uint8_t ADC_Byte_Low;
  uint16_t ADC_Byte;

  while(I2C_START());
  I2C_SendByte(ADS1110_RD_ADRRESS);
  while(!I2C_WaitACK());
  
  ADC_Byte_High = I2C_ReadByte();
  I2C_SendACK();
  ADC_Byte_Low = I2C_ReadByte();
  I2C_SendACK();
  ADC_Byte = I2C_ReadByte();
  I2C_STOP();
  ADC_Byte = (ADC_Byte_High << 8) + ADC_Byte_Low;
  
  return ADC_Byte;
}

/* get adc value */
uint16_t Get_ADC_Value(void)
{
  uint16_t ADC_Value_Buf;
  uint32_t ADC_Value;
  uint8_t i;
  
  for(i=8; i>0; i--)
  {
    ADC_Value_Buf = ReadADS1110();
    ADC_Value += (uint32_t)ADC_Value_Buf;
  }
  
  return (uint16_t)(ADC_Value/8);
}

void ADS1110_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
    /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(I2C_GPIO_PORT, I2C_SCL_PIN|I2C_SDA_PIN, GPIO_PIN_SET);
  GPIO_InitStruct.Pin = I2C_SCL_PIN|I2C_SDA_PIN;
  
  /*Configure GPIO pin : PB8-PB15 */
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  
  I2C_SCL_HIGH();
  I2C_SDA_HIGH();
  
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  
  ADS1110_Config();
}
