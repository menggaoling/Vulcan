/*
 * wifi_event_handlers.c
 *
 *  Created on: May 23, 2013
 *      Author: brenden.capps
 */

//#include "Information.h"
#include "wifi_event_handlers.h"
#include "wifi_define.h"


//#ifdef __WIFI_FUNC__
#include"ht1381.h"
#include "UsbFunction.h"

#include "bike.h"
#include <stdlib.h>
#include <stdio.h>
//#include <string.h>

// 2014.06.17
#include "General.h"
#include "C_safe.h"
#include "WiFi_Information.h"
//


// 2014.06.17
//static bool workout_paused = false;

PROGRAMID GetProgramID( void )
{
  PROGRAMID ProgramID = MANUAL;
  switch(WiFi_Information_GetProgramMode())
  {
      case TM_Rolling:
                ProgramID = ROLLING_HILLS_SPEED_INCLINE;
                break;
      case TM_FatBurn:
                ProgramID = FATBURN; 
                break;
      case TM_Gerkin:
                ProgramID = GERKIN_FITNESS_TEST; 
                break;
      case TM_PEB:
                ProgramID = PEB_FITNESS_TEST;
                break;
      case TM_Army:
                ProgramID = ARMY_FITNESS_TEST;
                break;
      case TM_Navy:
                ProgramID = NAVY_FITNESS_TEST;
                break;
      case TM_Airforce:
                ProgramID = AIR_FORCE_FITNESS_TEST;
                break;
      case TM_Marine:
                ProgramID = MARINES_FITNESS_TEST;
                break; 
      case TM_WFI:
                ProgramID = WFI_FITNESS_TEST;
                break;
      case TM_TargetHR:
                ProgramID = THR_ZONE;
                break;
      case TM_5K:
                ProgramID = GOAL_5K;
                break;
      case TM_Sprint8:
                ProgramID = SPRINT_8;
                break;
                
      default:
                ProgramID = MANUAL;
                break;
  }
  return ProgramID ;
}

/* This function is called when a remote app wants to start a program.
 * If there is not a workout running, the software should start the
 * machine and run a manual workout using the warmup, cooldown, and workout times
 * to compute the length of the workout.
 */


int StartWorkout( WIFIDataProgramStart* program )
{
  if( WiFi_Information_GetWorkoutStatus() == 0 )//!Data_In_Workout() 2014.06.17
  {
      // 2014.06.17
      //Data_Set_Start( 1 );
      WiFi_Information_SetWorkoutStatus(1);
      //workout_paused = false;
      //
      return 0;
  }
  return 1;
}

/*
 * This function is restarts a paused workout.  Restart the workout if in pause mode.
 */
int RestartWorkout()
{
  if( WiFi_Information_GetWorkoutStatus() == 1 )//Data_In_Workout() 2014.06.17
  {
      // 2014.06.17
      //if( workout_paused )
      //        workout_paused = false;
      //
      return 0;
  }
  return 1;
}

/*
 * This function stops the workout.
 */
unsigned char StopWorkout(void)
{
  if( WiFi_Information_GetWorkoutStatus() == 1 )//Data_In_Workout() 2014.06.17
  {
      // 2014.06.17
      //Data_Set_Start( 0 );
      WiFi_Information_SetWorkoutStatus(0);
      //
      return 0;
  }
  return 1;
}

/*
 * This function pauses the currently running workout.
 */
int PauseWorkout()
{
  if( WiFi_Information_GetWorkoutStatus() == 1 )// Data_In_Workout() 2014.06.17
  {
      // 2014.06.17
      //if( !workout_paused )
      //        workout_paused = true;
      //
      return 0;
  }
  return 1;
}

/*
 * This function changes the treadmill speed
 */
int SetSpeed( UINT16 speedX10, unsigned char metric )// bool  2014.06.17
{
//  printf( "Set Speed %d \n", mphX10 );//sean_move
  if(WiFi_Information_GetWorkoutStatus() == 0)//!Data_In_Workout()  2014.06.17
  {
	  return 1;
  }

  if( metric && ( speedX10 < machineConfig->minKPHx10 || speedX10 > machineConfig->maxKPHx10 ) )
		  return 1;
  else if( !metric && ( speedX10 < machineConfig->minMPHx10 || speedX10 > machineConfig->maxMPHx10 ) )
		return 1;

  // TODO: Set the speed of the treadmill here.
  return 0;
}

/*
 * This function changes the treamdill or EP incline.
 */
int SetIncline( UINT16 inclineX10 )
{
  if( WiFi_Information_GetWorkoutStatus() == 0 || inclineX10 < machineConfig->minInclinex10 || inclineX10 > machineConfig->maxInclinex10 )//!Data_In_Workout() 2014.06.17
  {
	  return 1;
  }
  // TODO: Set the incline here.
  // 2014.06.17
  //Incline_Set_Incline( inclineX10 );
  WiFi_Information_SetIncline( inclineX10 );
  //
  return 0;
}

/*
 * This function changes the resistance level for the bike / EP.
 */
int SetResistance( UINT8 resistance )
{
  
  // TODO: Set the resistance here.
  // 2014.06.17
  //Data_Set_Resistance( resistance );
  WiFi_Information_SetResistance(resistance );
  //
  return 0;
}

/*
 * This function changes the workout type and workout time for the current workout.
 * Also the machine should adjust to the start speed, start incline, and start level.
 */
int ChangeProgram( WIFIDataProgramStart* program )
{
  if( WiFi_Information_GetWorkoutStatus() == 1 )//Data_In_Workout()  2014.06.17
  {
      // TODO: Update workout here.
      return 0;
  }
  return 1;
}

/*
 * This function is used to set the raw ADC value.  This should only be called in test mode.
 */
int SetInclineADC( UINT16 adc )
{
  if( adc >= machineConfig->minInclineADC && adc <= machineConfig->maxInclineADC )
  {
      // TODO: Set incline ADC here.
      // Incline_Set_ADC( adc );//sean_0618
      return 0;
  }
  return 1;
}

/*
 * This function is used to set the raw RPM value for the treadmill.  This should only be called in
 * test mode.
 */
int SetRPM( UINT16 rpm )
{
  if( WiFi_Information_GetWorkoutStatus() == 0 && rpm >= machineConfig->minKPH_RPM && rpm <= machineConfig->maxKPH_RPM )//!Data_In_Workout() 2014.06.17
  {
      // TODO: Set RPM here.
      return 0;
  }
  return 1;
}

/*
 * This function is used to set the raw ECB counts.  This is useful at times during certain
 * fitness tests so may be called during remote control or test mode.
 */
int SetECB( UINT16 ecb )
{
  // TODO: Set raw ECB counts here.
  return 0;
}

/*
 * This function is used to set the raw PWM value for induction brakes.  This is useful at times
 * during certain fitness tests so may be called during remote control or test mode.
 */
int SetPWM( UINT16 pwm )
{
  // TODO: Set raw PWM here.
  return 0;
}

/*
 * This function is used to simulate a user pressing a key.
 */
int PressKey( UINT8 key )
{
  // TODO: Simulate key press.
  return 0;
}
/*
 * This function fills out the structure passed with the current workout data.
 */
void GetWorkoutDataRemote( WIFIWorkoutDataRemote* data )
{
  /* Notes on the various times.
   *
   * Workout Time = Total Workout Time - warm up - cool down.
   *
   * Stage Time Seconds = Program Stage Time.  This only applies to programs with multiple stages like
   * the fitness test and table based programs.
   *
   * State Time Seconds = Current state time.  This depends on the program state which is either in
   * warmup, workout, cool down, or pause.
   *
   * timeIn... variables is the elapsed time.
   */
  // 2014.06.17
  unsigned int totalTime = WiFi_Information_GetWorkoutTime();//Data_Get_Time(DT_TARGET);
  unsigned int workoutTime = WiFi_Information_GetAccTime();//Data_Get_Time(DT_TIME);
  //

  // TODO: Fill this out with actual data.
  data->programType = 0;

  // 0 = Warmup, 1 = Workout, 2 = Cooldown, 3 = Pause
  data->workoutState = 1;

  // Stage time is used for table based programs, intervals, and fit test.  If this
  // is a manual workout, THR, VA, or constant watts the stage time = state time.
  data->stage = 0;

  data->stageTimeSeconds = totalTime;
  data->workoutTimeSeconds = totalTime;
  data->warmupTimeSeconds = 0;
  data->cooldownTimeSeconds = 0;

  data->timeInStateSeconds = workoutTime;
  data->timeInStageSeconds = workoutTime;

  data->mphx10 = WiFi_Information_GetSpeed();//Data_Get_Speed(); 2014.06.17
  //if( machineConfig->unit == C_KM )
  //        data->mphx10 = data->mphx10 * 1609 / 1000;
  data->inclinex10 = WiFi_Information_GetIncline() * 10 / 2;   //Incline_Get_Incline() //2014.06.17
  data->totalCalories = WiFi_Information_GetCalories();//Data_Get_Calories();  2014.06.17
  UINT32 calorieRate = (UINT32)WiFi_Information_GetCalories() * 60 * 60 / 10000; //Data_Get_Calories()  2014.06.17
  data->caloriesPerHour = (UINT16)calorieRate;

  // TODO: Get Mets and Watts calculations for cardio.  The calculations below are for a treadmill.
  //UINT32 metersPerMinute = ( (UINT32)data->mphx10 * 1609 / 60 + 5 ) / 10;  2014.06.17
  if( data->mphx10 <= 40 )
      data->metsX10 = (UINT16)( ( (UINT32)35000 + 2680 * data->mphx10 + 4828 * data->mphx10 * data->inclinex10 / 100 ) / 3500 );
  else
      data->metsX10 = (UINT16)( ( (UINT32)35000 + 5360 * data->mphx10 + 2414 * data->mphx10 * data->inclinex10 / 100 ) / 3500 );
  data->watts = data->caloriesPerHour * 10 / 42;

  UINT32 meters = WiFi_Information_GetDistance();//Data_Get_Distance();  2014.06.17
  UINT32 milesX1000 = meters * 1000 / 1609;
  data->milesx100 = (UINT16)(( milesX1000 + 5 ) / 10 );
  data->averagePace = milesX1000 / workoutTime / 1000;
  data->heartRate = WiFi_Information_GetHeartRate();// 2014.06.17

  // TODO: Compute get RPM for cardio machine.
  data->rpm = 0;
  data->programLevel = WiFi_Information_GetLevel();//Program_Get_Level();  2014.06.17
  // TODO: Get resistance level for cardio.
  data->resistanceLevel = 0;
  // TODO: Get THR for Target Heart rate Program.
  data->thr = 0;
  // TODO: Get Target Watts for Target Heart rate Program.
  data->targetWattsX10 = 0;
  // TODO: Get goal for calorie and distance goal programs.
  data->goal = 0;
  // TODO: Get next speed, incline, and resistance from programs.
  data->nextSpeed = -1;
  data->nextIncline = -1;
  data->nextResistance = -1;

}

//#endif
char WF_HRC_Get_Target( void )
{
  char targetHeartRate = 0;
  
  // 2014.06.17
  if( WiFi_Information_GetProgramMode( ) == TM_TargetHR)
  {
      targetHeartRate = WiFi_Information_GetTHR();
  }
  //
  return targetHeartRate;
}
unsigned int WF_Get_target_Watt( void )
{
  unsigned int targetWatts = 0;
  
  //if( WiFi_Information_GetProgramMode( ) == BK_ConstantWatts)
  //{
  //    targetWatts = WiFi_Information_GetConstWatts();// WC_Get_Watt( 0 );  
  //}
  //
  return targetWatts ;
}
/*
 * This function fills out the structure passed with the current workout data.
 */
void GetWorkoutData( WIFIWorkoutData* data )
{

	/* Notes on the various times.
	 *
	 * Workout Time = Total Workout Time - warm up - cool down.
	 *
	 * Stage Time Seconds = Program Stage Time.  This only applies to programs with multiple stages like
	 * the fitness test and table based programs.
	 *
	 * State Time Seconds = Current state time.  This depends on the program state which is either in
	 * warmup, workout, cool down, or pause.
	 *
	 * timeIn... variables is the elapsed time.
	 */
        // 2014.06.17
        unsigned int totalTime = WiFi_Information_GetWorkoutTime();// Data_Get_Time(DT_TARGET); 
        //mcu time is 0 to 59,wifi 1 to 60
	unsigned int workoutTime = WiFi_Information_GetAccTime();// Data_Get_Time(DT_TIME) +1; 
        unsigned int WarmupTime  = WiFi_Information_GetWarmUpTime();//Program_Get_WarmupTime( );
	unsigned int CoolDownTime = WiFi_Information_GetCoolDownTime();//Program_Get_CooldownTime( );
        //
        
	// TODO: Fill this out with actual data.
	data->programType = GetProgramID( );
        
        /*state start*/
	// 0 = Warmup, 1 = Workout, 2 = Cooldown, 3 = Pause
	if(WiFi_Information_GetWorkoutStatus() == 0)//Data_Get_Start( )  2014.06.17
	{
	    data->workoutState = 3;
	}
	else
	{
            if( workoutTime <= WarmupTime )//1-60*4
            {
                data->workoutState = 0;
            }
            else
            {
                if(((totalTime) - workoutTime) <= (CoolDownTime) )
                {
                    data->workoutState = 2;
                }
                else
                {
                    data->workoutState = 1;
                }
            }
	}
        
        if(totalTime)
        {
	    data->workoutTimeSeconds = totalTime - WarmupTime - CoolDownTime;
        }
        else
        {
            data->workoutTimeSeconds = 0;
        }
	data->warmupTimeSeconds = WarmupTime;
	data->cooldownTimeSeconds = CoolDownTime;
        
        static long int timeInStateSeconds = 0;
        switch(data->workoutState)
        {
            case 0:
            {
                timeInStateSeconds = workoutTime;
                break;
            }
            case 1:
            {
                timeInStateSeconds = workoutTime - WarmupTime;
                break;
            }
            case 2:
            {
                timeInStateSeconds =  CoolDownTime - (totalTime - workoutTime);
                break;
            }
            case 3:
            {
                if(WiFi_Information_WorkoutFinish() == 1)//Program_Get_Finish()) 2014.06.17
                {
                  timeInStateSeconds =  CoolDownTime - (totalTime - workoutTime);
                }
                break;
            }
        }
        data->timeInStateSeconds = timeInStateSeconds;
        /*state end*/
        
        /*stage start*/
	// Stage time is used for table based programs, intervals, and fit test.  If this
	// is a manual workout, THR, VA, or constant watts the stage time = state time.
        // 2014.06.17
	//data->stage = Program_Get_Current_Stage( );
	//data->stageTimeSeconds = Program_Get_Stage_Time( );
        //mcu time is 0 to 59,wifi 1 to 60..so add 1
        //data->timeInStageSeconds = Program_Get_TimeInStage( ) + 1;
        data->mphx10 = WiFi_Information_GetSpeed();//Data_Get_Speed(); 2014.06.17
        //
	if( machineConfig->unit == C_KM )
        {
	    data->mphx10 = data->mphx10 * 1000 / 1609;
        }
	//data->inclinex10 =  Incline_Get_Incline()*5 ; 2014.06.17
	//Data_Get_Resistance() * 10 / 2;
        
	data->totalCalories = WiFi_Information_GetCalories();//Data_Get_Calories(); 2014.06.17
        UINT32 calorieRate = data->totalCalories;
	calorieRate = (UINT32)calorieRate*3600/workoutTime;
	data->caloriesPerHour = (UINT16)calorieRate;
        
	// TODO: Get Mets and Watts calculations for cardio.  The calculations below are for a treadmill
        
	if( machineConfig->machineType == TREADMILL )
	{
		//UINT32 metersPerMinute =( (UINT32)data->mphx10 * 1609 / 60 + 5 ) / 10; 2014.06.17
		if( data->mphx10 <= 40 )
			data->metsX10 = (UINT16)( ( (UINT32)35000 + 2680 * data->mphx10 + 4828 * data->mphx10 * data->inclinex10 / 100 ) / 3500 );
		else
			data->metsX10 = (UINT16)( ( (UINT32)35000 + 5360 * data->mphx10 + 2414 * data->mphx10 * data->inclinex10 / 100 ) / 3500 );
		data->watts = data->caloriesPerHour * 10 / 42;
	}
	else if( ( machineConfig->machineType == ELLIPTICAL )
	       ||( machineConfig->machineType == ELLIPTICAL )
	       ||( machineConfig->machineType == ASCENT )
               ||( machineConfig->machineType == BIKE )
	       )
	{
                // 2014.06.17
		data->watts = WiFi_Information_GetWatts();//Data_Get_Watts( );
		//UINT32 weight  = (UINT32)WiFi_Information_GetWeight();//Data_Get_Weight();
                UINT32 weight  = WiFi_Information_Get(_WiFi_Weight);
                //
		if( machineConfig->unit == C_MILE )
		{		
			weight = weight*1609/1000;
		}
		data->metsX10 = (((UINT32)WiFi_Information_GetWatts() * 120) + weight * 35 )/(weight*35)/10;// Data_Get_Watts( )  2014.06.17
	}
	else if(( machineConfig->machineType == STEPPER ) )
	{
	}
	else if(( machineConfig->machineType == CLIMB_MILL ) )
	{
	}
	else if(( machineConfig->machineType == ROWER ) )
	{
	}
        //2014.06.17
	UINT32 meters = WiFi_Information_GetDistance();//Data_Get_Distance( ); 2014.06.17
	if( machineConfig->unit == C_MILE )
	{
		meters = meters*16/10;
	}
	UINT32 milesX1000 = meters * 1000 / 1609;
	data->milesx100 = (UINT16)(( milesX1000 + 5 ) / 10 );
	data->averagePace = milesX1000 / workoutTime / 1000;
	data->heartRate = WiFi_Information_GetHeartRate();// 2014.06.17
	// TODO: Compute get RPM for cardio machine.
	data->rpm = WiFi_Information_GetRPM();//RPM_Get_RPM( );
	data->programLevel = WiFi_Information_GetLevel();//Program_Get_Level() + 1;
	// TODO: Get resistance level for cardio.
	data->resistanceLevel = WiFi_Information_GetResistanceLV();//ECB_Get_Resistance( );
	// TODO: Get THR for Target Heart rate Program.
	data->thr = WiFi_Information_GetTHR();//WF_HRC_Get_Target( );
	// TODO: Get Target Watts for Target Heart rate Program.   
	data->targetWattsX10 = WiFi_Information_GetWatts();// WF_Get_target_Watt ( );  
	// TODO: Get goal for calorie and distance goal programs.
	data->goal = 0;// Manual_Get_Goal( ); 
        //
        //if(data->workoutState != 3)
//        {
//          data->timeInStateSeconds++;
//        }
//        data->timeInStageSeconds++;
        //data->timeInStageSeconds = data->timeInStageSeconds;
}

void GetWorkoutDataComplete( WIFIWorkoutDataComplete* data )
{
        // 2014.06.17
	unsigned int totalTime = WiFi_Information_GetWorkoutTime();// Data_Get_Time(DT_TARGET); 
	unsigned int workoutTime = WiFi_Information_GetAccTime();// Data_Get_Time(DT_TIME) +1; 
        unsigned int WarmupTime  = WiFi_Information_GetWarmUpTime();//Program_Get_WarmupTime( );
	unsigned int CoolDownTime = WiFi_Information_GetCoolDownTime();//Program_Get_CooldownTime( );
        //
        
	// TODO: Fill this out with actual data.
	if( workoutTime <= WarmupTime )
	{
		data->warmupTimeSeconds = workoutTime;
		data->workoutTimeSeconds = 0;
		data->cooldownTimeSeconds = 0;
	}
	else
	{
		data->warmupTimeSeconds = WarmupTime;
		if(((totalTime) - workoutTime) <= (CoolDownTime) )
		{
			data->cooldownTimeSeconds = CoolDownTime - (totalTime - (workoutTime));
			data->workoutTimeSeconds = workoutTime - WarmupTime - data->cooldownTimeSeconds;
		}
		else
		{
			data->workoutTimeSeconds = workoutTime - WarmupTime;			
			data->cooldownTimeSeconds = 0;
		}
	}
	UINT32 meters = WiFi_Information_GetDistance();//Data_Get_Distance(); 2014.06.17
	if( machineConfig->unit == C_MILE )
	{
	    meters = meters*16/10;
	}
        UINT32 milesX1000 = meters * 1000 / 1609;
	data->milesx100 = (UINT16)(( milesX1000 + 5 ) / 10 );
        // 2014.06.17
	data->averageHeartRate = CSAFE_GetInformation(_Get_HRAvg); 
	data->totalCalories = WiFi_Information_GetCalories();//Data_Get_Calories( ); 
	data->programType = GetProgramID( );//WiFi_Information_GetProgramMode(); 
        //
        
}

void GetStartWorkoutData( WIFIDataExternalProgramStart* data )
{
	// TODO: Fill this out with actual data.

	//UINT8 userId = 0;
	//WiFi_memcpy( data->userId, UserInfo[userId]->userExternalId, 36 );
	data->programType = 1;
	data->workoutTime = 2;
	data->warmupTime = 3;
	data->cooldownTime = 4;
	data->startMPHX10 = 5;
	data->startInclineX10 = 6;
	data->startLevel = 7;

}


// This function updates the time based on the input.
// 这设定主要时间来源为网络上所回馈的日期与时间
bool UpdateRTCTime( WIFIDataRegion* time, int offset )
{
  unsigned char by_Bu; 
  SHT1381 now;//==>HT1381时间IC Data指向

  // Validate time.
  if( time->hour <= 0 || time->hour > 23 || \
      time->minute <= 0 || time->minute > 59 || \
      time->second <= 0 || time->second > 59 || \
      time->month < 1 || time->month > 12 || \
      time->day < 1 || time->day > 31 || \
      time->year < 2013 )
    
  {
      return false;
  }
  // Check for valid day.
  switch( time->month )
  {
      case 1:
      case 3:
      case 5:
      case 7:
      case 8:
      case 10:
      case 12:
              if( time->day > 31 )
                  return false;
              break;
      case 4:
      case 6:
      case 9:
      case 11:
              if( time->day > 30 )
                  return false;
              break;
      case 2:
              if(( time->year % 100 == 0 && time->year % 400 == 0 ) ||
                 ( time->year % 4 == 0 && time->year % 100 != 0 )
                )
              {
                  if( time->day > 29 )
                      return false;
              }
              else if( time->day > 28 )
                      return false;
              break;
  }
  if( time->minute == 0 || time->minute > 59 )
      return false;
  if( offset > 0 )
  {
      time->second = ( time->second + offset );
      if( time->second >= 60 )
      {
          time->second = time->second % 60;
          time->minute++;
          if( time->minute == 60 )
          {
              time->minute = 0;
              time->hour++;
              if( time->hour == 24 )
              {
                  time->hour = 0;
                  time->day++;
                  switch( time->month )
                  {
                      case 1:
                      case 3:
                      case 5:
                      case 7:
                      case 8:
                      case 10:
                      case 12:
                              if( time->day == 32 )
                              {
                                  time->day = 1;
                                  time->month++;
                                  if( time->month == 13 )
                                  {
                                      time->month = 1;
                                      time->year++;
                                  }
                              }
                              break;
                      case 4:
                      case 6:
                      case 9:
                      case 11:
                              if( time->day == 31 )
                              {
                                  time->day = 1;
                                  time->month++;
                              }
                              break;
                      case 2:
                              if(( time->year % 100 == 0 && time->year % 400 == 0 ) ||
                                 ( time->year % 4 == 0 && time->year % 100 != 0 )
                                 )
                              {
                                  if( time->day == 30 )
                                  {
                                      time->month++;
                                      time->day = 1;
                                  }
                              }
                              else if( time->day == 29 )
                              {
                                  time->month++;
                                  time->day = 1;
                              }
                              break;
                  }
              }
          }
      }
  }
  by_Bu = (time->day / 10)*16;
  now.Date = by_Bu + (time->day % 10);
  by_Bu = (time->month / 10)*16;
  now.Month = by_Bu + (time->month % 10);
  by_Bu = ((time->year-2000) / 10)*16;
  now.Year = by_Bu + ((time->year-2000)%10);
  by_Bu = (time->hour / 10)*16;
  now.Hours = by_Bu + (time->hour % 10);
  by_Bu = (time->minute / 10)*16;
  now.Minutes = by_Bu + (time->minute % 10);
  by_Bu = (time->second / 10)*16;
  now.Seconds = by_Bu + (time->second % 10);

  WriteHT1381( &now );
  WiFi_memset( machineConfig->timezone, 0, 65 );
  WiFi_memcpy( machineConfig->timezone, time->timeZone, 64 );
  machineConfig->timeFormat = time->timeFormat;
  WiFi_Information_SaveMachineConfig();// 2014.06.17
  return true;
}

void SyncingUserInfo()
{

}


void SyncUserInfoComplete()
{

}


int ProgramSelected( UINT16 program_id )
{
	return 1;
}


void WIFIModeChanged( WIFI_MODE mode )
{

}

void UpdatePWMCalibration( PWM_CAL_POINT calPoint[5] )
{
  
  return;
}

void OnChangedUser( UINT8 userId )
{
}

bool GrantRemoteAccess()
{
	return true;
}


//#endif


