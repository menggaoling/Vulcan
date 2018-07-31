/*
 * wifi_event_handlers.c
 *
 *  Created on: May 23, 2013
 *      Author: brenden.capps
 */

#include "wifi_event_handlers.h"
#include "wifi_define.h"
#include "ht1381.h"
#include "UsbFunction.h"
#include <stdlib.h>
#include <stdio.h>
#include "General.h"
#include "C_safe.h"
#include "WiFi_Information.h"
//


// 2014.06.17
//static bool workout_paused = false;

//PROGRAMID GetProgramID( void )
//{
//  return WiFi_Information_GetProgramMode();
//}

/* This function is called when a remote app wants to start a program.
 * If there is not a workout running, the software should start the
 * machine and run a manual workout using the warmup, cooldown, and workout times
 * to compute the length of the workout.
 */


unsigned char StartWorkout( WIFIDataProgramStart* program )
{
  if( WiFi_Information_Get(_WiFi_WorkoutState) == _WiFi_WorkoutState_WarmUp )//!Data_In_Workout() 2014.06.17
  {
      // 2014.06.17
      //Data_Set_Start( 1 );
      WiFi_Information_Set(_WiFi_WorkoutState,1);
      //workout_paused = false;
      //
      return 0;
  }
  return 1;
}

/*
 * This function is restarts a paused workout.  Restart the workout if in pause mode.
 */
unsigned char RestartWorkout()
{
  if( WiFi_Information_Get(_WiFi_WorkoutState) == _WiFi_WorkoutState_Workout )//Data_In_Workout() 2014.06.17
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
  if( WiFi_Information_Get(_WiFi_WorkoutState) == _WiFi_WorkoutState_Workout )//Data_In_Workout() 2014.06.17
  {
      // 2014.06.17
      //Data_Set_Start( 0 );
      WiFi_Information_Set(_WiFi_WorkoutState,0);
      //
      return 0;
  }
  return 1;
}

/*
 * This function pauses the currently running workout.
 */
unsigned char PauseWorkout()
{
  if( WiFi_Information_Get(_WiFi_WorkoutState) == _WiFi_WorkoutState_Workout )// Data_In_Workout() 2014.06.17
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
unsigned char SetSpeed( UINT16 speedX10, unsigned char metric )// bool  2014.06.17
{
//  printf( "Set Speed %d \n", mphX10 );//sean_move
  if(WiFi_Information_Get(_WiFi_WorkoutState) == _WiFi_WorkoutState_WarmUp)//!Data_In_Workout()  2014.06.17
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
unsigned char SetIncline( UINT16 inclineX10 )
{
  if((WiFi_Information_Get(_WiFi_WorkoutState) == _WiFi_WorkoutState_WarmUp)||\
     (inclineX10 < machineConfig->minInclinex10)||\
     (inclineX10 > machineConfig->maxInclinex10) )//!Data_In_Workout() 2014.06.17
  {
	  return 1;
  }
  // TODO: Set the incline here.
  // 2014.06.17
  //Incline_Set_Incline( inclineX10 );
  WiFi_Information_Set(_WiFi_Incline, inclineX10 );
  //
  return 0;
}

/*
 * This function changes the resistance level for the bike / EP.
 */
unsigned char SetResistance( UINT8 resistance )
{
  
  // TODO: Set the resistance here.
  // 2014.06.17
  //Data_Set_Resistance( resistance );
  WiFi_Information_Set(_WiFi_ResistanceLevel,resistance );
  //
  return 0;
}

/*
 * This function changes the workout type and workout time for the current workout.
 * Also the machine should adjust to the start speed, start incline, and start level.
 */
unsigned char ChangeProgram( WIFIDataProgramStart* program )
{
  if( WiFi_Information_Get(_WiFi_WorkoutState) == _WiFi_WorkoutState_Workout )//Data_In_Workout()  2014.06.17
  {
      // TODO: Update workout here.
      return 0;
  }
  return 1;
}

/*
 * This function is used to set the raw ADC value.  This should only be called in test mode.
 */
/*
unsigned char SetInclineADC( UINT16 adc )
{
  if( adc >= machineConfig.minInclineADC && adc <= machineConfig.maxInclineADC )
  {
      // TODO: Set incline ADC here.
      // Incline_Set_ADC( adc );//sean_0618
      return 0;
  }
  return 1;
}
*/
/*
 * This function is used to set the raw RPM value for the treadmill.  This should only be called in
 * test mode.
 */
unsigned char SetRPM( UINT16 rpm )
{
  if((WiFi_Information_Get(_WiFi_WorkoutState) == _WiFi_WorkoutState_WarmUp)&&\
     (rpm >= machineConfig->minKPH_RPM)&&\
     (rpm <= machineConfig->maxKPH_RPM) )//!Data_In_Workout() 2014.06.17
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
unsigned char SetECB( UINT16 ecb )
{
  // TODO: Set raw ECB counts here.
  return 0;
}

/*
 * This function is used to set the raw PWM value for induction brakes.  This is useful at times
 * during certain fitness tests so may be called during remote control or test mode.
 */
unsigned char SetPWM( UINT16 pwm )
{
  // TODO: Set raw PWM here.
  return 0;
}

/*
 * This function is used to simulate a user pressing a key.
 */
unsigned char PressKey( UINT8 key )
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
  // TODO: Fill this out with actual data.
  data->programType = WiFi_Information_Get(_WiFi_ProgramID);
  // 0 = Warmup, 1 = Workout, 2 = Cooldown, 3 = Pause
  data->workoutState = WiFi_Information_Get(_WiFi_WorkoutState);
  //
  data->stage = WiFi_Information_Get(_WiFi_WorkoutStage);
  // Stage time is used for table based programs, intervals, and fit test.  If this
  // is a manual workout, THR, VA, or constant watts the stage time = state time.
  data->stageTimeSeconds = WiFi_Information_Get(_WiFi_WorkoutStageTime);
  data->workoutTimeSeconds = WiFi_Information_Get(_WiFi_WorkoutTime);
  data->warmupTimeSeconds = WiFi_Information_Get(_WiFi_WarmUpTime);
  data->cooldownTimeSeconds = WiFi_Information_Get(_WiFi_CoolDownTime);

  data->timeInStateSeconds = WiFi_Information_Get(_WiFi_TimeInState);
  data->timeInStageSeconds = WiFi_Information_Get(_WiFi_TimeInStage);

  data->mphx10 = WiFi_Information_Get(_WiFi_Speed);
  data->inclinex10 = WiFi_Information_Get(_WiFi_Incline);
  data->totalCalories = WiFi_Information_Get(_WiFi_Calories);
  data->caloriesPerHour = WiFi_Information_Get(_WiFi_CaloriesPerHour);

  // TODO: Get Mets and Watts calculations for cardio.
  data->metsX10 = WiFi_Information_Get(_WiFi_METs);
  data->watts = WiFi_Information_Get(_WiFi_Watts);

  data->milesx100 = WiFi_Information_Get(_WiFi_Distance);
  data->averagePace = WiFi_Information_Get(_WiFi_AveragePace);
  data->heartRate = WiFi_Information_Get(_WiFi_HeartRate);

  // TODO: Compute get RPM for cardio machine.
  data->rpm = WiFi_Information_Get(_WiFi_RPM);
  data->programLevel = WiFi_Information_Get(_WiFi_ProgramLevel);
  // TODO: Get resistance level for cardio.
  data->resistanceLevel = WiFi_Information_Get(_WiFi_ResistanceLevel);
  // TODO: Get THR for Target Heart rate Program.
  data->thr = WiFi_Information_Get(_WiFi_THR);
  // TODO: Get Target Watts for Target Heart rate Program.
  data->targetWattsX10 = WiFi_Information_Get(_WiFi_TargetWatts);
  // TODO: Get goal for calorie and distance goal programs.
  
  data->nextSpeed = WiFi_Information_Get(_WiFi_NextIncline);
  data->nextIncline = WiFi_Information_Get(_WiFi_NextSpeed);
  data->nextResistance = WiFi_Information_Get(_WiFi_NextResistance);
    //
  //  data->totalFloorsX10 = WiFi_Information_Get(_WiFi_Floors);
  //  data->totalSteps = WiFi_Information_Get(_WiFi_TotalSteps);

}

//#endif
  
// This function updates the time based on the input.
// 这设定主要时间来源为网络上所回馈的日期与时间
bool UpdateRTCTime( WIFIDataRegion* time, int offset )
{
  unsigned char by_Bu; 
  SHT1381 now;//==>HT1381时间IC Data指向

  // Validate time.
    if( time->hour > 23 || time->minute > 59 || time->second > 59 || \
        time->year < 1900 || \
      time->month < 1 || time->month > 12 || \
        time->day < 1 || time->day > 31
        )
  {
      return false;
  }
  // Check for valid day.
  switch( time->month )
  {
      case 4:
      case 6:
      case 9:
      case 11:
              if( time->day > 30 )
                  return false;
              break;
      case 2:
            if( time->day > ((time->year%4 == 0)?29:28) )
                      return false;
              break;
  }
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
    WiFi_memset( machineConfig->timeZone, 0, _WIFI_Length_TimeZone );
    WiFi_memcpy( machineConfig->timeZone, time->timeZone, (_WIFI_Length_TimeZone-1) );
    machineConfig->timeFormat = time->timeFormat;

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


