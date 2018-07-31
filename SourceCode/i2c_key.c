/*******************************************************************************
  测试平台：STM32F103ZET6最小系统
*******************************************************************************/
#include "i2c_key.h" 


#define SCL_H                 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_SET)
#define SCL_L                 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_RESET)
#define SDA_H                 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_SET)
#define SDA_L                 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_RESET)
#define SDA_READ              HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_9)


static void i2cDelay()
{
    volatile int i = 10;
    while (i)
    i--;
}

// SCL高电平期间，SDA出现下降沿为起始信号
static bool i2cStart()
{
    SDA_H;
    i2cDelay();
    SCL_H;
    i2cDelay();
    if (!SDA_READ)  // 如果SDA为低电平，则总线忙，退出
        return FALSE;
    SDA_L;
    if (SDA_READ)  // 如果SDA为高电平，则总线忙，退出
        return FALSE;
    SCL_L;
    return TRUE;
}

// SCL高电平期间，SDA出现上升沿为停止信号
static void i2cStop(void)
{ 
    SCL_L;
    SDA_L;
    i2cDelay();  // STOP:when CLK is high DATA from low to high 
    SCL_H;
    i2cDelay();
    SDA_H;  
    i2cDelay();
}

static void i2cAck(void)
{
    SCL_L;
    i2cDelay();
    SDA_L;
    i2cDelay();
    SCL_H;
    i2cDelay();
    SCL_L;
}

static void i2cNoAck(void)
{
    SCL_L;
    i2cDelay();
    SDA_H;
    i2cDelay();
    SCL_H;
    i2cDelay();
    SCL_L;
}

// SCL高电平期间，SDA电平被从设备拉低表示应答
static bool i2cWaitAck(void)
{
    uint8_t errTimes = 0;

    SDA_H;
    i2cDelay();
    SCL_H;
    i2cDelay();
    while (SDA_READ) 
    {
        if (errTimes++ > 20) 
        {
            SCL_L;
            return FALSE;
        }           
        i2cDelay();
    }
    SCL_L;
    return TRUE;
}

// 发送数据，数据从高位到低位传输  
static void i2cSendByte(uint8_t byte)  
{
    uint8_t i = 8;    
    
    SCL_L;  // 时钟信号为低电平期间，允许数据线电平变化
    while (i--) 
    {      
        if (byte & 0x80)
            SDA_H;
        else
            SDA_L; 
        byte <<= 1; 
        i2cDelay();
        SCL_H;
        i2cDelay();
        SCL_L;  // 时钟信号为低电平期间，允许数据线电平变化
        i2cDelay();
    }
}

static uint8_t i2cReceiveByte(void)  
{
    uint8_t i = 8;
    uint8_t byte = 0;

//    SDA_H;
    while (i--) 
    {
        byte <<= 1;
        SCL_H;
        i2cDelay();
        if (SDA_READ) 
        {
            byte |= 0x01;
        }
        
        SCL_L;
        i2cDelay();
    }
    SCL_L;
    return byte; 
}


void i2cInit(void)
{    
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_10,GPIO_PIN_RESET);
    for(char i = 0;i< 30;i++);
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_10,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_SET);
    
}

/**
 * 通过I2C总线写一字节数据
 * @param[in] dev:设备I2C地址
 * @param[in] reg:寄存器地址
 * @param[in] data:要写入的数据
 */
bool i2cWriteOneByte(uint8_t dev, uint8_t reg, uint8_t data)
{
    if (!i2cStart())        
        return FALSE;
    i2cDelay();
    i2cSendByte(dev & 0xFE);  // 从机地址由高7位+读写位构成   
    if (!i2cWaitAck()) 
    {     
        i2cStop();
        return FALSE;
    }
    i2cSendByte(reg);       
    i2cWaitAck();
    i2cSendByte(data);     
    i2cWaitAck();
    return TRUE;
}

/**
 *  
 * @param[in] dev:设备I2C地址
 * @param[in] reg:寄存器地址
 * @param[in] len:字节数 
 * @param[in] data:待写入的数据 
 */
bool i2cWriteBytes(uint8_t dev, uint8_t reg, uint8_t len, uint8_t *data)
{
    uint8_t i;

    if (!i2cStart())        
        return FALSE;
    i2cDelay();
    i2cSendByte(dev & 0xFE);          
    if (!i2cWaitAck()) 
    {     
        i2cStop();
        return FALSE;
    }
    for( i = 0;i<10;i++)
      i2cDelay();
    i2cSendByte(reg);   
    i2cWaitAck();
    for (i = 0; i < len; i++) 
    {
        i2cSendByte(data[i]);
        if (!i2cWaitAck()) 
        {
            i2cStop();
            return FALSE;
        }
    }
    i2cStop();
    
    for( i = 0;i<20;i++)
      i2cDelay();
    return TRUE;
}


/**
 * 从I2C设备中读取数据
 * @param[in] dev:设备I2C地址
 * @param[in] reg:寄存器地址
 * @param[in] len:数据字节数
 * @param[out] data:读出的数据
 */
bool i2cReadBytes(uint8_t dev, uint8_t reg, uint8_t len, uint8_t *data)
{
  uint8_t i;
//    if (!i2cStart())        
//        return FALSE;
//    i2cSendByte(dev << 1);      
//    if (!i2cWaitAck()) 
//    {     
//        i2cStop();
//        return FALSE;
//    }

//    i2cSendByte(reg);     
//    i2cWaitAck();
    i2cStart();   
    for( i = 0;i<15;i++)
      i2cDelay();
    i2cSendByte(dev | 0x01);  // 器件地址+读命令    
    i2cWaitAck();
    
    for( i = 0;i<20;i++)
      i2cDelay();
    
    while (len) 
    {
        *data = i2cReceiveByte();
        if (len == 1)
            i2cNoAck();  // 最后一个字节不应答
        else
            i2cAck();
        data++;
        len--;
    }
    i2cStop();
    for( i = 0;i<25;i++)
      i2cDelay();
    return TRUE;
}

