#include "HRM8700.h"
#include "stm32f10x.h"
#include "UART.h"
#include <string.h>

#define TX_BUFFER_SIZE  20
#define RX_BUFFER_SIZE  30

#define TX_HEAD    0x0B
#define RX_HEAD    0x0A

#define MIN_PACKET_LEN    8
#define MAX_PACKET_LEN    24

//HRM MAX supports one 5khz ,5 ble ,5 ant+ devices,total is 11.
#define MAX_DEVICE_NUM    5  



void HRM_Tx_Enable(void);

typedef enum 
{
	START,
	LENGTH,
	CHECK_LOW,
	CHECK_HIGH,
	RESPONSEBIT,
	CLASS,
	SUB,
	DATA
} PARSE_STATE;


enum
{
  _5KHZ_NAME = (0x0C00 + 1),  //0x0C01
  BLE_NAME,                   
  ANT_NAME,
  
  BLE_HR_DATA = (0x0F00 + 1), //0x0F01
  ANT_HR_DATA,
  _5KHZ_HR_DATA,
  CONTACT_HR_DATA,
  
  CONTACT_RR_DATA = (0x0F00 + 0x14), //0x0F14
  
  ERROR_CODE = (0x0C00 + 5),  //0x0C05
  SYSTEM_STATUS,
  ACLK,
  SOFTWARE_VERSION,
  CONTACT_STATE,
  
  _5KHZ_RSSI = (0x0D00 + 1),  //0x0D01
  BLE_RSSI = (0x0D00 + 0x10), //0x0D10
  ANT_RSSI = (0x0D00 + 0x20), //0x0D20
  
  RECV_CMD_ALL
  
}HRM_Recv_CMD;//console receive command set.

typedef enum 
{
  SEND_5K_NAME,//CONNECTING
  SEND_BLE_ADDR,
  SEND_ANT_ID,
  
  START_WORKOUT_RSSI1,//WORKOUT
  START_WORKOUT_RSSI2,
  START_WORKOUT_RSSI3,
  STOP_WORKOUT,
  START_WORKOUT_RSSI_DATA,
  
  START_SCANNING,//SCANNING
  
  GET_STATE_STATUS,//GET_STATUS
  GET_SOFTWARE_VERSION,
  
  AUTO_CONNECT,//5KHZ COMMAND
  DISABLE_AUTO_CONNECT,
  
  CONTACT_OVERRIDE_ON,//CONTACT OPTION
  CONTACT_OVERRIDE_OFF,
  RR_INTERVAL_ENABLE,
  RR_INTERVAL_DISABLE,
  
  START_BLE_BROADCAST,//ENABLE BLE OTA
  STOP_BLE_BROADCAST,
  
  ENABLE_FEC_DISPLAY_PROFILE,//FEC DISPLAY OPTION
  DISABLE_FEC_DISPLAY_PROFILE,
  
  
  //BIKE FE-C PROTOCOL CMD
  SET_PAGE_16,//DETAILS FOR PAGE 
  SET_PAGE_18,
  SET_PAGE_21,
  SET_PAGE_80,
  SET_PAGE_81,
  
  SET_DEVICE_ID, //DEVEICE ID
  
  CMD_ALL
}HRM_CMD;

typedef enum 
{
  LENGTH_INDEX,
  RESPONSEBIT_INDEX,
  CLASS_INDEX,
  SUB_INDEX,
  DATA_INDEX,
  INDEX_ALL
}CMD_SET_INDEX;

const uint8_t HRM_CMD_SET[CMD_ALL][INDEX_ALL] =
{//length, responsebit, class, sub , data
  {15,     0,           0x1C,  0x01, 0x0 },//CONNECTING
  {15,     0,           0x1C,  0x02, 0x0 },
  {11,     0,           0x1C,  0x03, 0x0 },
  
  { 8,     0,           0x1C,  0x04, 0x01},//WORKOUT
  { 8,     0,           0x1C,  0x04, 0x02},
  { 8,     0,           0x1C,  0x04, 0x03},
  { 8,     0,           0x1C,  0x04, 0x04},
  { 8,     0,           0x1C,  0x04, 0x0 },
  
  { 8,     0,           0x1C,  0x05, 0x01},//SCANNING
  
  { 8,     1,           0x1C,  0x06, 0x01},//GET_STATUS
  { 8,     1,           0x1C,  0x06, 0x02},
  
  { 8,     1,           0x1C,  0x07, 0x01},//5KHZ COMMAND
  { 8,     1,           0x1C,  0x07, 0x02},
  
  { 8,     1,           0x1C,  0x08, 0x01},//CONTACT OPTION
  { 8,     1,           0x1C,  0x08, 0x02},
  { 8,     1,           0x1C,  0x08, 0x03},
  { 8,     1,           0x1C,  0x08, 0x04},
  
  { 8,     0,           0x1E,  0x01, 0x01},//ENABLE BLE OTA
  { 8,     0,           0x1E,  0x01, 0x02},
  
  { 8,     0,           0x1E,  0x02, 0x01},//FEC DISPLAY OPTION
  { 8,     0,           0x1E,  0x02, 0x02},
  
  //BIKE FE-C PROTOCOL CMD
  {15,     0,           0x12,  0x01, 0x00},//DETAILS FOR PAGE 16
  {15,     0,           0x13,  0x01, 0x00},//DETAILS FOR PAGE 18
  {15,     0,           0x14,  0x01, 0x00},//DETAILS FOR PAGE 21
  {15,     0,           0x15,  0x01, 0x00},//DETAILS FOR PAGE 80
  {15,     0,           0x16,  0x01, 0x00},//DETAILS FOR PAGE 81
  
  {15,     0,           0x17,  0x01, 0x00},//DEVEICE ID
};
typedef struct 
{
  uint8_t Head ;  
  uint8_t Length ;
  uint16_t CheckSum;   
  uint8_t ResponseBit ;//need ack
  uint8_t Class ;
  uint8_t Sub ;
  uint8_t Data[RX_BUFFER_SIZE - MIN_PACKET_LEN] ;
} HRM_Packet ;

typedef struct 
{
    uint8_t txBusy : 1;                  // indicates if this slot is used
    uint8_t responsebit : 1;             // indicates if this command expects a reply
    uint8_t reserved : 6;                // remaining bits for use
    uint8_t length;                      // defines the length of the packet bytes
    uint8_t txIndex;                     //  transmit index
    uint8_t retries;                     // retry count
} TX_INFO;

struct
{
  int8_t _5KHz_Name[8];
  
  struct
  {
    int8_t BLE_Name[11];
    uint8_t BLE_Addr[6]; //BLE MAC address 
    
  }BLE_ID[MAX_DEVICE_NUM];
    
  int8_t ANT_Name[MAX_DEVICE_NUM][4];    
    
  uint8_t ANT_Index;
  uint8_t BLE_Index;
  
} Device_ID;

struct
{
  uint8_t BLE_HR_Data;
  uint8_t ANT_HR_Data;
  uint8_t _5KHZ_HR_Data;
  uint8_t Contact_HR_Data;
  uint8_t Contact_RR_Data;  
} HR_Data;

struct
{
  union
  {
    struct
    {
      uint8_t Connection_Lost : 1;
      uint8_t Corrupted_HR : 1;
      uint8_t Relearn : 1;
      uint8_t Code_Noisy : 1;
      uint8_t Code_Noisy_Off : 1;
      uint8_t Code_Over_Limit : 1;
      uint8_t Cross_Talk : 1;
    }bit;
    uint8_t All;
  }_5KHZ;
  union
  {
    struct
    {
      uint8_t Connection_Lost : 1;
      uint8_t Connection_Failed : 1;
      uint8_t No_device_Found : 1;
    }bit;
    uint8_t All;
  }Common;
  struct
  {
    uint8_t Garbled : 1;//unrecognized
  }Packet;
  union
  {
    struct
    {
      uint8_t Connection_Lost : 1;
      uint8_t Corrupted_HR : 1;
      uint8_t Relearn : 1;
      uint8_t Code_Noisy : 1;
      uint8_t Code_Noisy_Off : 1;
      uint8_t Code_Over_Limit : 1;
    }bit;
    uint8_t All;
  }Contact;
}HRM_Error;

union
{
  struct
  {
    uint16_t non_used : 1;
    uint16_t Disconnected : 1;
    uint16_t Searching_Device : 1;
    uint16_t Device_Not_Found : 1;
    uint16_t Device_Found : 1;
    uint16_t Idle_Mode : 1;
    uint16_t Connected : 1;
    uint16_t Scanning : 1;
    uint16_t Stop_Scanning : 1;
    uint16_t Workout_Started : 1;
    uint16_t Workout_Stopped : 1;
    uint16_t _5KHZ_WL_Active : 1;
    uint16_t Contact_Active : 1;
    uint16_t FEC_Status : 1;
  }bit;
  uint16_t All;  
}System_Status;

union
{
  struct
  {
    uint8_t Contact_Active : 1;
    uint8_t Contact_Inactive : 1;
    uint8_t Contact_HandOn : 1;
    uint8_t Contact_HandOff : 1;
  }bit;
  uint8_t All;
}Contact_State;

struct
{
  uint8_t _5KHZ_RSSI;
  uint8_t BLE_RSSI;
  uint8_t ANT_RSSI;
}RSSI_Value;

uint8_t HRM_Version[11];
uint8_t HRM_TxBuffer[TX_BUFFER_SIZE];
uint8_t HRM_RxBuffer[RX_BUFFER_SIZE];
static TX_INFO Tx_Status;


uint16_t Get_Fletcher_Checksum(const uint8_t *data, uint16_t len);
uint8_t HRM_Send_Command(HRM_CMD cmdIndex,uint8_t *data,uint8_t length);
static uint32_t HR_Clr_Timer;

void HRM_Init(void)
{
  memset(HRM_TxBuffer,0,sizeof(HRM_TxBuffer));
  memset(HRM_RxBuffer,0,sizeof(HRM_RxBuffer));
  memset(&Tx_Status,0,sizeof(TX_INFO));
  memset(&HR_Data,0,sizeof(HR_Data));
  HR_Clr_Timer = 0;
}
void HRM_Command_Parse(HRM_Packet * p_DataPacket)
{
  
  switch( (p_DataPacket->Class << 8) + p_DataPacket->Sub)
  {
  case _5KHZ_NAME :
    {
      memcpy(Device_ID._5KHz_Name,p_DataPacket->Data,sizeof(Device_ID._5KHz_Name));
    }
    break;
  case BLE_NAME :
    {
      if( Device_ID.BLE_Index < MAX_DEVICE_NUM)
        memcpy(&Device_ID.BLE_ID[Device_ID.BLE_Index],p_DataPacket->Data,sizeof(&Device_ID.BLE_ID[Device_ID.BLE_Index++]));
    }
    break;  
  case ANT_NAME :
    {
      if( Device_ID.ANT_Index < MAX_DEVICE_NUM)
        memcpy(&Device_ID.ANT_Name[Device_ID.ANT_Index],p_DataPacket->Data,sizeof(&Device_ID.ANT_Name[Device_ID.ANT_Index++]));
    }
    break;
  case BLE_HR_DATA :
    HR_Data.BLE_HR_Data = p_DataPacket->Data[0];
    HR_Clr_Timer = HAL_GetTick();
    break; 
  case ANT_HR_DATA :
    HR_Data.ANT_HR_Data = p_DataPacket->Data[0];
    HR_Clr_Timer = HAL_GetTick();
    break;
  case _5KHZ_HR_DATA :
    HR_Data._5KHZ_HR_Data = p_DataPacket->Data[0];
    HR_Clr_Timer = HAL_GetTick();
    break;  
  case CONTACT_HR_DATA :
    HR_Data.Contact_HR_Data = p_DataPacket->Data[0];
    HR_Clr_Timer = HAL_GetTick();
    break;
  case CONTACT_RR_DATA :
    HR_Data.Contact_RR_Data = p_DataPacket->Data[0];
    break; 
  case ERROR_CODE :
    {
      if(p_DataPacket->Data[0] < 0x30)
      {
        HRM_Error._5KHZ.All = 1 << p_DataPacket->Data[0];
      }
      else if(p_DataPacket->Data[0] < 0x40)
      {
        HRM_Error.Common.All = 1 << (p_DataPacket->Data[0]&0x0F);
      }
      else if(p_DataPacket->Data[0] < 0x50)
      {
        HRM_Error.Packet.Garbled = 1;
      }
      else if(p_DataPacket->Data[0] < 0x60)
        HRM_Error.Contact.All = 1 << (p_DataPacket->Data[0]&0x0F);
    }
    break;
  case SYSTEM_STATUS :
    {
      System_Status.All = 1 << p_DataPacket->Data[0];
    }
    break;  
  case ACLK :
    asm("nop");
    break;
  case SOFTWARE_VERSION :
    {
      memcpy(HRM_Version,p_DataPacket->Data,sizeof(HRM_Version));
    }
    break; 
  case CONTACT_STATE :
    {
      Contact_State.All = 1 << p_DataPacket->Data[0];
    }
    break;
  case _5KHZ_RSSI :
    RSSI_Value._5KHZ_RSSI = p_DataPacket->Data[0];
    break;  
  case BLE_RSSI :
    RSSI_Value.BLE_RSSI = p_DataPacket->Data[0];
    break;
  case ANT_RSSI :
    RSSI_Value.ANT_RSSI = p_DataPacket->Data[0];
    break; 
  default:break;
  }

}
typedef enum 
{
  INITIAL,
  START_WORKOUT, 
  SCANNING,
  CONNECT,
  WORKOUT_STOP,
  IDLE = 30
  
}HRM_STATE;

void HRM_State_Machine(void)
{
  static uint8_t current_state,next_state = INITIAL;
  current_state = next_state;
  
  if(HR_Data.ANT_HR_Data || HR_Data.BLE_HR_Data || HR_Data.Contact_HR_Data\
    || HR_Data.Contact_RR_Data || HR_Data._5KHZ_HR_Data)
  {
    if(HAL_GetTick() - HR_Clr_Timer >= 6000)
    {
      memset(&HR_Data,0,sizeof(HR_Data));
    }
  }
  switch(current_state)
  {
  case 1 :
    {
      char _5KHzName[9];
      sprintf(_5KHzName,"%s","5kHz HRM");//==>
      if(HRM_Send_Command(SEND_5K_NAME,(uint8_t *)_5KHzName,8))
      {     
        next_state = IDLE;
      }
    }
    break;
  case 2 :
    if(HRM_Send_Command(SEND_BLE_ADDR,NULL,1))
    {
      next_state = IDLE;
    }
    break;
  case 3 :
    if(HRM_Send_Command(SEND_ANT_ID,NULL,1))
    {
      next_state = IDLE;
    }
    break;
  case 4 :
    if(HRM_Send_Command(START_WORKOUT_RSSI1,NULL,1))
    {
      next_state = IDLE;
    }
    break;
  case 5 :
    if(HRM_Send_Command(START_WORKOUT_RSSI2,NULL,1))
    {
      next_state = IDLE;
    }
    break;
  case 6 :
    if(HRM_Send_Command(START_WORKOUT_RSSI3,NULL,1))
    {
      next_state = IDLE;
    }
    break;
  case 7 :
    if(HRM_Send_Command(STOP_WORKOUT,NULL,1))
    {
      next_state = IDLE;
    }
    break;
  case 8 :
    if(HRM_Send_Command(START_WORKOUT_RSSI_DATA,NULL,1))
    {
      next_state = IDLE;
    }
    break;
  case 9 :
    if(HRM_Send_Command(START_SCANNING,NULL,1))
    {
      next_state = IDLE;
    }
    break;
  case 10 :
    if(HRM_Send_Command(GET_STATE_STATUS,NULL,1))
    {
      next_state = IDLE;
    }
    break;
  case INITIAL :
    if(HRM_Send_Command(GET_SOFTWARE_VERSION,NULL,1))
    {
      next_state = IDLE;
    }
    break;
  case 12 :
    if(HRM_Send_Command(AUTO_CONNECT,NULL,1))
    {
      next_state = IDLE;
    }
    break;
  case 13 :
    if(HRM_Send_Command(DISABLE_AUTO_CONNECT,NULL,1))
    {
      next_state = IDLE;
    }
    break;
  case 14 :
    if(HRM_Send_Command(CONTACT_OVERRIDE_ON,NULL,1))
    {
      next_state = IDLE;
    }
    break;
  case 15 :
    if(HRM_Send_Command(CONTACT_OVERRIDE_OFF,NULL,1))
    {
      next_state = IDLE;
    }
    break;
  case 16 :
    if(HRM_Send_Command(RR_INTERVAL_ENABLE,NULL,1))
    {
      next_state = IDLE;
    }
    break;
  case 17 :
    if(HRM_Send_Command(RR_INTERVAL_DISABLE,NULL,1))
    {
      next_state = IDLE;
    }
    break;
  case 18 :
    if(HRM_Send_Command(START_BLE_BROADCAST,NULL,1))
    {
      next_state = IDLE;
    }
    break;
  case 19 :
    if(HRM_Send_Command(STOP_BLE_BROADCAST,NULL,1))
    {
      next_state = IDLE;
    }
    break;
  case 20 :
    if(HRM_Send_Command(ENABLE_FEC_DISPLAY_PROFILE,NULL,1))
    {
      next_state = IDLE;
    }
    break;
  case 21 :
    if(HRM_Send_Command(DISABLE_FEC_DISPLAY_PROFILE,NULL,1))
    {
      next_state = IDLE;
    }
    break;
  case 22 :
    if(HRM_Send_Command(SET_PAGE_16,NULL,1))
    {
      next_state = IDLE;
    }
    break;
  case 23 :
    if(HRM_Send_Command(SET_PAGE_18,NULL,1))
    {
      next_state = IDLE;
    }
    break;
  case 24 :
    if(HRM_Send_Command(SET_PAGE_21,NULL,1))
    {
      next_state = IDLE;
    }
    break;
  case 25 :
    if(HRM_Send_Command(SET_PAGE_80,NULL,1))
    {
      next_state = IDLE;
    }
    break;
  case 26 :
    if(HRM_Send_Command(SET_PAGE_81,NULL,1))
    {
      next_state = IDLE;
    }
    break;
  case 27 :
    if(HRM_Send_Command(SET_DEVICE_ID,NULL,1))
    {
      next_state = IDLE;
    }
    break;
  case IDLE:
    asm("nop");
    break;
  default:break;
  }
//  {
//  case INITIAL :
//    if(HRM_Send_Command(AUTO_CONNECT,NULL,1))
//    {
//      //    next_state = START_WORKOUT;
//    }
//    HRM_Send_Command(GET_SOFTWARE_VERSION,NULL,1);
//
//    break; 
//  case START_WORKOUT :
//    HRM_Send_Command(START_WORKOUT_RSSI1,NULL,1);
////    next_state = SCANNING;
//    break;     
//  case SCANNING :
//    HRM_Send_Command(START_WORKOUT_RSSI1,NULL,1);
////    next_state = CONNECT;
//    break;  
//  case IDLE:
//    HRM_Send_Command(GET_STATE_STATUS,NULL,1);
//    asm("nop");
//    break;
//  case WORKOUT_STOP :
//    HRM_Send_Command(STOP_WORKOUT,NULL,1);
////    next_state = CONNECT;    
//  case CONNECT :
//    HRM_Send_Command(SEND_5K_NAME,NULL,0);
//    HRM_Send_Command(SEND_BLE_ADDR,NULL,0);
//    HRM_Send_Command(SEND_ANT_ID,NULL,0);
////    next_state = CONNECT;       
//  default:break;
//  }
  
}

uint8_t HRM_Send_Command(HRM_CMD cmdIndex,uint8_t *data,uint8_t length)
{
  static uint32_t Time_Stamp = 0;
  if(HAL_GetTick() - Time_Stamp > 500)
  {
    HRM_Packet * send_data = (HRM_Packet *)HRM_TxBuffer;
    send_data->Head = TX_HEAD;
    send_data->Length = HRM_CMD_SET[cmdIndex][LENGTH_INDEX];  
    send_data->ResponseBit = HRM_CMD_SET[cmdIndex][RESPONSEBIT_INDEX];
    send_data->Class = HRM_CMD_SET[cmdIndex][CLASS_INDEX];
    send_data->Sub = HRM_CMD_SET[cmdIndex][SUB_INDEX];
    
    if(length > 1 && length < (TX_BUFFER_SIZE - MIN_PACKET_LEN))//prevent overflow
    {
      memcpy(send_data->Data, data, length);
    }
    else if(length == 1 )
    {
      send_data->Data[0] = HRM_CMD_SET[cmdIndex][DATA_INDEX];
    }
    send_data->CheckSum = Get_Fletcher_Checksum(&send_data->ResponseBit,(send_data->Length - RESPONSEBIT) );//check from responsebit to packet end
    
    //update tx info
    Tx_Status.length = send_data->Length;
    Tx_Status.txIndex = 0;
    Tx_Status.txBusy = 1;
    Tx_Status.responsebit = send_data->ResponseBit;
    
    //enable tx
    HRM_Tx_Enable();
    
    Time_Stamp = HAL_GetTick();
    return SUCCESS;
  }
  else
    return ERROR; 
  
}


uint8_t HRM_Recv_Int(uint8_t byte)
{
  static uint8_t index = 0;
  static uint8_t parse_state = START;
  static HRM_Packet *p_Packets = (HRM_Packet *)HRM_RxBuffer;
  uint8_t Rx_Flag = 0;
  HRM_RxBuffer[index++] = byte;
  
  switch(parse_state)
    {
    case START:
        if(byte == RX_HEAD)
            ++parse_state;
        else
            index = 0;
        break;
    case LENGTH:
        if(byte >= MIN_PACKET_LEN &&  byte <= MAX_PACKET_LEN)//receive range 8-24 bytes
        {
            ++parse_state;
        }
        else
        {
            index = 0;
            parse_state = START;
        }
        break;
    case CHECK_LOW:
      ++parse_state;
      break;
    case CHECK_HIGH:
      ++parse_state;
      break;      
    case RESPONSEBIT:
      ++parse_state;
      break;  
    case CLASS:
      ++parse_state;
      break;  
    case SUB:
      ++parse_state;
      break;  
    case DATA:
        if(index >=  p_Packets->Length)
        {
          if(Get_Fletcher_Checksum(&p_Packets->ResponseBit ,p_Packets->Length - RESPONSEBIT) == p_Packets->CheckSum)
          {
            HRM_Command_Parse(p_Packets);
          }          
          parse_state = START;
          index = 0;
          Rx_Flag = 1;
        }        
        break;     
    default:break;    
    }
  return Rx_Flag;
}


uint16_t Fletcher16(const uint8_t *data, uint16_t len)
{
	uint16_t c0, c1;
	 uint16_t i;

	for (c0 = c1 = 0; len >= 5802; len -= 5802) {
		for (i = 0; i < 5802; ++i) {
			c0 = c0 + *data++;
			c1 = c1 + c0;
		}
		c0 = c0 % 255;
		c1 = c1 % 255;
	}
	for (i = 0; i < len; ++i) {
		c0 = c0 + *data++;
		c1 = c1 + c0;
	}
	c0 = c0 % 255;
	c1 = c1 % 255;
	return (c1 << 8 | c0);
}

uint16_t Get_Fletcher_Checksum(const uint8_t *data, uint16_t len)
{
	uint16_t nFletcherSum;
	uint8_t nSumLowByte, nSumHighByte;
	nFletcherSum = Fletcher16(data, len);

	nSumLowByte = (255 - (((nFletcherSum & 0xFF) + (nFletcherSum >> 8)) % 255)) & 0xFF;
	nSumHighByte = (255 - ((nSumLowByte + (nFletcherSum & 0xFF)) % 255)) & 0xFF;

	return ((nSumHighByte << 8) | (nSumLowByte & 0xFF));
}


//Hardware 
void HRM_Tx_Enable(void)
{
  __HAL_UART_ENABLE_IT(&huart6,UART_IT_TXE);//==>USART6中断打开
}

void HR_UartTxRx_Information(void)
{
    
    if(__HAL_UART_GET_IT_SOURCE(&huart6, UART_IT_TXE) != RESET && __HAL_UART_GET_FLAG(&huart6,UART_FLAG_TXE) != RESET)
    {//==>判断是否发送中断
        /* Clear the USART6 transmit interrupt */
        __HAL_UART_CLEAR_FLAG(&huart6, UART_FLAG_TXE);
        if(Tx_Status.txIndex < Tx_Status.length)
        {
          /* Write one byte to the transmit data register */
          huart6.Instance->DR = HRM_TxBuffer[Tx_Status.txIndex++] & 0xFF;
        }
        else
        {
          /* Disable the USART4 Transmit interrupt */
          __HAL_UART_DISABLE_IT(&huart6,UART_IT_TXE);
          __HAL_UART_ENABLE_IT(&huart6,UART_IT_RXNE);
          
          Tx_Status.length = 0;
          Tx_Status.txIndex = 0;
          Tx_Status.txBusy = 0;          
        }
    }
    if(__HAL_UART_GET_IT_SOURCE(&huart6, UART_IT_RXNE) != RESET && __HAL_UART_GET_FLAG(&huart6,UART_FLAG_RXNE) != RESET)
    {//==>判断是否接收中断
        /* Clear the USART1 Receive interrupt */
        __HAL_UART_CLEAR_FLAG(&huart6, UART_FLAG_RXNE);
        /* Read one byte from the receive data register */        
        if(HRM_Recv_Int((uint8_t)(huart6.Instance->DR & (uint16_t)0x00FF)))
        {      
          if(Tx_Status.responsebit)
          {            
            Tx_Status.responsebit = 0;
          }
//          else
//            __HAL_UART_DISABLE_IT(&huart6,UART_IT_RXNE);
        }    
    }
}