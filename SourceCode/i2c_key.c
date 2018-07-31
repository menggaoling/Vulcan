/*******************************************************************************
  ����ƽ̨��STM32F103ZET6��Сϵͳ
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

// SCL�ߵ�ƽ�ڼ䣬SDA�����½���Ϊ��ʼ�ź�
static bool i2cStart()
{
    SDA_H;
    i2cDelay();
    SCL_H;
    i2cDelay();
    if (!SDA_READ)  // ���SDAΪ�͵�ƽ��������æ���˳�
        return FALSE;
    SDA_L;
    if (SDA_READ)  // ���SDAΪ�ߵ�ƽ��������æ���˳�
        return FALSE;
    SCL_L;
    return TRUE;
}

// SCL�ߵ�ƽ�ڼ䣬SDA����������Ϊֹͣ�ź�
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

// SCL�ߵ�ƽ�ڼ䣬SDA��ƽ�����豸���ͱ�ʾӦ��
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

// �������ݣ����ݴӸ�λ����λ����  
static void i2cSendByte(uint8_t byte)  
{
    uint8_t i = 8;    
    
    SCL_L;  // ʱ���ź�Ϊ�͵�ƽ�ڼ䣬���������ߵ�ƽ�仯
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
        SCL_L;  // ʱ���ź�Ϊ�͵�ƽ�ڼ䣬���������ߵ�ƽ�仯
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
 * ͨ��I2C����дһ�ֽ�����
 * @param[in] dev:�豸I2C��ַ
 * @param[in] reg:�Ĵ�����ַ
 * @param[in] data:Ҫд�������
 */
bool i2cWriteOneByte(uint8_t dev, uint8_t reg, uint8_t data)
{
    if (!i2cStart())        
        return FALSE;
    i2cDelay();
    i2cSendByte(dev & 0xFE);  // �ӻ���ַ�ɸ�7λ+��дλ����   
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
 * @param[in] dev:�豸I2C��ַ
 * @param[in] reg:�Ĵ�����ַ
 * @param[in] len:�ֽ��� 
 * @param[in] data:��д������� 
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
 * ��I2C�豸�ж�ȡ����
 * @param[in] dev:�豸I2C��ַ
 * @param[in] reg:�Ĵ�����ַ
 * @param[in] len:�����ֽ���
 * @param[out] data:����������
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
    i2cSendByte(dev | 0x01);  // ������ַ+������    
    i2cWaitAck();
    
    for( i = 0;i<20;i++)
      i2cDelay();
    
    while (len) 
    {
        *data = i2cReceiveByte();
        if (len == 1)
            i2cNoAck();  // ���һ���ֽڲ�Ӧ��
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

