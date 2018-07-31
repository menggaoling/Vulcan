#ifndef _Formula_H_
#define _Formula_H_

#define _TM_   1
#define _Bike_ 2

long unsigned Formula_Calories(char by_Type,long unsigned by_Cal_Speed,long unsigned by_Cal_Incline);
unsigned short Formula_HourCalories(long unsigned by_Cal_Weight,long unsigned by_Bu);
unsigned short Formula_SecCalories(long unsigned by_Cal_Weight,long unsigned by_Bu);
unsigned short Formula_BMI(char by_Type,char _Unit,unsigned short _Weight,unsigned short _Height);
unsigned short Formula_VO2(char by_Type,unsigned short _ACCtime,unsigned short _BMI);
unsigned short Formula_METs(char by_Type,char _Unit,unsigned short _Speed,unsigned short _Incline);


#endif
