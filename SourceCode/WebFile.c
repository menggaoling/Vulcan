//##############################################################################
//
//
//##############################################################################
/* Includes ------------------------------------------------------------------*/
#include  "HT1381.h"
#include  "General.h"
#include  "WebFile.h"
#include  <stdio.h>
#include  "UsbFunction.h"
#include  "WiFi.h"
#include  "EEPRom.h"
#include  "Digital.h"
//#include  "Treadmill.h"

#define   HistoryMaxSize      1200
unsigned short DistanceHistory[HistoryMaxSize] ;
unsigned short SpeedHistory[HistoryMaxSize] ;
unsigned char InclineHistory[HistoryMaxSize] ;
unsigned char HeartRateHistory[HistoryMaxSize] ;
WebFileDataType WebFile ;

//#ifdef WiFi_Module
extern WiFiModule WiFiModuleData ;
//#endif

//
void WebFile_Initial(void)
{
  //
  WebFile.mac=&WiFiModuleData.MAC[0] ;
  WebFile.ip=&WiFiModuleData.IP[0] ;
  WebFile.mfr = 0x10  ;
  WebFile.model = 99 ;
  WebFile.distance_history = &DistanceHistory[0] ;
  WebFile.speed_history = &SpeedHistory[0] ;
  WebFile.incline_history = &InclineHistory[0] ;
  WebFile.HRT_history = &HeartRateHistory[0] ;
  WebFile.duration = 0 ;
  WebFile.distance = 0 ;
  WebFile.vertical = 0 ;  //
  WebFile.Calories = 0 ;  //Kcal/min
  WebFile.avg_pace = 0 ;
  WebFile.avg_speed = 0 ;
  WebFile.history_count = 0 ;
  WebFile.distance_unit = 0 ;
  WebFile.vertical_unit = 0 ;
  WebFile.avg_HRT = 0;
  WebFile.max_HRT = 0;
  WebFile.speed_unit = 0;
  WebFile.program = 0;
  WebFile.increment = 5;
  //
  WebFile.Error = 0 ;
  //WebFile.ServiceCode = 0 ;  
  WebFile.motor_temp = 0;
  WebFile.motor_temp_unit = 0 ;
  WebFile.bus_current = 0 ;
  WebFile.bus_current_unit = 0;
  WebFile.bus_voltage = 0 ;
  WebFile.bus_voltage_unit= 0;
  //
  return ;
}



//
short WebFile_SaveHistory( unsigned short D, unsigned short S, unsigned char I, unsigned char HRT )
{
  //
  if( WebFile.history_count < HistoryMaxSize )
      {
      *(WebFile.distance_history+WebFile.history_count) = D ;
      *(WebFile.speed_history+WebFile.history_count) = S ;
      *(WebFile.incline_history+WebFile.history_count) = I ;
      *(WebFile.HRT_history+WebFile.history_count) = HRT ;
      // shift next point
      WebFile.history_count +=1 ;  
      //  
      return WebFile.history_count ;
      }
  //
  return (0xFFFF) ;   // Error 
}



//
void WebFile_CreateStream(unsigned char StreamNo,unsigned char *Data)
{
  unsigned short i ;
  long Len ;
  
  switch(StreamNo)
      {
      case  0 :
              /*  <workout user_id='0' mac='00:13:02:1b:3e:84' mfr='15' model='4'
                  date='2010-10-14 12:45:31' duration='150' distance='8'
                  distance_unit='3' vertical='4' vertical_unit='5'
                  calories='7' avg_pace='1875' avg_heart_rate='88'
                  max_heart_rate='97' avg_speed='19' speed_unit='17'
                  program_id='1' > \t<summary>\n\r\t\t*/
              //
              ReadHT1381(&WebFile.Clock) ;
              WebFile.Clock.Year = __CharBCD2DEC(WebFile.Clock.Year) ;
              WebFile.Clock.Month = __CharBCD2DEC(WebFile.Clock.Month) ;
              WebFile.Clock.Date = __CharBCD2DEC(WebFile.Clock.Date) ;
              WebFile.Clock.Hours = __CharBCD2DEC(WebFile.Clock.Hours) ;
              WebFile.Clock.Minutes = __CharBCD2DEC(WebFile.Clock.Minutes) ;
              WebFile.Clock.Seconds = __CharBCD2DEC(WebFile.Clock.Seconds) ;
              //
                            
              sprintf((char*)Data,"<workout user_id='0' mac='%02x:%02x:%02x:%02x:%02x:%02x' mfr='%d' modle='%d' date='20%02d-%02d-%02d %02d:%02d:%02d' duration='%d' distance='%d' distance_unit='%d' vertical='%d' vertical_unit='%d' calories='%d' avg_pace='%02d:%02d' avg_heart_rate='%d' max_heart_rate='%d' avg_speed='%d' speed_unit='%d' program_id='%2d' >\n\r",\
              *(WebFile.mac),*(WebFile.mac+1),*(WebFile.mac+2),*(WebFile.mac+3),*(WebFile.mac+4),*(WebFile.mac+5),WebFile.mfr,WebFile.model,\
              WebFile.Clock.Year,WebFile.Clock.Month,WebFile.Clock.Date,WebFile.Clock.Hours,WebFile.Clock.Minutes,WebFile.Clock.Seconds,\
              WebFile.duration,WebFile.distance,WebFile.distance_unit,WebFile.vertical,WebFile.vertical_unit,WebFile.Calories,\
              (WebFile.avg_pace/0x100),(WebFile.avg_pace%0x100),WebFile.avg_HRT,WebFile.max_HRT,WebFile.avg_speed,WebFile.speed_unit,WebFile.program );
              break ;
      case  1 :
              /*  <distance_history increment='5'>0,0,0,0,0,0,0,1,1,1,2,2,2,3,3,3,3,4,4,4,5,5,5,6,6,6,7,7,7,7,8
                  </distance_history> */
              sprintf((char*)Data,"\t<distance_history increment='%d'>",WebFile.increment) ;
              for( i = 0 ; i < WebFile.history_count ; i++ )
                  {
                  Len = __strlen((char const*)Data) ;
                  if( i == ( WebFile.history_count -1 ))
                      sprintf((char*)(Data+Len),"%d",*(WebFile.distance_history+i)) ;
                  else
                      sprintf((char*)(Data+Len),"%d,",*(WebFile.distance_history+i)) ;
                  }
              Len = __strlen((char const*)Data) ;
              sprintf((char*)(Data+Len),"\n\r\t</distance_history>\n\r") ;
              break ;
      case  2 :
              /*  <speed_history increment='5'>10,10,10,10,10,10,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22
                  </speed_history> */
              sprintf((char*)Data,"\t<speed_history increment='%d'>",WebFile.increment) ;
              for( i = 0 ; i < WebFile.history_count ; i++ )
                  {
                  Len = __strlen((char const*)Data) ;
                  if( i == ( WebFile.history_count -1 ))
                      sprintf((char*)(Data+Len),"%d",*(WebFile.speed_history+i)) ;
                  else
                      sprintf((char*)(Data+Len),"%d,",*(WebFile.speed_history+i)) ;
                  }
              Len = __strlen((char const*)Data) ;
              sprintf((char*)(Data+Len),"\n\r\t</speed_history>\n\r") ;
              break ;
      case  3 :
              /*  <incline_history increment='5'>6,6,6,6,6,6,6,10,10,10,
                  10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,
                  10,10,10,10
                  </incline_history> */
              sprintf((char*)Data,"\t<incline_history increment='%d'>",WebFile.increment) ;
              for( i = 0 ; i < WebFile.history_count ; i++ )
                  {
                  Len = __strlen((char const*)Data) ;
                  if( i == ( WebFile.history_count -1 ))
                      sprintf((char*)(Data+Len),"%d",*(WebFile.incline_history+i)) ;
                  else   
                      sprintf((char*)(Data+Len),"%d,",*(WebFile.incline_history+i)) ;
                  }
              Len = __strlen((char const*)Data) ;
              sprintf((char*)(Data+Len),"\n\r\t</incline_history>\n\r") ;
              break ;
      case  4 :
              /*  <heart_rate_history increment='5'>0,0,65,79,88,93,95,95,
                  97,97,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                  </heart_rate_history> */
              sprintf((char*)Data,"\t<heart_rate_history increment='%d'>",WebFile.increment) ;
              for( i = 0 ; i < WebFile.history_count ; i++ )
                  {
                  Len = __strlen((char const*)Data) ;
                  if( i == ( WebFile.history_count -1 ))
                      sprintf(((char*)Data+Len),"%d",*(WebFile.HRT_history+i)) ;
                  else   
                      sprintf(((char*)Data+Len),"%d,",*(WebFile.HRT_history+i)) ;
                  }
              Len = __strlen((char const*)Data) ;
              sprintf((char*)(Data+Len),"\n\r\t</heart_rate_history>\n\r") ;
              break ; 
      case  5 :
              /*  </workout> */
              sprintf((char*)Data,"</workout>\n\r") ;
              break ;
      case  6 :
              ///*
              sprintf((char*)Data,"<asset_management\r\n \
              mac=\"%02X:%02X:%02X:%02X:%02X:%02X\"\r\n \
              lan_ip=\"%d.%d.%d.%d\"\r\n \
              mfr=\"%d\"\r\n \
              model=\"%d\"\r\n \
              date=\"20%02d-%02d-%02d %02d:%02d:%02d\"\r\n \
              api_key=\"%s\"\r\n \
              error=\"%04x\"\r\n \
              service_code=\"%s\"\r\n \
              ver=\"%s\"\r\n \
              sernum=\"%s\"\r\n \
              frame_sernum=\"%s\"\r\n \
              utilization=\"%d\"\r\n \
              odometer=\"%d\"\r\n \
              odometer_unit=\"%d\"\r\n \
              motor_temp=\"%d\"\r\n \
              motor_temp_unit=\"%d\"\r\n \
              bus_current=\"%d\"\r\n \
              bus_current_unit=\"%d\"\r\n \
              bus_voltage=\"%d\"\r\n \
              bus_voltage_unit=\"%d\"\r\n \
              partner_location_id=\"%d\"\r\n \
              model_id=\"%d\" />\r\n", \
              *(WebFile.mac),*(WebFile.mac+1),*(WebFile.mac+2),*(WebFile.mac+3),*(WebFile.mac+4),*(WebFile.mac+5),\
              *(WebFile.ip+2),*(WebFile.ip+3),*(WebFile.ip+4),*(WebFile.ip+5),WebFile.mfr,WebFile.model,\
              WebFile.Clock.Year,WebFile.Clock.Month,WebFile.Clock.Date,WebFile.Clock.Hours,WebFile.Clock.Minutes,WebFile.Clock.Seconds,"9%2BJKLSfizDw%3D",\
              WebFile.Error,WebFile.ServiceCode,WebFile.Version,WebFile.ConsoleSerialNumber,WebFile.FrameSerialNumber,\
              WebFile.duration,WebFile.distance,WebFile.distance_unit,\
              WebFile.motor_temp,WebFile.motor_temp_unit,WebFile.bus_current,WebFile.bus_current_unit,WebFile.bus_voltage,WebFile.bus_voltage_unit,(unsigned short)EEPROM_ReadInformation(EE_CLUB_ID),WebFile.module_id) ;
              //*/
              break ;
      case  10: // Top
              sprintf((char*)Data,"<asset_management\r\n") ;
              break ;
      case  11: // MAC
              sprintf((char*)Data,"mac=\"%02X:%02X:%02X:%02X:%02X:%02X\"\r\n",*(WebFile.mac),*(WebFile.mac+1),*(WebFile.mac+2),*(WebFile.mac+3),*(WebFile.mac+4),*(WebFile.mac+5)) ;
              break ;
      case  12:
              sprintf((char*)Data,"lan_ip=\"%d.%d.%d.%d\"\r\n",*(WebFile.ip+2),*(WebFile.ip+3),*(WebFile.ip+4),*(WebFile.ip+5)) ;
              break ;        
      case  13:
              sprintf((char*)Data,"mfr=\"%d\"\r\n",WebFile.mfr) ;
              break ; 
      case  14:        
              sprintf((char*)Data,"model=\"%d\"\r\n",WebFile.model) ;
              break ; 
      case  15:        
              sprintf((char*)Data,"date=\"20%02d-%02d-%02d %02d:%02d:%02d\"\r\n",WebFile.Clock.Year,WebFile.Clock.Month,WebFile.Clock.Date,WebFile.Clock.Hours,WebFile.Clock.Minutes,WebFile.Clock.Seconds) ;
              break ; 
      case  16:
              sprintf((char*)Data,"api_key=\"%s\"\r\n","9%2BJKLSfizDw%3D") ;
              break ;        
      case  17:        
              sprintf((char*)Data,"error=\"%04X\"\r\n",WebFile.Error) ;
              break ; 
      case  18:        
              sprintf((char*)Data,"service_code=\"%s\"\r\n",WebFile.ServiceCode) ;
              break ; 
      case  19:        
              sprintf((char*)Data,"ver=\"%s\"\r\n",WebFile.Version) ;
              break ; 
      case  20:        
              sprintf((char*)Data,"sernum=\"%s\"\r\n",WebFile.ConsoleSerialNumber) ;
              break ; 
      case  21:        
              sprintf((char*)Data,"frame_sernum=\"%s\"\r\n",WebFile.FrameSerialNumber) ;
              break ; 
      case  22:        
              sprintf((char*)Data,"utilization=\"%d\"\r\n",WebFile.duration) ;
              break ; 
      case  23:        
              sprintf((char*)Data,"odometer=\"%d\"\r\n",WebFile.distance) ;
              break ; 
      case  24:        
              sprintf((char*)Data,"odometer_unit=\"%d\"\r\n",WebFile.distance_unit) ;
              break ; 
      case  25:        
              sprintf((char*)Data,"motor_temp=\"%d\"\r\n",WebFile.motor_temp) ;
              break ; 
      case  26:        
              sprintf((char*)Data,"motor_temp_unit=\"%d\"\r\n",WebFile.motor_temp_unit) ;
              break ; 
      case  27:        
              sprintf((char*)Data,"bus_current=\"%d\"\r\n",WebFile.bus_current) ;
              break ; 
      case  28:        
              sprintf((char*)Data,"bus_current_unit=\"%d\"\r\n",WebFile.bus_current_unit) ;
              break ; 
      case  29:        
              sprintf((char*)Data,"bus_voltage=\"%d\"\r\n",WebFile.bus_voltage) ;
              break ; 
      case  30:        
              sprintf((char*)Data,"bus_voltage_unit=\"%d\"\r\n",WebFile.bus_voltage_unit) ;
              break ; 
      case  31: // 20120402 Modify  and Add     
              sprintf((char*)Data,"partner_location_id=\"%d\"\r\n",(unsigned short)EEPROM_ReadInformation(EE_CLUB_ID)) ;
              break ;
      case  32: // 20120214 Add         
              sprintf((char*)Data,"model_id=\"%d\" />\r\n",WebFile.module_id) ;
              break ;
      }
  return ;
}




WebFileDataType* WebFile_GetDataAddress(void)
{
  return &WebFile ;
}



void WebFile_SetSerialNumber(unsigned char mode,unsigned char *ModuleStr,unsigned long Ver,unsigned long SerialNumber) 
{
  char Temp[20] ;
    
  __memset(&Temp[0],0,sizeof(Temp)) ;
  if( Ver != 0 )
      sprintf((char*)&Temp[0],"%s%c%09d",ModuleStr,(Ver+'A'),SerialNumber) ;
  else
      sprintf((char*)&Temp[0],"%s%09d",ModuleStr,SerialNumber) ;
  
  if( mode == 0 ) // console
      {
      __memset(&WebFile.ConsoleSerialNumber[0],0,sizeof(WebFile.ConsoleSerialNumber)) ;
      __memcpy(&WebFile.ConsoleSerialNumber[0], Temp,sizeof(Temp) ) ; 
      }
  else
      {
      __memset(&WebFile.FrameSerialNumber[0],0,sizeof(WebFile.FrameSerialNumber)) ;
      __memcpy( &WebFile.FrameSerialNumber[0], Temp,sizeof(Temp) ) ; 
      }
  
  return ;
}



void WebFile_SetServiceCode(unsigned char *ModuleStr,unsigned long Number) 
{
  __memset(&WebFile.ServiceCode[0],0,sizeof(WebFile.ServiceCode)) ;  
  sprintf((char*)&WebFile.ServiceCode[0],"%s%04d",ModuleStr,Number) ;
  return ;
}


//
void WebFile_SetVersion(void) 
{
  __memset(&WebFile.Version[0],0,sizeof(WebFile.Version)) ;  
  //sprintf((char*)&WebFile.Version[0],"%s%09d",ModuleStr,Number) ;
  //sprintf((char*)&WebFile.Version[0],"APP:%s,MLCB:%d","4.0",Digtial_Get_MCBVersion()) ;
  sprintf((char*)&WebFile.Version[0],"APP:%s,MLCB:%d",UCB_SoftwareVerWiFi,Digtial_Get_MCBVersion()) ;
  return ;
}


//------------------------------- End of File ----------------------------------
