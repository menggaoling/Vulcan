

#ifndef __WEBFILE_H__
#define __WEBFILE_H__

typedef struct {
  //
  unsigned char *mac ;
  unsigned char *ip ;
  unsigned char mfr ;
  unsigned char model ;
  SHT1381 Clock;
  //
  unsigned long  duration ;
  unsigned long  distance ;
  unsigned short vertical ;
  unsigned short Calories ; //Kcal/min
  unsigned short avg_pace ;
  unsigned short avg_speed ;
  unsigned short history_count ;
  unsigned short *distance_history ;
  unsigned short *speed_history ;
  unsigned char *incline_history ; // %
  unsigned char *HRT_history ;
  unsigned char distance_unit ;
  unsigned char vertical_unit ;
  unsigned char avg_HRT ;
  unsigned char max_HRT ;
  unsigned char speed_unit ;
  unsigned char program ;
  unsigned char increment ;
  //
  unsigned char Version[101] ;
  unsigned char ConsoleSerialNumber[20] ;
  unsigned char FrameSerialNumber[20] ;
  unsigned short Error ;
  unsigned char ServiceCode[25] ; // Modify by Kunlung 20120214 short-->char[]
  unsigned short motor_temp ;
  unsigned char motor_temp_unit ;
  unsigned short bus_current ;
  unsigned char bus_current_unit ;
  unsigned short bus_voltage ;
  unsigned char bus_voltage_unit ;
  unsigned char module_id ;
  //
} WebFileDataType ;



/* Xml file format samples
<workout user_id='0' mac='00:13:02:1b:3e:84' mfr='15' model='4'
    date='2010-10-14 12:45:31' duration='150' distance='8'
    distance_unit='3' vertical='4' vertical_unit='5'
    calories='7' avg_pace='1875' avg_heart_rate='88'
    max_heart_rate='97' avg_speed='19' speed_unit='17'
    program_id='1' >
    <distance_history increment='5'>0,0,0,0,0,0,0,1,1,1,2,2,2,3,3,3,3,4,4,4,5,5,5,6,6,6,7,7,7,7,8
    </distance_history>
    <speed_history increment='5'>10,10,10,10,10,10,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22
    </speed_history>
    <incline_history increment='5'>6,6,6,6,6,6,6,10,10,10,
        10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,
        10,10,10,10
    </incline_history>
    <heart_rate_history increment='5'>0,0,65,79,88,93,95,95,
        97,97,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    </heart_rate_history>
</workout>

<workout>: This attribute is the parent node. It contains everything that represents a complete workout.
user_id: Represents the Netpulse user id for the exerciser. This value should always be 0 (zero)
mac: MAC address of the third party client
mfr: The manufacturer id from CSAFE
model: The model id from CSAFE
date: The local date and time of the workout. 
duration: The number of seconds for the workout.
distance: The distance traveled during the exercise. This value is further defined by the distance_unit attribute.
distance_unit: The CSAFE value that represents the distance unit. See the CSAFE specification
vertical: The vertical distance traveled during the exercise. This value is further defined by the vertical_unit attribute. (optional)
vertical_unit: The CSAFE value that represents the distance unit. See the CSAFE specification
calories: The number of calories burned during the workout.
avg_pace: The average pace (optional)
avg_heart_rate: The average heart rate during the workout. 
max_heart_rate: The max heart rate during the workout.
avg_speed: The average speed during the workout (optional)
speed_unit: The CSAFE value that represents the speed unit. See the CSAFE specification
program_id: This value represents the program identifier for the workout on the exercise equipment. 
<distance_history>: This tag contains a coma-separated list of values. Each value represents the distance for the workout. Use the increment attribute to figure out the rate (in seconds) at which the data was recorded
increment: This value represents the number of seconds that each item in the history was saved.
<speed_history>: This tag contains a coma-separated list of values. Each value represents the speed for the workout. Use the increment attribute to figure out the rate (in seconds) at which the data was recorded. The speed_unit attribute from the <workout> tag will be used for calculating the speed history.
increment: This value represents the number of seconds that each item in the history was saved.
<incline_history>: This tag contains a coma-separated list of values. Each value represents the incline for the workout. Use the increment attribute to figure out the rate (in seconds) at which the data was recorded. If the exercise device does not support incline, an empty <incline_history> tag should be included. (tag required, contents are optional)
increment: This value represents the number of seconds that each item in the history was saved.
<heart_rate_history>: This tag contains a coma-separated list of values. Each value represents the heart rate for the workout. Use the increment attribute to figure out the rate (in seconds) at which the data was recorded. If the exercise device does not support heart rate, an empty <heart_rate_history> tag should be included. If the exerciser did not provide a heart rate value, a 0 (zero) should be entered.
increment: This value represents the number of seconds that each item in the history was saved.
*/
void WebFile_Initial(void) ;
short WebFile_SaveHistory( unsigned short D, unsigned short S, unsigned char I, unsigned char HRT ) ;
void WebFile_CreateStream(unsigned char StreamNo,unsigned char *Data) ;
WebFileDataType* WebFile_GetDataAddress(void) ;
void WebFile_SetSerialNumber(unsigned char mode,unsigned char *ModuleStr,unsigned long Ver,unsigned long SerialNumber) ;
void WebFile_SetServiceCode(unsigned char *ModuleStr,unsigned long Number) ;
void WebFile_SetVersion(void) ;
#endif /* __WEBFILE_H__ */


