#include "General.h"

/*#################################################################################################
  ###                                    English message                                        ###
  #################################################################################################*/
const unsigned char* const Message_English[]=
{  "          ",
 "Press go or select a workout",
 "Enter your workout time by using any of the arrow keys or the number keypad",
 "Enter your weight using any of the arrow keys or the number keypad",
 "Choose your program level using any of the arrow keys or the number keypad",
  // 5
 "Enter your age using any of the arrow keys or the number keypad",
 "Choose your Target Heart Rate Percentage using any of the arrow keys or the number keypad",
 "Select your gender using any of the arrow keys",
 "Press go to begin",
 "Starting speed 3 MPH - Starting incline 0%",
  // 10
 "Starting speed 4.8 KPH - Starting incline 0%",
 "Your target heart rate is ",
 "Manual",
 "5K Run",
 "10K Run",
  // 15
 "Heart Rate",
 "Fat Burn",
 "Rolling Hills",
 "Gerkin",
 "PEB",
  // 20
 "Army",
 "Navy",
 "Air force",
 "Marine",
 "WFI",
  // 25
 "Weight    ",
 " Level    ",
 " Age      ",
 "Male",
 "Female",
  // 30
 " HR%      ",
 " Time     ",
 "Speed     ",
 "3",
 "2",
  // 35
 "1",
 "Begin",
 "Emergency stop - Lift to reset",
 "Workout complete",
 "Press go to resume",
  // 40
 "Exceeding max heart rate",
 "Total time ",
 "Stage number ",
 "Calories burned ",
 "Superior",
  // 45
 "Excellent",
 "Good",
 "Fair",
 "Poor",
 "Very poor",
  // 50
 "Score ",
 "Outstanding - High",
 "Outstanding - Medium",
 "Outstanding - Low",
 "Excellent - High",
  // 55
 "Excellent - Medium",
 "Excellent - Low",
 "Good - High",
 "Good - Medium",
 "Good - Low",
  // 60
 "Satisfactory - High",
 "Satisfactory - Medium",
 "Probationary",
 "Out of order",
 "Enter the speed using any of the arrow keys or the number keypad",
  // 65
 "Enter the incline using any of the arrow keys or the number keypad",
 "Incline    ",
 "Enter FitLinxx ID",
 "Submitting ID",
 "ID accepted",
  // 70
 "Incorrect login - Please try again",
 "Network error",
 "Please hold",
 "cool down",
 "Press stop to select a workout",
  // 75
 "Press stop to select a workout",
 "Pause",
 "Total time ",
 "Distance",
 "Avg. pace",
  // 80
 "Test complete",
 "No HR detected",
 "Watts",
 "Detecting HR",
 "Please wait for the system to reset",
  // 85
 "Heart Rate Detected",
 "Press stop to select a workout",
 "Press stop to select a workout",
 "Press stop to select a workout",
 "Press stop to select a workout",
  // 90
 "Enter your height using any of the arrow keys or the number keypad",
 "Height    ",
 "BMI",
 "Goal achieved",
 "Points ",
  // 95
 "Press enter to install new software",
 "Select software",
 "Incorrect software",
 "No USB",
 "WORKOUT SAVED TO USB",
  // 100
 "WORKOUT SAVE FAILED",
 "Ready To Enter Sleep Mode",
 "LCB No Wake Up",
 "Software update via WiFi",
 "Channel ",
  // 105
 "Volume ",
 "Mute",
 "Select speed or incline",
 "Max incline",
 "Stop in   seconds",
  // 110
 "No user detected - workout will end in 10 seconds",
 "Add languages",
 "Select logo file",
 "No update file",
 "Updating...",
  // 115
 "Updating...",
 "Incorrect software",
 "Update complete",
 "Update error",
  // WiFi
 "Enter xID or select a workout",
  // 120
 "Enter 4 digit passcode",
 "Login.....",
 "Incorrect login - Please try again",
 "Activated",
 "Incorrect login - Please try again",
  // 125
  // Headphone Jack
 "Check earphone board",
 "Replace Earphone Board",
   // Bad Keypad
 "Replace Up Toggle Overlay",
 "Replace Lower Toggle Overlay",
 "Replace Quick Key Overlay",
  // 130
 "Training Workouts",
 "Fitness Test",
 "Welcome",
 "Console software will be updated. Press any key to skip",
 "xID workout tracking available, press go or select a workout to begin, or wait to login with xID",
 "No Internet connection, Please try again later"
};



/*
uc8 E0[]="                  ";
//uc8 E1[]="Begin your workout by pressing the GO button or one of the workout buttons";
uc8 E1[]="Press Go Or Select Workout";
uc8 E2[]="Enter your workout time by using any of the arrow keys or the number keypad";
uc8 E3[]="Enter your weight using any of the arrow keys or the number keypad";
uc8 E4[]="Choose your program level using any of the arrow keys or the number keypad";
uc8 E5[]="Enter your age using any of the arrow keys or the number keypad";
uc8 E6[]="Choose your Target Heart Rate Percentage using any of the arrow keys or the number keypad";
uc8 E7[]="Select your gender using any of the arrow keys";
uc8 E8[]="Enter your speed using any of the arrow keys or the number keypad";
uc8 E9[]="Press GO to begin your workout";
uc8 E10[]="Starting Speed 3.0MPH Starting Incline 0%";
uc8 E11[]="Your Target Heart Rate is ";

uc8 E12[]="      Manual      ";
uc8 E13[]="      5K RUN      ";
uc8 E14[]="   Rolling Hills  ";
uc8 E15[]="     Fat Burn     ";
uc8 E16[]="    Heart Rate    ";
uc8 E17[]="      Gerkin      ";
uc8 E18[]="        PEB       ";
uc8 E19[]="       Army       ";
uc8 E20[]="       Navy       ";
uc8 E21[]="     Airforce     ";
uc8 E22[]="      Marine      ";

uc8 E23[]=" Weight :         ";
uc8 E24[]="  Level :         ";
uc8 E25[]="    Age :         ";
uc8 E26[]=" Gender :   Male  ";
uc8 E27[]=" Gender :  Female ";
uc8 E28[]="THR Percent :     ";
uc8 E29[]="   Time :         ";
uc8 E30[]="  Speed :         ";
uc8 E31[]="         3        ";
uc8 E32[]="         2        ";
uc8 E33[]="         1        ";
uc8 E34[]="       Begin      ";
uc8 E35[]="Emergency Stop Lift to Reset";
uc8 E36[]="    Program END   ";
uc8 E37[]="Press The GO OR PAUSE Button To Restart The Program";
uc8 E38[]="Exceeding Max Heart Rate";
uc8 E39[]="Total Time :      ";
uc8 E40[]="    Stage Number   ";
uc8 E41[]="  Calories Burned  ";
uc8 E42[]="Performance Level is SUPERIOR";
uc8 E43[]="Performance Level is EXCELLENT";
uc8 E44[]="Performance Level is GOOD";
uc8 E45[]="Performance Level is FAIR";
uc8 E46[]="Performance Level is POOR";
uc8 E47[]="Performance Level is VERY POOR";
uc8 E48[]="    Score :       ";
uc8 E49[]="Performance Level is OUTSTANDING-HIGH";
uc8 E50[]="Performance Level is OUTSTANDING-Med";
uc8 E51[]="Performance Level is OUTSTANDING-LOW";
uc8 E52[]="Performance Level is EXCELLENT-HIGH";
uc8 E53[]="Performance Level is EXCELLENT-MEDIUM";
uc8 E54[]="Performance Level is EXCELLENT-LOW";
uc8 E55[]="Performance Level is GOOD-HIGH";
uc8 E56[]="Performance Level is GOOD-MEDIUM";
uc8 E57[]="Performance Level is GOOD-LOW";
uc8 E58[]="Performance Level is SATISFACTORY-HIGH";
uc8 E59[]="Performance Level is SATISFACTORY MEDIUM";
uc8 E60[]="Performance Level is Probationary";
uc8 E61[]="Out of Order";
uc8 E62[]="Enter the speed using any of the arrow keys or the number keypad";
uc8 E63[]="Enter the incline using any of the arrow keys or the number keypad";
uc8 E64[]=" Incline :        ";
uc8 E65[]="Please Enter User ID";
uc8 E66[]="Submitting ID";
uc8 E67[]="ID Accepted";
uc8 E68[]="Bad ID,Please Revised Your ID";
uc8 E69[]="Network Miss";
uc8 E70[]="Please Hold";
uc8 E71[]="Cool Down";
uc8 E72[]="Reset For Fitness test";
uc8 E73[]="Reset For Heart Rate";
uc8 E74[]="   Paused :       ";
uc8 E75[]="Total Time ";
uc8 E76[]="Dist. ";
uc8 E77[]="Avg. Pace ";
uc8 E78[]="Test Terminated";
uc8 E79[]="HEART RATE LOST GRASP SENSORS";
uc8 E80[]=" WATTs ";
uc8 E81[]="   Detecting HR   ";
uc8 E82[]="Restarting... please wait for safety system to reset";
uc8 E83[]="Heart Rate Detected";
uc8 E84[]="Starting Speed 4.8 Km/h Starting Incline 0%";
uc8 E85[]="Reset For Manual";
uc8 E86[]="Reset For Rolling Hills";
uc8 E87[]=" Reset For Fat Burn";
uc8 E88[]="Reset For 5K Run";
uc8 E89[]="        WFI       ";
uc8 E90[]="Enter your height using any of the arrow keys or the number keypad";
uc8 E91[]="  Height :        ";
uc8 E92[]=" BMI   ";
uc8 E93[]="Goal Achieved";
uc8 E94[]=" Points ";
uc8 E95[]="USB Has New Software, Press Enter to Update...";
uc8 E96[]="Please Choose Update Name";
uc8 E97[]="Software Does Not Support This Console";
uc8 E98[]="      No USB      ";
uc8 E99[]="WORKOUT SAVED TO USB";
uc8 E100[]="WORKOUT SAVE FAILED";
uc8 E101[]="Ready to Enter Sleep Mode";
uc8 E102[]="LCB No Wake Up";
uc8 E103[]="Console software update via WiFi";
uc8 E104[]="  Channel :       ";
uc8 E105[]="   Volume :       ";
uc8 E106[]="       Mute       ";
uc8 E107[]="Choose Control Using Speed Or Incline Arrow Keys";
uc8 E108[]="Set Incline Maximum Value";
uc8 E109[]="STOP IN    SECONDS";//"STOPPED IN  SECOND";
uc8 E110[]="NO USER DETECTED, BELT WILL STOP IN 10 SECONDS";
uc8 E111[]="Add Multiple Languages From USB";
uc8 E112[]="Choose One Pattern Name To Update";
uc8 E113[]="No File For Update";
uc8 E114[]="Loading Languages";
uc8 E115[]="Loading Pattern";
uc8 E116[]="This File Can Not Be Used In This Console";
uc8 E117[]="Update Successful";
uc8 E118[]="Updated Failed";
// WiFi
uc8 E119[]="Login xID or press program key to setup workout";
uc8 E120[]="Enter 4 digits Pin";
uc8 E121[]="Login.....";
uc8 E122[]="Pin is wrong,check your account and Pin";
uc8 E123[]="Activated";
uc8 E124[]="Enter 4-14 Digits Account";
// Headphone Jack
uc8 E125[]="Headphone jack will require replacement soon";
// Sprint 8 Program
uc8 E126[]="     Sprint 8     ";
uc8 E127[]="     Beginner     ";
uc8 E128[]="   Intermediate   ";
uc8 E129[]="     Advanced     ";
uc8 E130[]="      Elite       ";
uc8 E131[]="      Custom      ";
uc8 E132[]="  Reset For   Sprint 8";
uc8 E133[]="Set Sprint Speed";
uc8 E134[]=" Set Sprint Incline";
uc8 E135[]="Set Active Recovery Speed";
uc8 E136[]="Set Active Recovery Incline";
uc8 E137[]="Get Ready";
uc8 E138[]="Ramping Up";
uc8 E139[]="  Last Sweat Score ";
//
uc8 E140[]="Replace earphone board";

//
uc8* Message_English[]=
{
  {E0  ,E1  ,E2  ,E3  ,E4  ,E5  ,E6  ,E7  ,E8  ,E9  ,
   E10 ,E11 ,E12 ,E13 ,E14 ,E15 ,E16 ,E17 ,E18 ,E19 ,
   E20 ,E21 ,E22 ,E23 ,E24 ,E25 ,E26 ,E27 ,E28 ,E29 ,
   E30 ,E31 ,E32 ,E33 ,E34 ,E35 ,E36 ,E37 ,E38 ,E39 ,
   E40 ,E41 ,E42 ,E43 ,E44 ,E45 ,E46 ,E47 ,E48 ,E49 ,
   E50 ,E51 ,E52 ,E53 ,E54 ,E55 ,E56 ,E57 ,E58 ,E59 ,
   E60 ,E61 ,E62 ,E63 ,E64 ,E65 ,E66 ,E67 ,E68 ,E69 ,
   E70 ,E71 ,E72 ,E73 ,E74 ,E75 ,E76 ,E77 ,E78 ,E79 ,
   E80 ,E81 ,E82 ,E83 ,E84 ,E85 ,E86 ,E87 ,E88 ,E89 ,
   E90 ,E91 ,E92 ,E93 ,E94 ,E95 ,E96 ,E97 ,E98 ,E99 ,
   E100,E101,E102,E103,E104,E105,E106,E107,E108,E109,
   E110,E111,E112,E113,E114,E115,E116,E117,E118,E119,
   E120,E121,E122,E123,E124,E125,E126,E127,E128,E129,
   E130,E131,E132,E133,E134,E135,E136,E137,E138,E139,
   E140
  };
*/

/*#################################################################################################
  ###                                  Japanese message                                        ###
  #################################################################################################*/
/*
uc8 J1[]={167,161,214,167,172,175,'-',179,' ',190,175,185,' ',179,201,'-',181,204,167,'"',193,181,212,'-',189,'"',175,204,205,' ',164,171,178,' ',165,161,171,171,178,167,175,'"',170,161,' ',0};
uc8 J2[]={216,217,189,'"',175,204,165,' ',172,162,171,'"',189,'"',175,204,178,'"',' ',198,204,181,204,167,'"',171,'"',165,204,205,' ',181,212,162,199,213,167,' ',171,178,167,175,'"',170,161,' ',0};
uc8 J3[]={216,217,189,'"',175,204,165,' ',172,162,171,'"',189,'"',175,204,178,'"',' ',175,161,171,'"',212,162,205,' ',181,212,162,199,213,167,' ',171,178,167,175,'"',170,161,' ',0};
uc8 J4[]={216,217,189,'"',175,204,165,' ',172,162,171,'"',189,'"',175,204,178,'"',' ',187,'*',202,167,'"',198,192,205,' ',181,212,162,199,213,167,' ',171,178,167,175,'"',170,161,' ',0};
uc8 J5[]={216,217,189,'"',175,204,165,' ',172,162,171,'"',189,'"',175,204,178,'"',' ',183,204,201,161,205,' ',181,212,162,199,213,167,' ',171,178,167,175,'"',170,161,' ',0};
uc8 J6[]={216,217,189,'"',175,204,165,' ',172,162,171,'"',189,'"',175,204,178,'"',' ',194,167,186,213,162,171,204,185,'*',167,172,162,205,' ',181,212,162,199,213,167,' ',171,178,167,175,'"',170,161,' ',0};
uc8 J7[]={216,217,189,'"',175,204,165,' ',172,162,171,'"',189,'"',175,204,178,'"',' ',173,161,188,'"',177,205,' ',181,212,162,199,213,167,' ',171,178,167,175,'"',170,161,' ',0};
uc8 J8[]={216,217,189,'"',175,204,165,' ',172,162,171,'"',189,'"',175,204,178,'"',' ',174,167,179,'"',205,' ',181,212,162,199,213,167,' ',171,178,167,175,'"',170,161,' ',0};
uc8 J9[]={172,175,'-',179,189,'"',175,204,178,'"',179,201,'-',181,204,167,'"',205,' ',185,171,'"',193,190,172,' ',0};
uc8 J10[]={165,161,171,172,186,'*','-',179,'"',185,'4','.','8','k','m','/','h',' ',165,161,171,167,'"',201,'-',179,'"','0','%',178,'"',172,' ',0};
uc8 J11[]={160,180,175,184,' ',170,161,175,'"',161,171,204,185,'*',167,172,162,176,' ',' ',0};
uc8 J12[]={' ',' ',' ',' ',' ',' ',' ',190,181,212,160,200,' ',' ',' ',' ',' ',' ',0};
uc8 J13[]={' ',' ',' ',' ',' ',' ',' ',' ','5','k','m',' ',' ',' ',' ',' ',' ',' ',0};
uc8 J14[]={' ',' ',' ',' ',' ',202,'-',199,204,167,'"',186,200,' ',' ',' ',' ',' ',0};
uc8 J15[]={' ',' ',' ',' ',' ',171,189,'"',162,183,204,171,213,162,' ',' ',' ',' ',0};
uc8 J16[]={' ',' ',' ',' ',' ',' ',171,204,185,'*',167,172,162,' ',' ',' ',' ',' ',0};
uc8 J17[]="      Gerkin      ";
uc8 J18[]="        PEB       ";//{' ',' ',' ',' ',' ',' ',' ',185,'*',200,172,' ',' ',' ',' ',' ',' ',' ',0};
uc8 J19[]={' ',' ',' ',199,167,167,'"',204,179,201,'-',181,204,167,'"',' ',' ',' ',0};
uc8 J20[]={' ',' ',' ',165,161,167,'"',204,179,201,'-',181,204,167,'"',' ',' ',' ',0};
uc8 J21[]={' ',' ',' ',167,162,167,'"',204,179,201,'-',181,204,167,'"',' ',' ',' ',0};
uc8 J22[]={' ',' ',165,161,188,161,175,161,161,204,179,201,'-',181,204,167,'"',' ',0};
uc8 J23[]={' ',' ',' ',175,161,171,'"',212,162,' ',':',' ',' ',' ',' ',' ',' ',' ',0};
uc8 J24[]={' ',' ',' ',' ',' ',201,188,'"',200,' ',':',' ',' ',' ',' ',' ',' ',' ',0};
uc8 J25[]={' ',' ',' ',' ',' ',183,204,201,161,' ',':',' ',' ',' ',' ',' ',' ',' ',0};
uc8 J26[]={' ',' ',' ',' ',173,161,188,'"',177,':',175,'"',204,173,161,' ',' ',' ',0};
uc8 J27[]={' ',' ',' ',' ',173,161,188,'"',177,':',171,'"',213,173,161,' ',' ',' ',0};
uc8 J28[]={194,167,186,213,162,' ',171,204,185,'*',167,172,162,185,'*','-',173,204,179,0};//"THR Percent :     ";
uc8 J29[]={' ',' ',' ',' ',' ',171,'"',165,204,' ',':',' ',' ',' ',' ',' ',' ',' ',0};
uc8 J30[]={' ',' ',' ',' ',' ',174,167,179,'"',' ',':',' ',' ',' ',' ',' ',' ',' ',0};
uc8 J34[]={' ',' ',' ',' ',' ',' ',' ',165,161,171,' ',' ',' ',' ',' ',' ',' ',' ',0};
uc8 J35[]={' ',' ',166,204,166,212,162,178,161,171,' ',171,190,171,175,' ',' ',' ',0};
uc8 J36[]={' ',' ',' ',187,'*',202,167,'"',198,192,171,212,162,199,213,162,' ',' ',0};
uc8 J37[]={187,'*',202,167,'"',198,192,205,' ',170,161,166,179,'"',162,172,200,181,185,' ',171,'"',214,169,162,190,175,' ',185,161,176,171,'"',178,161,171,189,'"',175,204,205,' ',164,171,190,172,' ',0};
uc8 J38[]={170,161,175,'"',161,171,204,185,'*',167,172,162,205,' ',169,163,178,161,190,172,0};
uc8 J39[]={169,'"',162,168,161,171,'"',165,204,' ',':',' ',' ',' ',' ',' ',' ',' ',0};
uc8 J40[]={' ',' ',' ',' ',' ',172,178,'-',171,'"',180,204,185,'"','-',' ',' ',' ',' ',0};//"   Stage Number    ";
uc8 J41[]={' ',' ',' ',' ',' ',' ',183,204,171,213,162,165,202,199,'-',' ',' ',' ',' ',0};
uc8 J42[]={179,201,'-',181,204,167,'"',201,188,'"',200,':',170,161,169,162,' ',0};
uc8 J43[]={179,201,'-',181,204,167,'"',201,188,'"',200,':',' ',175,161,188,204,197,161,' ',0};
uc8 J44[]={' ',179,201,'-',181,204,167,'"',201,188,'"',200,':',' ',197,161,178,'"',172,' ',0};
uc8 J45[]={179,201,'-',181,204,167,'"',201,188,'"',200,':',' ',187,177,162,178,'"',172,' ',0};
uc8 J46[]={179,201,'-',181,204,167,'"',201,188,'"',200,':',' ',165,'"',204,185,'"',199,190,171,213,162,' ',0};
uc8 J47[]={179,201,'-',181,204,167,'"',201,188,'"',200,':',' ',194,214,179,165,'"',204,185,'"',199,190,171,213,162,' ',0};
uc8 J48[]={' ',' ',' ',' ',' ',172,169,160,' ',':',' ',' ',' ',' ',' ',' ',' ',' ',0};
uc8 J49[]={185,'*',187,210,'-',190,204,172,201,188,'"',200,185,' ',175,161,188,204,172,185,'"',198,171,161,178,'"',172,' ',' ',' ','H','I','G','H',' ',0};
uc8 J50[]={185,'*',187,210,'-',190,204,172,201,188,'"',200,185,' ',175,161,188,204,172,185,'"',198,171,161,178,'"',172,' ','M','E','D','I','U','M',' ',0};
uc8 J51[]={185,'*',187,210,'-',190,204,172,201,188,'"',200,185,' ',' ',' ',175,161,188,204,172,185,'"',198,171,161,178,'"',172,' ',' ',' ',' ','L','O','W',' ',0};
uc8 J52[]={185,'*',187,210,'-',190,204,172,201,188,'"',200,185,' ',172,185,'"',198,171,161,178,'"',172,' ','H','I','G','H',' ',0};
uc8 J53[]={185,'*',187,210,'-',190,204,172,201,188,'"',200,185,' ',172,185,'"',198,171,161,178,'"',172,' ','M','E','D','I','U','M',' ',0};
uc8 J54[]={185,'*',187,210,'-',190,204,172,201,188,'"',200,185,' ',172,185,'"',198,171,161,178,'"',172,' ','L','O','W',' ',0};
uc8 J55[]={185,'*',187,210,'-',190,204,172,201,188,'"',200,185,' ',175,161,188,204,197,161,178,'"',172,' ','H','I','G','H',' ',0};
uc8 J56[]={185,'*',187,210,'-',190,204,172,201,188,'"',200,185,' ',175,161,188,204,197,161,178,'"',172,' ','M','E','D','I','U','M',' ',0};
uc8 J57[]={185,'*',187,210,'-',190,204,172,201,188,'"',200,185,' ',175,161,188,204,197,161,178,'"',172,' ','L','O','W',' ',0};
uc8 J58[]={185,'*',187,210,'-',190,204,172,201,188,'"',200,185,' ',197,161,178,'"',172,' ','H','I','G','H',' ',0};
uc8 J59[]={185,'*',187,210,'-',190,204,172,201,188,'"',200,185,' ',197,161,178,'"',172,' ','M','E','D','I','U','M',' ',0};
uc8 J60[]={185,'*',187,210,'-',190,204,172,201,188,'"',200,185,' ',197,161,178,'"',172,' ','L','O','W',' ',0};
uc8 J61[]={169,171,213,162,171,178,161,190,172,0};
uc8 J62[]={216,217,189,'"',175,204,165,' ',172,162,171,'"',189,'"',175,204,178,'"',' ',174,167,179,'"',205,' ',181,212,162,199,213,167,171,178,167,175,'"',170,161,' ',0};
uc8 J63[]={216,217,189,'"',175,204,165,' ',172,162,171,'"',189,'"',175,204,178,'"',' ',168,161,171,211,205,' ',181,212,162,199,213,167,171,178,167,175,'"',170,161,' ',0};
uc8 J64[]={' ',' ',' ',' ',168,161,171,211,' ',':',' ',' ',' ',' ',' ',' ',' ',' ',0};
uc8 J65[]={196,'-',170,'"','-','I','D',205,' ',181,212,162,199,213,167,' ',171,178,167,175,'"',170,161,' ',0};
uc8 J66[]={'I','D',' ',205,' ',181,212,162,199,213,167,176,212,162,' ',0};
uc8 J67[]={'I','D',' ',165,'"',181,204,171,213,162,170,201,190,171,175,' ',0};
uc8 J68[]={'I','D',165,'"',176,165,'"',161,190,172,' ','I','D',205,' ',161,201,180,164,171,178,167,175,'"',170,161,' ',0};
uc8 J69[]={183,214,179,203,'-',167,165,'"',' ',191,177,165,199,190,173,204,' ',0};
uc8 J70[]={164, 190, 176, 167, 175, '"', 170, 161,' ',0};
uc8 J71[]={167,'-',200,175,'"',162,204,0};//"Cool Down";
uc8 J72[]={' ',175,161,199,213,167,' ',178,172,179,205,' ',199,173,214,179,171,190,172,' ',0};//"You must stop this workout before switching to Fitness Test";
uc8 J73[]={171,204,185,'*',167,172,162,205,' ',199,173,214,179,171,190,172,' ',0};//"You must stop this workout before switching to Heart Rate";
uc8 J74[]={' ',' ',161,176,171,'"',178,161,171,' ',':',' ',' ',' ',' ',' ',' ',' ',0};//"   Paused :       ";
uc8 J75[]={' ',' ',' ',' ',' ',169,'"',162,168,161,171,'"',165,204,' ',' ',' ',' ',' ',0};//"    Total Time     ";
uc8 J76[]={' ',' ',' ',' ',' ',' ',' ',' ',' ',166,213,199,' ',' ',' ',' ',' ',' ',' ',0};//"     Distance      ";
uc8 J77[]={' ',' ',' ',' ',' ',' ',' ',' ',188,'*','-',172,' ',' ',' ',' ',' ',' ',' ',0};//"    Average pace   ";
//uc8 J78[]={' ',' ',' ',' ',' ',' ',188,161,166,204,174,167,179,'"',' ',' ',' ',' ',' ',0};//"   Average speed   ";// 无使用
//uc8 J79[]=" Mets  ";// 无使用
uc8 J78[]={178,172,179,171,212,162,199,213,162,0};//"  Test Terminated  ";
uc8 J79[]={171,204,185,'*',167,172,162,165,'"',' ',165,204,176,178,'"',166,190,173,204,' ',173,204,170,'-',205,' ',181,166,'"',214,178,167,175,'"',170,161,' ',0};//"Grasp HR Sensors";
uc8 J80[]=" WATTS ";
uc8 J81[]={171,204,185,'*',167,172,162,205,' ',174,167,178,161,' ',171,178,161,190,172,' ',0};//"Detecting HR ";
uc8 J82[]={170,161,166,179,'"',162,171,178,161,190,172,' ',199,173,214,179,165,'"',' ',165,204,199,213,162,172,200,190,178,'"',' ',164,190,176,167,175,'"',170,161,' ',0};//"Restarting... please wait for safety system to reset";
uc8 J83[]={' ',' ',' ',' ',' ',' ',171,204,185,'*',167,172,162,205,' ',' ',' ',' ',' ',' ',165,204,176,171,190,171,175,' ',0};//"Heart Rate Detected";
//uc8 J84[]={165,161,171,172,186,'*','-',179,'"',185,'4','.','8','k','m','/','h',' ',165,171,161,167,'"',201,'-',179,'"','0','%',178,'"',172,' ',0};
uc8 J85[]={190,181,212,160,200,205,' ',199,173,214,179,171,190,172,' ',0};//"You Must Stop The Current Program Before Starting The Manual Program";
uc8 J86[]={202,'-',199,204,167,'"',186,200,205,' ',199,173,214,179,171,190,172,0};//"You Must Stop The Current Program Before Starting The Rolling Hills Program";
uc8 J87[]={' ',171,189,'"',162,183,204,171,213,162,205,' ',199,173,214,179,171,190,172,' ',0};//"You Must Stop The Current Program Before Starting The Fat Burn Program";
//uc8 J90[]={' ',' ',' ',194,167,186,213,162,' ',171,204,185,'*',167,172,162,' ',' ',' ',0};//" Target Heart Rate ";// 无使用
//uc8 J91[]={' ',' ',' ',' ',171,204,185,'*',167,172,162,255,' ','%',' ',' ',' ',' ',' ',0};//"   Heart Rate %    ";// 无使用
//uc8 J92[]={' ',' ',' ',' ',' ',168,161,171,211,160,214,187,'*',' ',' ',' ',' ',' ','1','0',186,'"',213,162,190,163,' ',0};//"     Incline in    10 sec ";// 无使用
//uc8 J93[]={' ',' ',' ',' ',' ',168,161,171,211,160,214,187,'*',' ',' ',' ',' ',' ','5',186,'"',213,162,190,163,' ',0};//"     Incline in    5  sec ";// 无使用
uc8 J88[]={'5','K',198,204,205,' ',199,173,214,179,171,190,172,' ',0};//"You Must Stop The Current Program Before Starting The 5K Run Program";
//uc8 J95[]={' ',' ',165,202,199,'-',' ',' ',' ',0};// 无使用
//89// 96
uc8 J90[]={' ',216,217,189,'"',175,204,165,' ',172,162,171,'"',189,'"',175,204,178,'"',' ',171,204,176,213,162,205,' ',181,212,162,199,213,167,' ',171,178,167,175,'"',170,161,0};
uc8 J91[]={' ',' ',' ',171,204,176,213,162,' ',':',' ',' ',' ',' ',' ',' ',' ',' ',0};
//92// 99
// 100// 无使用
//uc8 J101[]={' ',168,161,171,211,' ','%',' ',0};// 无使用
uc8 J93[]={' ',175,214,173,161,171,175,' ',194,167,186,213,162,' ',0};
uc8 J94[]={' ',178,204,172,162,' ',0};
uc8 J95[]={' ','U','S','B',181,' ',160,175,198,171,161,174,187,179,165,'"',' ',160,199,190,172,' ','E','N','T','E','R',205,164,171,178,' ',169,162,171,204,171,178,167,175,'"',170,161,'.','.','.',0};
uc8 J96[]={' ',169,162,171,204,172,200,' ',180,190,163,205,' ',173,204,175,167,171,178,167,175,'"',170,161,' ',0};
uc8 J97[]={' ',169,184,174,187,179,162,163,160,185,' ',169,184,169,204,174,'-',200,205,' ',' ',' ',170,189,'*','-',179,171,190,173,204,' ',0};
uc8 J98[]={' ','U','S','B',178,'"',160,199,190,173,204,' ',0};
uc8 J99[]={' ',179,201,'-',181,204,167,'"',165,'"',' ','U','S','B',181,189,174,'"',204,170,201,190,171,175,' ',0};
uc8 J100[]={' ',179,201,'-',181,204,167,'"',184,' ',189,174,'"',204,181,171,214,185,'*',161,171,190,171,175,' ',0};
uc8 J101[]={172,199,'-',187,'*',' ',194,'-',179,'"',181,185,161,200,171,'"',212,204,186,'"',165,'"',' ',178,'"',166,190,171,175,' ',0};
uc8 J102[]={' ',' ',' ','L','C','B',165,'"',166,179,'"',162,171,190,173,204,' ',' ',0}; // Channel
uc8 J103[]={'W','i','F','i',178,'"',' ',169,162,171,204,171,190,172,165,' ',0}; //"WiFi Have New Software, Press Enter to Update}; // Channel
uc8 J104[]={' ',' ',' ',' ',' ',176,211,204,183,200,' ',':',' ',' ',' ',' ',' ',' ',' ',0}; // Channel
uc8 J105[]={' ',' ',' ',189,'"',199,212,'-',192,' ',':',' ',' ',' ',' ',' ',' ',' ',0}; // Volume
uc8 J106[]={' ',' ',' ',' ',' ',' ',' ',191,212,'-',179,' ',' ',' ',' ',' ',' ',' ',0}; //"       Mute       ";"       Mute       ";
uc8 J107[]={174,167,179,'"',' ',190,175,185,168,161,171,211,195,171,'"',200,171,166,'-',205,' ',177,165,214,178,' ',169,204,179,202,'-',200,205,173,204,175,167,' ',0}; 
//108// 117

uc8* Message_Japanese[]=
  {E0  ,J1  ,J2  ,J3  ,J4  ,J5  ,J6  ,J7  ,J8  ,J9  ,
   J10 ,J11 ,J12 ,J13 ,J14 ,J15 ,J16 ,J17 ,J18 ,J19 ,
   J20 ,J21 ,J22 ,J23 ,J24 ,J25 ,J26 ,J27 ,J28 ,J29 ,
   J30 ,E31 ,E32 ,E33 ,J34 ,J35 ,J36 ,J37 ,J38 ,J39 ,
   J40 ,J41 ,J42 ,J43 ,J44 ,J45 ,J46 ,J47 ,J48 ,J49 ,
   J50 ,J51 ,J52 ,J53 ,J54 ,J55 ,J56 ,J57 ,J58 ,J59 ,
   J60 ,J61 ,J62 ,J63 ,J64 ,J65 ,J66 ,J67 ,J68 ,J69 ,
   J70 ,J71 ,J72 ,J73 ,J74 ,J75 ,J76 ,J77 ,J78 ,J79 ,
   J80 ,J81 ,J82 ,J83 ,J10 ,J85 ,J86 ,J87 ,J88 ,E89 ,
   J90 ,J91 ,E92 ,J93 ,J94 ,J95 ,J96 ,J97 ,J98 ,J99 ,
   J100,J101,J102,J103,J104,J105,J106,J107,E108
  };
*/
/*#################################################################################################
  ###                                     German message                                        ###
  #################################################################################################*/
/*
uc8 G1[]="Beginn des Trainings durch Druecken der SCHNELLSTART Taste oder eine der Programmtasten";
uc8 G2[]="Geben Sie die Trainingsdauer ueber die Pfeiltasten oder die Zahlentastatur ein";
uc8 G3[]="Geben Sie Ihr Gewicht ueber die Pfeiltasten oder die Zahlentastatur ein";
uc8 G4[]="Waehlen Sie das Programmlevel ueber die Pfeiltasten oder die Zahlentastatur";
uc8 G5[]="Geben Sie Ihr Alter ueber die Pfeiltasten oder die Zahlentastatur ein";
uc8 G6[]="Waehlen Sie den Zielherzfrequenz - Prozentanteil ueber die Pfeiltasten oder die Zahlentastatur";
uc8 G7[]="Wahl des Geschlechts ueber die Pfeiltasten";
uc8 G8[]="Geben Sie Ihre Geschwindigkeit ueber die Pfeiltasten oder die Zahlentastatur";
uc8 G9[]="Druecken Sie LOS um Ihr Training zu beginnen";
uc8 G10[]="Start - Geschwindigkeit 3.0MPH Start-Anstieg 0%";
uc8 G11[]="Ihre Zielherzfrequenz ist ";
uc8 G12[]="      Manuell     ";
uc8 G13[]="        5KM       ";
uc8 G14[]="       Huegel     ";
uc8 G15[]="  Fettverbrennung ";
uc8 G16[]="    Herzfrequenz  ";
uc8 G17[]="      Gerkin      ";
uc8 G18[]="        PEB       ";
uc8 G19[]="       Army       ";
uc8 G20[]="       Navy       ";
uc8 G21[]="     Airforce     ";
uc8 G22[]="      Marine      ";
uc8 G23[]="Gewicht :         ";
uc8 G24[]="  Level :         ";
uc8 G25[]="  Alter :         ";
uc8 G26[]="     Maennlich    ";//Geschlecht : Maennlich ==?? Gender :   Male
uc8 G27[]="     Weiblich     ";//Geschlecht : Weiblich ==?? Gender :   Female
uc8 G28[]="ZHF Prozent :     ";
uc8 G29[]="   Zeit :         ";
uc8 G30[]="Geschwindigkeit";
//uc8 G31[]="         3        ";
//uc8 G32[]="         2        ";
//uc8 G33[]="         1        ";
uc8 G34[]="       Start      ";
uc8 G35[]="Notfall Stopp Hochdruecken zum Neustart";
uc8 G36[]="   Programm Ende  ";
uc8 G37[]="Druecken Sie LOS oder PAUSE, um das Programm neu zu starten";
uc8 G38[]="Uebersteigt Max Herzfrequenz";
uc8 G39[]="Gesamtzeit :      ";
uc8 G40[]="  Abschnitt Nummer ";
uc8 G41[]="verbrannte Kalorien ";
uc8 G42[]="Leistungsniveau ist HERVORRAGEND";
uc8 G43[]="Leistungsniveau ist SEHR GUT";
uc8 G44[]="Leistungsniveau ist GUT";
uc8 G45[]="Leistungsniveau ist AUSREICHEND";
uc8 G46[]="Leistungsniveau ist SCHLECHT";
uc8 G47[]="Leistungsniveau ist SEHR SCHLECHT";
uc8 G48[]=" Ergebnis :       ";
uc8 G49[]="Leistungsniveau ist HERAUSRAGEND - HOCH";
uc8 G50[]="Leistungsniveau ist HERAUSRAGEND - MITTELMAESSIG";
uc8 G51[]="Leistungsniveau ist HERAUSRAGEND - NIEDRIG";
uc8 G52[]="Leistungsniveau ist EXZELLENT-HOCH";
uc8 G53[]="Leistungsniveau ist EXZELLENT - MITTELMAESSIG";
uc8 G54[]="Leistungsniveau ist EXZELLENT - NIEDRIG";
uc8 G55[]="Leistungsniveau ist GUT-HOCH";
uc8 G56[]="Leistungsniveau ist GUT - MITTELMAESSIG";
uc8 G57[]="Leistungsniveau ist GUT-NIEDRIG";
uc8 G58[]="Leistungsniveau ist ZUFRIEDENSTELLEND - HOCH";
uc8 G59[]="Leistungsniveau ist ZUFRIEDENSTELLEND - MITTELMAESSIG";
uc8 G60[]="Leistungsniveau ist KNAPP - ZUFRIEDENSTELLEND";
uc8 G61[]="Geraet ausser Betrieb";
uc8 G62[]="Eingabe der Geschwindigkeit durch Benutzen der Pfeiltasten oder der Zahlentastatur";
uc8 G63[]="Eingabe der Steigung durch Benutzen der Pfeiltasten oder der Zahlentastatur";
uc8 G64[]="Steigung :        ";
uc8 G65[]="Bitte Benutzer PIN eingeben";
uc8 G66[]="  PIN Verarbeitung ";
uc8 G67[]="   PIN akzeptiert  ";
uc8 G68[]="Falsche PIN,Bitte PIN verbessern";
uc8 G69[]="  Netzwerk fehlt   ";
uc8 G70[]="   Bitte warten    ";
uc8 G71[]="Cool-down";
uc8 G72[]="Fuer Fitnesstest zuruecksetzen";
uc8 G73[]="Fuer Herzfrequenz zuruecksetzen";
uc8 G74[]=" Pausiert :       ";
uc8 G75[]="    Gesamtzeit     ";
uc8 G76[]="    Entfernung     ";
uc8 G77[]=" Tempo ";
//uc8 G78[]="Durchschnitts -geschwindigkeit";// 无使用
//uc8 G79[]=" Mets  ";// 无使用
uc8 G78[]="Test beendet";
uc8 G79[]="Herzfrequenz verloren Sensoren ergreifen";
uc8 G80[]=" WATT  ";
uc8 G81[]="    Findet HF     ";
uc8 G82[]="Neustart...Bitte zur Neueinstellung auf das Sicherheitssystem warten";
uc8 G83[]="Herzfrequenz gefunden";
uc8 G84[]="Start - Geschwindigkeit 4.8 km/h Start-Anstieg 0%";
uc8 G85[]="Fuer Manuell zuruecksetzen";
uc8 G86[]="Fuer Huegel zuruecksetzen";
uc8 G87[]="Fuer Fettverbrennung zuruecksetzen";
//uc8 G90[]="  Zielherzfrequenz ";// 无使用
//uc8 G91[]="  Herzfrequenz %   ";// 无使用
//uc8 G92[]="     Steigung in  10 Sek";// 无使用
//uc8 G93[]="     Steigung in  5 Sek";// 无使用
uc8 G88[]="Fuer 5 KM zuruecksetzen";
//uc8 G95[]=" Kalorien ";// 无使用
//uc8 E89[]="        WFI       ";// 无使用
uc8 G90[]="Geben Sie Ihre Groesse uber die Pfeiltasten oder das Zahlentastenfeld ein";
uc8 G91[]=" Groesse :        ";
//uc8 E92[]=" BMI   ";
//uc8 E100[]=" VO2   ";
//uc8 G101[]="Steigung % ";// 无使用
uc8 G93[]="Ziel erreicht";
uc8 G94[]=" Punkte ";
uc8 G95[]="USB hat neue Software, Enter druecken fur Update...";
uc8 G96[]="Bitte Update-Name waehlen";
uc8 G97[]="Software unterstuetzt diese Konsole nicht";
uc8 G98[]="    Kein USB      ";
uc8 G99[]="TRAINING AUF USB GESPEICHERT";
uc8 G100[]="TRAINING SPEICHERN FEHLGESCHLAGEN";
uc8 G101[]="BEREIT FUR RUHEMODUS";
uc8 G102[]="LCB NICHT AKTIVIERT";
uc8 G103[]="Aktualisierung ueber WiFi gewuenscht";
uc8 G104[]="    Kanal :       ";
uc8 G105[]="Lautstaerke:      ";
uc8 G106[]="       Mute       ";
uc8 G107[]="Steuerung ueber Pfeiltasten fuer Geschwindigkeit oder Steigung auswaehlen";
uc8 G108[]="Eingestellte Steigung maximalen Wert";


uc8* Message_German[]=
  {E0  ,G1  ,G2  ,G3  ,G4  ,G5  ,G6  ,G7  ,G8  ,G9  ,
   G10 ,G11 ,G12 ,G13 ,G14 ,G15 ,G16 ,G17 ,G18 ,G19 ,
   G20 ,G21 ,G22 ,G23 ,G24 ,G25 ,G26 ,G27 ,G28 ,G29 ,
   G30 ,E31 ,E32 ,E33 ,G34 ,G35 ,G36 ,G37 ,G38 ,G39 ,
   G40 ,G41 ,G42 ,G43 ,G44 ,G45 ,G46 ,G47 ,G48 ,G49 ,
   G50 ,G51 ,G52 ,G53 ,G54 ,G55 ,G56 ,G57 ,G58 ,G59 ,
   G60 ,G61 ,G62 ,G63 ,G64 ,G65 ,G66 ,G67 ,G68 ,G69 ,
   G70 ,G71 ,G72 ,G73 ,G74 ,G75 ,G76 ,G77 ,G78 ,G79 ,
   G80 ,G81 ,G82 ,G83 ,G84 ,G85 ,G86 ,G87 ,G88 ,E89 ,
   G90 ,G91 ,E92 ,G93 ,G94 ,G95 ,G96 ,G97 ,G98 ,G99 ,
   G100,G101,G102,G103,G104,G105,G106,G107,G108
  };
*/
/*#################################################################################################
  ###                                     Italian message                                       ###
  #################################################################################################*/
/*
uc8 I1[]="Inizia l`allenamento premendo AVVIO VELOCE o un programma di allenamento";
uc8 I2[]="Inserisci la durata dell`allenamento usando uno dei tasti freccia o i tasti numerici";
uc8 I3[]="Inserisci il tuo peso usando uno dei tasti freccia o i tasti numerici";
uc8 I4[]="Seleziona il livello del programma usando uno dei tasti freccia o i tasti numerici";
uc8 I5[]="Inserisci la tua eta usando uno dei tasti freccia o i tasti numerici";
uc8 I6[]="Seleziona la Percentuale del battito cardiaco target usando uno dei tasti freccia o i tasti numerici";
uc8 I7[]="Seleziona il tuo sesso usando uno dei tasti freccia";
uc8 I8[]="Inserisci la velocita usando uno dei tasti freccia o i tasti numerici";
uc8 I9[]="Premi VIA per iniziare l`allenamento";
uc8 I10[]="Velocita iniziale 4.82 km/h Livello iniziale 0%";
uc8 I11[]="Il tuo battito cardiaco target e ";
uc8 I12[]="     Manuale      ";
uc8 I13[]="        5K        ";
uc8 I14[]="Percorso collinare";
uc8 I15[]="  Brucia grassi   ";
uc8 I16[]=" Battito cardiaco ";
uc8 I17[]="      Gerkin      ";
uc8 I18[]="        PEB       ";
uc8 I19[]="     Esercito     ";
uc8 I20[]=" Marina militare  ";
uc8 I21[]="Aviazione militare";
uc8 I22[]="      Marina      ";
uc8 I23[]="  Peso  :         ";
uc8 I24[]="Livello :         ";
uc8 I25[]="   Eta  :         ";
uc8 I26[]=" Sesso : maschile ";
uc8 I27[]=" Sesso : femminile";
uc8 I28[]="Percentuale battito cardiaco target"; 
uc8 I29[]="  Tempo :         ";
uc8 I30[]=" Velocita :       ";
//uc8 I31[]="         3        ";
//uc8 I32[]="         2        ";
//uc8 I33[]="         1        ";
uc8 I34[]="      Inizio      ";
uc8 I35[]="Stop di emergenza per resettare";
uc8 I36[]="TERMINE programma ";
uc8 I37[]="Premi il tasto VIA O PAUSA per riavviare il programma";
uc8 I38[]="Superamento battito cardiaco massimo";
uc8 I39[]="Tempo totale:     ";
uc8 I40[]="      Stadio       ";
uc8 I41[]=" Calorie bruciate  ";
uc8 I42[]="Il livello della prestazione e ECCEZIONALE";
uc8 I43[]="Il livello della prestazione e ECCELLENTE";
uc8 I44[]="Il livello della prestazione e BUONO";
uc8 I45[]="Il livello della prestazione e DISCRETO";
uc8 I46[]="Il livello della prestazione e SCARSO";
uc8 I47[]="Il livello della prestazione e MOLTO SCARSO";
uc8 I48[]=" Punteggio :      ";
uc8 I49[]="Il livello della prestazione e ECCEZIONALE - ALTO";
uc8 I50[]="Il livello della prestazione e ECCEZIONALE - MEDIO";
uc8 I51[]="Il livello della prestazione e ECCEZIONALE - BASSO";
uc8 I52[]="Il livello della prestazione e ECCELLENTE - ALTO";
uc8 I53[]="Il livello della prestazione e ECCELLENTE - MEDIO";
uc8 I54[]="Il livello della prestazione e ECCELLENTE - BASSO";
uc8 I55[]="Il livello della prestazione e BUONO-ALTO";
uc8 I56[]="Il livello della prestazione e BUONO-MEDIO";
uc8 I57[]="Il livello della prestazione e BUONO-BASSO";
uc8 I58[]="Il livello della prestazione e SODDISFACENTE - ALTO";
uc8 I59[]="Il livello della prestazione e SODDISFACENTE - MEDIO";
uc8 I60[]="Il livello della prestazione e da migliorare";
uc8 I61[]="Fuori servizio";
uc8 I62[]="Inserisci la velocita usando uno dei tasti freccia o i tasti numerici";
uc8 I63[]="Inserisci l`inclinazione usando uno dei tasti freccia o i tasti numerici";
uc8 I64[]=" Inclinazione:    ";
uc8 I65[]=" Inserire ID utente ";
uc8 I66[]="Inserire ID";
uc8 I67[]="ID accettato";
uc8 I68[]="ID non valido, riverificare il proprio ID";
uc8 I69[]="Perdita rete";
uc8 I70[]="Attendere prego";
uc8 I71[]="Defaticamento";
uc8 I72[]="Reimposta per test fitness";
uc8 I73[]="Reimposta per battito cardiaco";
uc8 I74[]="  In pausa:       ";
uc8 I75[]="   Tempo totale    ";
uc8 I76[]="     Distanza      ";
uc8 I77[]=" Andatura ";
//uc8 I78[]="  Velocita media   ";// 无使用
//uc8 I79[]=" Mets  ";// 无使用
uc8 I78[]="Test terminato";
uc8 I79[]="Battito cardiaco non rilevato afferrare i sensori";
uc8 I80[]=" WATT  ";
uc8 I81[]="Rilevamento battito cardiaco";
uc8 I82[]="Riavvio... attendere la reimpostazione del sistema di sicurezza";
uc8 I83[]="Battito cardiaco rilevato";
//uc8 I84[]="VELOCITA` INIZIALE 4.82 km/h  LIVELLO INIZIALE 0%";
uc8 I85[]="Reimposta per manuale";
uc8 I86[]="Reimposta per percorso collinare";
uc8 I87[]="Reimposta per brucia grassi ";
//uc8 I90[]="Battito cardiaco target ";// 无使用
//uc8 I91[]="% battito cardiaco ";// 无使用
//uc8 I92[]=" Inclinazione in 10 sec";// 无使用
//uc8 I93[]="  Inclinazione in 5 sec";// 无使用
uc8 I88[]="Reimposta per corsa 5K";
//uc8 I95[]=" Calorie  ";// 无使用
//uc8 E89[]="        WFI       ";
uc8 I90[]="Inserisci la tua altezza usando uno dei tasti freccia o i tasti numerici";
uc8 I91[]=" Altezza :        ";
//uc8 E92[]=" BMI   ";
//uc8 E100[]=" VO2   ";// 无使用
//uc8 I101[]="  % inclinazione    ";//配合卷动需要加入空格  // 无使用
uc8 I93[]="Obiettivo raggiunto";
uc8 I94[]=" Punti  ";
uc8 I95[]="Nuovo software USB, premere Invio per aggiornare...";
uc8 I96[]="Scegliere il nome dell` aggiornamento";
uc8 I97[]="Software non supportato da questa macchina";
uc8 I98[]="CHIAVE USB NON RILEVATA";
uc8 I99[]="ALLENAMENTO SALVATO NELLA CHIAVE USB";
uc8 I100[]="SALVATAGGIO ALLENAMENTO NON RIUSCITO";
uc8 I101[]="PRONTO PER ENTRARE IN MODALITA SOSPENSIONE";
uc8 I102[]="NESSUNA SVEGLIA LCB";
uc8 I103[]="Aggiornare tramite WiFi";
uc8 I104[]="   Canale :       ";
uc8 I105[]="   Volume :       ";
uc8 I106[]="       Muto       ";
uc8 I107[]="Seleziona controllo usando i tasti freccia velocita o inclinazione";
uc8 I108[]="Impostare valore massimo della inclinazione";

uc8* Message_Italian[]=
  {E0  ,I1  ,I2  ,I3  ,I4  ,I5  ,I6  ,I7  ,I8  ,I9  ,
   I10 ,I11 ,I12 ,I13 ,I14 ,I15 ,I16 ,I17 ,I18 ,I19 ,
   I20 ,I21 ,I22 ,I23 ,I24 ,I25 ,I26 ,I27 ,I28 ,I29 ,
   I30 ,E31 ,E32 ,E33 ,I34 ,I35 ,I36 ,I37 ,I38 ,I39 ,
   I40 ,I41 ,I42 ,I43 ,I44 ,I45 ,I46 ,I47 ,I48 ,I49 ,
   I50 ,I51 ,I52 ,I53 ,I54 ,I55 ,I56 ,I57 ,I58 ,I59 ,
   I60 ,I61 ,I62 ,I63 ,I64 ,I65 ,I66 ,I67 ,I68 ,I69 ,
   I70 ,I71 ,I72 ,I73 ,I74 ,I75 ,I76 ,I77 ,I78 ,I79 ,
   I80 ,I81 ,I82 ,I83 ,I10 ,I85 ,I86 ,I87 ,I88 ,E89 ,
   I90 ,I91 ,E92 ,I93 ,I94 ,I95 ,I96 ,I97 ,I98 ,I99 ,
   I100,I101,I102,I103,I104,I105,I106,I107,I108
  };
*/
/*#################################################################################################
  ###                                    Spanish message                                        ###
  #################################################################################################*/
/*
uc8 S1[]="Presione el boton de INICIO RAPIDO o uno de los botones de entrenamiento para comenzar el entrenamiento";
uc8 S2[]="Introduzca el tiempo de entrenamiento con cualquiera de las flechas o el teclado numerico";
uc8 S3[]="Introduzca el peso con cualquier de las flechas o el teclado numerico";
uc8 S4[]="Elija el nivel de programa con cualquiera de las flechas o el teclado numerico";
uc8 S5[]="Introduzca la edad con cualquiera de las flechas o el teclado numerico";
uc8 S6[]="Elija el porcentaje de ritmo cardiaco objetivo con cualquiera de las flechas o el teclado numerico";
uc8 S7[]="Seleccione el sexo con cualquiera de las flechas";
uc8 S8[]="Introduzca la velocidad con cualquiera de las flechas o el teclado numerico";
uc8 S9[]="Presione IR para comenzar el entrenamiento";
uc8 S10[]="Iniciando velocidad 3.0 MPH Iniciando grado 0%";
uc8 S11[]="  Su ritmo cardiaco objetivo es de  ";//配合卷动需要加入空格
uc8 S12[]="      Manual      ";
uc8 S13[]="       5 km       ";
uc8 S14[]="     Montanas     ";
uc8 S15[]="   Quemar grasa   ";
uc8 S16[]="  Ritmo cardiaco  ";
uc8 S17[]="      Gerkin      ";
uc8 S18[]="        PEB       ";
uc8 S19[]="     Ejercito     ";
uc8 S20[]="      Marina      ";
uc8 S21[]="   Fuerza aerea   ";
uc8 S22[]="Infante de marina ";
uc8 S23[]="   Peso :         ";
uc8 S24[]="  Nivel :         ";
uc8 S25[]="   Edad :         ";
uc8 S26[]=" Sexo : masculino ";
uc8 S27[]=" Sexo : femenino  ";
uc8 S28[]="Porcentaje de ritmo cardiaco objetivo";//"";
uc8 S29[]="  Tiempo :        ";
uc8 S30[]=" Velocidad :      ";
//uc8 S31[]="         3        ";
//uc8 S32[]="         2        ";
//uc8 S33[]="         1        ";
uc8 S34[]="     Comenzar     ";
uc8 S35[]="Levante el boton parada de emergencia para reiniciar";
uc8 S36[]=" FIN del programa ";
uc8 S37[]="Pulse el boton IR/PAUSA para restaurar el programa";
uc8 S38[]="Excediendo ritmo cardiaco maximo";
uc8 S39[]="Tiempo total:     ";
uc8 S40[]="  Numero de etapa  ";
uc8 S41[]="Calorias quemadas  ";
uc8 S42[]="Nivel de rendimiento SUPERIOR";
uc8 S43[]="Nivel de rendimiento EXCELENTE";
uc8 S44[]="Nivel de rendimiento BUENO";
uc8 S45[]="Nivel de rendimiento REGULAR";
uc8 S46[]="Nivel de rendimiento BAJO";
uc8 S47[]="Nivel de rendimiento MUY BAJO";
uc8 S48[]="    Puntaje :     ";
uc8 S49[]="Nivel de rendimiento SOBRESALIENTE- ALTO";
uc8 S50[]="Nivel de rendimiento SOBRESALIENTE- MEDIO";
uc8 S51[]="Nivel de rendimiento SOBRESALIENTE- BAJO";
uc8 S52[]="Nivel de rendimiento EXCELENTE-ALTO";
uc8 S53[]="Nivel de rendimiento EXCELENTE-MEDIO";
uc8 S54[]="Nivel de rendimiento EXCELENTE-BAJO";
uc8 S55[]="Nivel de rendimiento BUENO-ALTO";
uc8 S56[]="Nivel de rendimiento BUENO-MEDIO";
uc8 S57[]="Nivel de rendimiento BUENO-BAJO";
uc8 S58[]="Nivel de rendimiento SATISFACTORIO - ALTO";
uc8 S59[]="Nivel de rendimiento SATISFACTORIO - MEDIO";
uc8 S60[]="El nivel de rendimiento es de prueba";
uc8 S61[]="Fuera de servicio";
uc8 S62[]="Introduzca la velocidad con cualquiera de las flechas o el teclado numerico";
uc8 S63[]="Introduzca la inclinacion con cualquiera de las flechas o el teclado numerico";
uc8 S64[]=" Inclinacion :    ";
uc8 S65[]="Introduzca ID de usuario";
uc8 S66[]="Enviando ID";
uc8 S67[]="ID aceptada";
uc8 S68[]="ID incorrecta. Revise su ID";
uc8 S69[]="Desconectado";
uc8 S70[]="Mantenga";
uc8 S71[]="Enfriamiento";
uc8 S72[]="Restablecer para test en forma";
uc8 S73[]="Restablecer para ritmo cardiaco";
uc8 S74[]=" En pausa :       ";
uc8 S75[]="   Tiempo total    ";
uc8 S76[]="     Distancia     ";
uc8 S77[]=" Ritmo ";
//uc8 S78[]="Velocidad promedio ";// 无使用
//uc8 S79[]=" Mets  ";// 无使用
uc8 S78[]="Prueba finalizada";
uc8 S79[]="Se perdio ritmo cardiaco. Tome los sensores";
uc8 S80[]=" Vatios ";
uc8 S81[]="Detectando ritmo cardiaco";
uc8 S82[]="Reiniciando... Espere que se reinicie el sistema de seguridad";
uc8 S83[]="Ritmo cardiaco detectado";
uc8 S84[]="Iniciando velocidad 4.8 km/h  Iniciando grado 0%";
uc8 S85[]="Restablecer para uso manual";
uc8 S86[]="Restablecer para cuestas rodantes";
uc8 S87[]="Restablecer para quema de grasas";
//uc8 S90[]="Ritmo cardiaco objetivo ";// 无使用
//uc8 S91[]="% de ritmo cardiaco";// 无使用
//uc8 S92[]="  Inclinacion en  10 segundos";// 无使用
//uc8 S93[]="  Inclinacion en  5 segundos";// 无使用
uc8 S88[]="Restablecer para carrera 5K";
//uc8 S95[]=" Calorias ";// 无使用
//uc8 E89[]="        WFI       ";
uc8 S90[]="Introduzca la altura con cualquiera de las flechas o con el teclado numerico";
uc8 S91[]="  Altura :        ";
//uc8 E92[]=" BMI   ";
//uc8 E100[]=" VO2   ";// 无使用
//uc8 S101[]="  % de inclinacion ";//配合卷动需要加入空格 // 无使用
uc8 S93[]="Meta lograda ";
uc8 S94[]=" Puntos ";
uc8 S95[]="Software nuevo en USB. Presione Entrar para actualizar...";
uc8 S96[]="Elija actualizar nombre";
uc8 S97[]="Consola incompatible con software";
uc8 S98[]="     Sin USB      ";
uc8 S99[]="EL ENTRENAMIENTO SE GUARDO EN USB";
uc8 S100[]="ERROR AL GUARDAR ENTRENAMIENTO";
uc8 S101[]="LISTO PARA ENTRAR EN MODO DORMIR";
uc8 S102[]=" LCB NO DESPIERTA ";
uc8 S103[]="Le gustaria actualizar a wifi";
uc8 S104[]="    Canal :       ";
uc8 S105[]="  Volumen :       ";
uc8 S106[]="       Mudo       ";
uc8 S107[]="Seleccionar control con las teclas de direccion de velocidad e inclinacion";
uc8 S108[]="Valor maximo de ajuste de la inclinacion";


uc8* Message_Spanish[]=
  {E0  ,S1  ,S2  ,S3  ,S4  ,S5  ,S6  ,S7  ,S8  ,S9  ,
   S10 ,S11 ,S12 ,S13 ,S14 ,S15 ,S16 ,S17 ,S18 ,S19 ,
   S20 ,S21 ,S22 ,S23 ,S24 ,S25 ,S26 ,S27 ,S28 ,S29 ,
   S30 ,E31 ,E32 ,E33 ,S34 ,S35 ,S36 ,S37 ,S38 ,S39 ,
   S40 ,S41 ,S42 ,S43 ,S44 ,S45 ,S46 ,S47 ,S48 ,S49 ,
   S50 ,S51 ,S52 ,S53 ,S54 ,S55 ,S56 ,S57 ,S58 ,S59 ,
   S60 ,S61 ,S62 ,S63 ,S64 ,S65 ,S66 ,S67 ,S68 ,S69 ,
   S70 ,S71 ,S72 ,S73 ,S74 ,S75 ,S76 ,S77 ,S78 ,S79 ,
   S80 ,S81 ,S82 ,S83 ,S84 ,S85 ,S86 ,S87 ,S98 ,E89 ,
   S90 ,S91 ,E92 ,S93 ,S94 ,S95 ,S96 ,S97 ,S98 ,S99 ,
   S100,S101,S102,S103,S104,S105,S106,S107,S108
  };
*/
/*#################################################################################################
  ###                                      Dutch message                                        ###
  #################################################################################################*/
/*
uc8 D1[]="Start oefening met Snelstart of met de programmatoetsen";
uc8 D2[]="Kies tijd met de pijlen of cijfertoetsen";
uc8 D3[]="Kies gewicht met de pijlen of cijfertoetsen";
uc8 D4[]="Kies programma nivo met de pijlen of cijfertoetsen";
uc8 D5[]="Kies leeftijd met de pijlen of cijfertoetsen";
uc8 D6[]="Kies doel- hartslagpercentage met de pijlen of cijfertoetsen";
uc8 D7[]="Kies geslacht met de pijlen";
uc8 D8[]="Kies snelheid met de pijlen of cijfertoetsen";
uc8 D9[]="Druk op GO om te beginnen";
uc8 D10[]="Startsnelheid is 3.0 MPH Hellingshoek is 0%";
uc8 D11[]="Uw doelhartslag is ";
uc8 D12[]="    Handmatig     ";
uc8 D13[]="        5K        ";
uc8 D14[]="      Heuvels     ";
uc8 D15[]="  Vetverbranding  ";
uc8 D16[]="     Hartslag     ";
uc8 D17[]="      Gerkin      ";
uc8 D18[]="        PEB       ";
uc8 D19[]="      Leger       ";
uc8 D20[]="      Marine      ";
uc8 D21[]="    Luchtmacht    ";
uc8 D22[]="      Marine      ";
uc8 D23[]=" Gewicht :        ";
uc8 D24[]="   Nivo :         ";
uc8 D25[]="Leeftijd :        ";
uc8 D26[]=" Geslacht : Man   ";
uc8 D27[]=" Geslacht : Vrouw ";
uc8 D28[]="Doelhartslag percentage";//" :     ";
uc8 D29[]="   Tijd :         ";
uc8 D30[]="Snelheid :        ";
//uc8 D31[]="         3        ";
//uc8 D32[]="         2        ";
//uc8 D33[]="         1        ";
uc8 D34[]="       Start      ";
uc8 D35[]="Noodstop liften om te resetten";
uc8 D36[]="  Einde programma ";
uc8 D37[]="Druk op de knop Start of Pauze om opnieuw te beginnen";
uc8 D38[]="Maximale hartslag overschreden";
uc8 D39[]="Totale tijd :     ";
uc8 D40[]="   Etappenummer    ";
uc8 D41[]="Verbrande calorieen  ";
uc8 D42[]="Prestatienivo is SUPERIEUR";
uc8 D43[]="Prestatienivo is UITSTEKEND";
uc8 D44[]="Prestatienivo is GOED";
uc8 D45[]="Prestatienivo is REDELIJK";
uc8 D46[]="Prestatienivo is SLECHT";
uc8 D47[]="Prestatienivo is ZEER SLECHT";
uc8 D48[]="    Score :       ";
uc8 D49[]="Prestatienivo is UITSTEKEND-HOOG";
uc8 D50[]="Prestatienivo is UITSTEKEND-MEDIUM";
uc8 D51[]="Prestatienivo is UITSTEKEND-LAAG";
uc8 D52[]="Prestatienivo is EXCELLENT-HOOG";
uc8 D53[]="Prestatienivo is EXCELLENT-MEDIUM";
uc8 D54[]="Prestatienivo is EXCELLENT-LAAG";
uc8 D55[]="Prestatienivo is GOED-HOOG";
uc8 D56[]="Prestatienivo is GOED-MEDIUM";
uc8 D57[]="Prestatienivo is GOED-LAAG";
uc8 D58[]="Prestatienivo is VOLDOENDE-HOOG";
uc8 D59[]="Prestatienivo is VOLDOENDE-MEDIUM";
uc8 D60[]="Prestatienivo is VOLDOENDE - MARGINAAL";
uc8 D61[]="Buiten gebruik";
uc8 D62[]="Kies snelheid met de pijlen of de cijfertoetsen";
uc8 D63[]="Kies hellingshoek met de pijlen of de cijfertoetsen";
uc8 D64[]="Hellingshoek :    ";
uc8 D65[]="Voer gebruikersnaam in";
uc8 D66[]="Gebruikersnaam wordt ingevoerd";
uc8 D67[]="ID geaccepteerd";
uc8 D68[]="Onjuiste ID, controleer aub uw ID";
uc8 D69[]="Netwerkfout";
uc8 D70[]="Vasthouden AUB";
uc8 D71[]="Afkoelen";
uc8 D72[]="Reset voor fitnesstest";
uc8 D73[]="Reset voor hartritme";
uc8 D74[]="    Pauze :       ";
uc8 D75[]="   Totale tijd     ";
uc8 D76[]="     Afstand       ";
uc8 D77[]=" tempo  ";
//uc8 D78[]="Gemiddelde snelheid";//"    Gemiddelde          snelheid     ";// 无使用
//uc8 D79[]=" Mets  ";// 无使用
uc8 D78[]="Test beeindigd";
uc8 D79[]="Hartslag is weggevallen, pak sensoren vast";
uc8 D80[]=" WATTs ";
uc8 D81[]="Detecteren hartslag";
uc8 D82[]="Herstarten... wacht tot dat veiligheidssysteem gereset is";
uc8 D83[]="Hartslag gedetecteerd";
uc8 D84[]="Startsnelheid is 4.8 km/h Hellingshoek is 0%";
uc8 D85[]="Reset voor handmatig";
uc8 D86[]=" Reset voor heuvels";
uc8 D87[]="Reset voor vetverbranding ";
//uc8 D90[]="   Doel Hartslag   ";// 无使用
//uc8 D91[]="    Hartslag %     ";// 无使用
//uc8 D92[]="  Hellingshoek in 10 sec";// 无使用
//uc8 D93[]="  Hellingshoek in 5 sec";// 无使用
uc8 D88[]="Reset voor 5km hardlopen";
//uc8 D95[]="Calorieen ";// 无使用
//uc8 E89[]="        WFI       ";
uc8 D90[]="Voer uw lengte in met behulp van de pijltoetsen of met cijfertoet senpaneel";
uc8 D91[]="  Lengte :        ";
//uc8 E92[]=" BMI   ";
//uc8 E100[]=" VO2   ";// 无使用
//uc8 D101[]="Hellingspercentage ";// 无使用
uc8 D93[]="Doel bereikt";
uc8 D94[]=" Punten ";
uc8 D95[]="USB heeft nieuwe software, druk op Enter voor update...";
uc8 D96[]="Kies naam voor update";
uc8 D97[]="Software ondersteunt deze console niet";
uc8 D98[]="    Geen USB      ";
uc8 D99[]="WORKOUT NAAR USB OPGESLAGEN";
uc8 D100[]="OPSLAAN WORKOUT MISLUKT";
uc8 D101[]="KLAAR VOOR SLAAPSTAND";
uc8 D102[]="  LCB NIET WEKKEN ";
uc8 D103[]="Wilt u via WiFi bijwerken";
uc8 D104[]="   Kanaal :       ";
uc8 D105[]="   Volume :       ";
uc8 D106[]="       Demp       ";
uc8 D107[]="Kies Beheren met behulp van de pijltjes voor snelheid of helling";
uc8 D108[]="Ingestelde helling maximale waarde";


uc8* Message_Dutch[]=
  {E0  ,D1  ,D2  ,D3  ,D4  ,D5  ,D6  ,D7  ,D8  ,D9  ,
   D10 ,D11 ,D12 ,D13 ,D14 ,D15 ,D16 ,D17 ,D18 ,D19 ,
   D20 ,D21 ,D22 ,D23 ,D24 ,D25 ,D26 ,D27 ,D28 ,D29 ,
   D30 ,E31 ,E32 ,E33 ,D34 ,D35 ,D36 ,D37 ,D38 ,D39 ,
   D40 ,D41 ,D42 ,D43 ,D44 ,D45 ,D46 ,D47 ,D48 ,D49 ,
   D50 ,D51 ,D52 ,D53 ,D54 ,D55 ,D56 ,D57 ,D58 ,D59 ,
   D60 ,D61 ,D62 ,D63 ,D64 ,D65 ,D66 ,D67 ,D68 ,D69 ,
   D70 ,D71 ,D72 ,D73 ,D74 ,D75 ,D76 ,D77 ,D78 ,D79 ,
   D80 ,D81 ,D82 ,D83 ,D84 ,D85 ,D86 ,D87 ,D88 ,E89 ,
   D90 ,D91 ,E92 ,D93 ,D94 ,D95 ,D96 ,D97 ,D98 ,D99 ,
   D100,D101,D102,D103,D104,D105,D106,D107,D108
  };
*/

/*#################################################################################################
  ###                                    Swedish message                                        ###
  #################################################################################################*/
/*
uc8 Sw1[]="Starta ditt traningspass genom att trycka pa knappen SNABBSTART eller pa en av traningsknapparna";
uc8 Sw2[]="Ange din traningstid med hjalp av nagon av piltangenterna eller den numeriska knappsatsen";
uc8 Sw3[]="Ange din vikt med hjalp av nagon av piltangenterna eller den numeriska knappsatsen";
uc8 Sw4[]="Valj programniva med hjalp av nagon av piltangenterna eller den numeriska knappsatsen";
uc8 Sw5[]="Ange din alder med hjalp av nagon av piltangenterna eller den numeriska knappsatsen";
uc8 Sw6[]="Valj din procentuella malpuls med hjalp av nagon av piltangenterna eller den numeriska knappsatsen";
uc8 Sw7[]="Valj ditt kon med hjalp av nagon av piltangenterna";
uc8 Sw8[]="Ange din hastighet med hjalp av nagon av piltangenterna eller den numeriska knappsatsen";
uc8 Sw9[]="Tryck pa GA for att starta din traning";
uc8 Sw10[]="Starthastighet 3.0 MPH Startlutning 0 %";
uc8 Sw11[]="  Din malpuls ar   ";
uc8 Sw12[]="     Manuellt     ";
uc8 Sw13[]="       5 km       ";
uc8 Sw14[]="       Kullar     ";
uc8 Sw15[]="  Fettforbranning ";
uc8 Sw16[]="       Puls       ";
uc8 Sw17[]="      Gerkin      ";
uc8 Sw18[]="        PEB       ";
uc8 Sw19[]="      Armen       ";
uc8 Sw20[]="     Flottan      ";
uc8 Sw21[]="    Flygvapnet    ";
uc8 Sw22[]="    Marinkaren    ";
uc8 Sw23[]="   Vikt :         ";
uc8 Sw24[]="   Niva :         ";
uc8 Sw25[]="  Alder :         ";
uc8 Sw26[]="    Kon :  Man    ";
uc8 Sw27[]="    Kon :  Kvinna ";
uc8 Sw28[]="% av malpuls:     ";
uc8 Sw29[]="    Tid :         ";
uc8 Sw30[]="Hastighet:        ";
//uc8 E31[]="         3        ";
//uc8 E32[]="         2        ";
//uc8 E33[]="         1        ";
uc8 Sw34[]="       Borja      ";
uc8 Sw35[]="Lyft nodstoppet for att aterstalla"; 
uc8 Sw36[]="    Programslut   ";
uc8 Sw37[]="Tryck pa knappen GA ELLER PAUS for att starta om programmet";
uc8 Sw38[]="Overskrider maximal puls";
uc8 Sw39[]=" Total tid :      ";
uc8 Sw40[]="   Etappnummer    ";
uc8 Sw41[]="Forbrukade kalorier";
uc8 Sw42[]="Prestandanivan ar OVERLAGSEN";
uc8 Sw43[]="Prestandanivan ar UTMARKT";
uc8 Sw44[]="Prestandanivan ar BRA";
uc8 Sw45[]="Prestandanivan ar RIMLIG";
uc8 Sw46[]="Prestandanivan ar SVAG";
uc8 Sw47[]="Prestandanivan ar MYCKET SVAG";
uc8 Sw48[]="    Betyg :       ";
uc8 Sw49[]="Prestandanivan ar ENASTAENDE-HOG";
uc8 Sw50[]="Prestandanivan ar ENASTAENDE-MEDEL";
uc8 Sw51[]="Prestandanivan ar ENASTAENDE-LAG";
uc8 Sw52[]="Prestandanivan ar UTMARKT-HOG";
uc8 Sw53[]="Prestandanivan ar UTMARKT-MEDEL";
uc8 Sw54[]="Prestandanivan ar UTMARKT-LAG";
uc8 Sw55[]="Prestandanivan ar BRA-HOG";
uc8 Sw56[]="Prestandanivan ar BRA-MEDEL";
uc8 Sw57[]="Prestandanivan ar BRA-LAG";
uc8 Sw58[]="Prestandanivan ar TILLFREDSSTALLANDE - HOG";
uc8 Sw59[]="Prestandanivan ar TILLFREDSSTALLANDE - MEDEL";
uc8 Sw60[]="Prestandanivan ar PROVOKLASS";
uc8 Sw61[]="Fungerar ej";
uc8 Sw62[]="Ange hastigheten med hjalp av nagon av piltangenterna eller den numeriska knappsatsen";
uc8 Sw63[]="Ange lutningen med hjalp av nagon av piltangenterna eller den numeriska knappsatsen";
uc8 Sw64[]=" Lutning :        ";
uc8 Sw65[]="   Ange anvandar-ID";
uc8 Sw66[]="ID skickas in";
uc8 Sw67[]="ID godkant";
uc8 Sw68[]="Ogiltigt ID. Ange ditt ID pa nytt";
uc8 Sw69[]="Natverk saknas";
uc8 Sw70[]="Vanta";
uc8 Sw71[]="Avkylning";
uc8 Sw72[]="Aterstall for konditionstest";
uc8 Sw73[]=" Aterstall for puls";
uc8 Sw74[]="   Pausat :       ";
uc8 Sw75[]=" Total tid ";
uc8 Sw76[]="     Avstand       ";//"     Distance      ";
uc8 Sw77[]="takt ";
//uc8 Sw78[]="Genomsnittlig hastighet";// 无使用
//uc8 Sw79[]=" MET   ";// 无使用
uc8 Sw78[]="Test avslutat";
uc8 Sw79[]="Pulsmatning forlorad - fatta tag i sensorerna";
uc8 Sw80[]=" WATT ";
uc8 Sw81[]=" Detekterar puls  ";
uc8 Sw82[]="Startar om... vanta medan sakerhetssystemet aterstalls";
uc8 Sw83[]="  Puls detekterad  ";
uc8 Sw84[]="Starthastighet 4.8 km/h Startlutning 0 %";
uc8 Sw85[]="Aterstall for manual";
uc8 Sw86[]="Aterstall for kullar";
uc8 Sw87[]="Aterstall for fettforbranning ";
//uc8 Sw90[]=" Malpuls ";// 无使用
//uc8 Sw91[]=" Puls i % ";// 无使用
//uc8 Sw92[]="     Lutning om    10 sek ";// 无使用
//uc8 Sw93[]="     Lutning om    5  sek ";// 无使用
uc8 Sw88[]=" Aterstall for 5 km";
//uc8 Sw95[]=" Kalorier ";// 无使用
//uc8 E89[]="        WFI       ";
uc8 Sw90[]="Ange din langd med hjalp av nagon av piltangenterna eller den numeriska knappsatsen";
uc8 Sw91[]="   Langd :        ";
//uc8 E92[]=" BMI   ";
//uc8 E100[]=" VO2   ";// 无使用
//uc8 Sw101[]="Lutning i % ";// 无使用
uc8 Sw93[]="Mal uppnatt";
uc8 Sw94[]=" Poang ";
uc8 Sw95[]="USB har ny programvara. Tryck pa Enter for att uppdatera...";
uc8 Sw96[]="Valj uppdateringens namn";
uc8 Sw97[]="Programvaran stoder inte denna konsol";
uc8 Sw98[]="   Ingen USB      ";
uc8 Sw99[]="TRANINGSPASS SPARAT PA USB";
uc8 Sw100[]="DET GICK INTE ATT SPARA TRANINGSPASSET";
uc8 Sw101[]="KLAR ATT OVERGA I VILOLAGE";
uc8 Sw102[]="   LCB VAKNAR EJ  ";
uc8 Sw103[]="Vill du uppdatera via WiFi";
uc8 Sw104[]="    Kanal :       ";
uc8 Sw105[]="    Volym :       ";
uc8 Sw106[]="      Dampa       ";
uc8 Sw107[]="Valj kontroll med pilknapparna for hastighet eller lutning";
uc8 Sw108[]="Installt max lutning varde";


uc8* Message_Swedish[]=
  {
  E0   ,Sw1  ,Sw2  ,Sw3  ,Sw4  ,Sw5  ,Sw6  ,Sw7  ,Sw8  ,Sw9  ,
  Sw10 ,Sw11 ,Sw12 ,Sw13 ,Sw14 ,Sw15 ,Sw16 ,Sw17 ,Sw18 ,Sw19 ,
  Sw20 ,Sw21 ,Sw22 ,Sw23 ,Sw24 ,Sw25 ,Sw26 ,Sw27 ,Sw28 ,Sw29 ,
  Sw30 ,E31  ,E32  ,E33  ,Sw34 ,Sw35 ,Sw36 ,Sw37 ,Sw38 ,Sw39 ,
  Sw40 ,Sw41 ,Sw42 ,Sw43 ,Sw44 ,Sw45 ,Sw46 ,Sw47 ,Sw48 ,Sw49 ,
  Sw50 ,Sw51 ,Sw52 ,Sw53 ,Sw54 ,Sw55 ,Sw56 ,Sw57 ,Sw58 ,Sw59 ,
  Sw60 ,Sw61 ,Sw62 ,Sw63 ,Sw64 ,Sw65 ,Sw66 ,Sw67 ,Sw68 ,Sw69 ,
  Sw70 ,Sw71 ,Sw72 ,Sw73 ,Sw74 ,Sw75 ,Sw76 ,Sw77 ,Sw78 ,Sw79 ,
  Sw80 ,Sw81 ,Sw82 ,Sw83 ,Sw84 ,Sw85 ,Sw86 ,Sw87 ,Sw88 ,E89  ,
  Sw90 ,Sw91 ,E92  ,Sw93 ,Sw94 ,Sw95 ,Sw96 ,Sw97 ,Sw98 ,Sw99 ,
  Sw100,Sw101,Sw102,Sw103,Sw104,Sw105,Sw106,Sw107,Sw108
  };
*/

/*#################################################################################################
  ###                                    Finnish message                                        ###
  #################################################################################################*/
/*
uc8 Fi1[]="Aloita harjoitus painamalla QUICK START -painiketta tai jotain harjoitu spainiketta";
uc8 Fi2[]="Anna harjoituksen aika nuolipainikkeilla tai numeron appaimistolla";
uc8 Fi3[]="Anna paino nuolipainikkeilla tai numeron appaimistolla";
uc8 Fi4[]="Valitse ohjelman taso nuolipainikkeilla tai numeron appaimistolla";
uc8 Fi5[]="Anna ika nuolipainikkeilla tai numeron appaimistolla";
uc8 Fi6[]="Valitse tavoitesyken opeusprosentti nuolipainikkeilla tai numeron appaimistolla";
uc8 Fi7[]="Anna sukupuoli nuolipainikkeilla";
uc8 Fi8[]="Anna nopeus nuolipainikkeilla tai numeron appaimistolla";
uc8 Fi9[]="Aloita harjoitus painamalla GO";
uc8 Fi10[]="Alkunopeus 3.0 MPH Alkukaltevuus 0%";
uc8 Fi11[]="Tavoitesykenopeus on ";
uc8 Fi12[]="   Manuaalinen    ";
uc8 Fi13[]="        5K        ";
uc8 Fi14[]="     Kukkulat     ";
uc8 Fi15[]="  Rasvan poltto   ";
uc8 Fi16[]="   Sykenopeus     ";
uc8 Fi17[]="      Gerkin      ";
uc8 Fi18[]="        PEB       ";
uc8 Fi19[]="     Armeija      ";
uc8 Fi20[]="     Laivasto     ";
uc8 Fi21[]="    Ilmavoimat    ";
uc8 Fi22[]="  Merijalkavaki   ";
uc8 Fi23[]="  Paino :         ";
uc8 Fi24[]="   Taso :         ";
uc8 Fi25[]="    Ika :         ";
uc8 Fi26[]="Sukupuoli : Mies  ";
uc8 Fi27[]="Sukupuoli : Nainen";
uc8 Fi28[]="THR-prosentti:    ";
uc8 Fi29[]="   Aika :         ";
uc8 Fi30[]=" Nopeus :         ";
//uc8 E31[]="         3        ";
//uc8 E32[]="         2        ";
//uc8 E33[]="         1        ";
uc8 Fi34[]="      Aloita      ";
uc8 Fi35[]="Hatapysaytys, nollaa nostamalla";
uc8 Fi36[]="  Ohjelman LOPPU  ";
uc8 Fi37[]="Kaynnista ohjelma uudelleen painamalla GO- tai PAUSE-painiketta";
uc8 Fi38[]="Suurin sykenopeus ylitetaan";
uc8 Fi39[]="Kokonaisaika:      ";
uc8 Fi40[]="   Vaiheen numero  ";
uc8 Fi41[]=" Poltetut kalorit  ";
uc8 Fi42[]="Suoritustaso on YLIVOIMAINEN";
uc8 Fi43[]="Suoritustaso on ERINOMAINEN";
uc8 Fi44[]="Suoritustaso on HYVA";
uc8 Fi45[]="Suoritustaso on KOHTUULLINNE";
uc8 Fi46[]="Suoritustaso on HEIKKO";
uc8 Fi47[]="Suoritustaso on HYVIN HEIKKO";
uc8 Fi48[]="    Tulos :       ";
uc8 Fi49[]="Suoritustaso on YLIVOIMAINEN - KORKEA";
uc8 Fi50[]="Suoritustaso on YLIVOIMAINEN - KESKITASO";
uc8 Fi51[]="Suoritustaso on YLIVOIMAINEN - MATALA";
uc8 Fi52[]="Suoritustaso on ERINOMAINEN - KORKEA";
uc8 Fi53[]="Suoritustaso on ERINOMAINEN - KESKITASO";
uc8 Fi54[]="Suoritustaso on ERINOMAINEN - MATALA";
uc8 Fi55[]="Suoritustaso on HYVA - KORKEA";
uc8 Fi56[]="Suoritustaso on HYVA - KESKITASO";
uc8 Fi57[]="Suoritustaso on HYVA - MATALA";
uc8 Fi58[]="Suoritustaso on TYYDYTTAVA - KORKEA";
uc8 Fi59[]="Suoritustaso on TYYDYTTAVA - KESKITASO";
uc8 Fi60[]="Suoritustaso on Kokeellinen";
uc8 Fi61[]="Epakunnossa";
uc8 Fi62[]="Anna nopeus nuolipainikkeilla tai numeron appaimistolla";
uc8 Fi63[]="Anna kaltevuus nuolipainikkeilla tai numeron appaimistolla";
uc8 Fi64[]="   Rinne :        ";
uc8 Fi65[]="Anna kayttajatunnus";
uc8 Fi66[]="Tunnusta lahetetaan";
uc8 Fi67[]="Kayttajatunnus hyvaksytty";
uc8 Fi68[]="Virheellinen tunnus, tarkista se";
uc8 Fi69[]="Verkko ei toimi";
uc8 Fi70[]="Odota";
uc8 Fi71[]="Jaahdyttele";
uc8 Fi72[]="Nollaa kuntotestia varten";
uc8 Fi73[]="Nollaa sykenopeutta varten";
uc8 Fi74[]="Keskeytetty:      ";
uc8 Fi75[]="   Kokonaisaika    ";
uc8 Fi76[]="    Valimatka      ";
uc8 Fi77[]="tahti ";
//uc8 Fi78[]="   Keskinopeus    ";// 无使用
//uc8 Fi79[]=" Metit ";// 无使用
uc8 Fi78[]="Testi paatetty";
uc8 Fi79[]="Sydamen sykenopeuden sensorit irtosivat";
uc8 Fi80[]=" WATTIA ";
uc8 Fi81[]="Sykenopeutta tunnistetaan";
uc8 Fi82[]="Kaynnistetaan uudelleen... odota, kun turvallisuu sjarjestelma nollautuu";
uc8 Fi83[]="Sykenopeus tunnistettu";
uc8 Fi84[]="Alkunopeus 4.8 km/h Alkukaltevuus 0%";
uc8 Fi85[]="Nollaa manuaalista kayttoa varten";
uc8 Fi86[]="Nollaa kukkuloita varten";
uc8 Fi87[]="Nollaa rasvanpolttoa varten ";
//uc8 Fi90[]=" Tavoitesykenopeus ";// 无使用
//uc8 Fi91[]="Sykenopeus % ";// 无使用
//uc8 Fi92[]="   Rinne ajassa    10 s   ";// 无使用
//uc8 Fi93[]="   Rinne ajassa    5  s   ";// 无使用
uc8 Fi88[]="Nollaa 5 km:n juoksua varten";
//uc8 Fi95[]=" Kaloreita ";// 无使用
//uc8 E89[]="        WFI       ";
uc8 Fi90[]="Anna pituus nuolipainikkeilla tai numeron appaimistolla";
uc8 Fi91[]="  Pituus :        ";
//uc8 E92[]=" BMI   ";
//uc8 E100[]=" VO2   ";// 无使用
//uc8 Fi101[]="   Rinne % ";// 无使用
uc8 Fi93[]="Tavoite saavutettu";
uc8 Fi94[]=" Pistetta ";
uc8 Fi95[]="USB:lla on uusi ohjelma, paivita painamalla Enter...";
uc8 Fi96[]="Valitse paivityksen nimi";
uc8 Fi97[]="Tama konsoli ei tue tata ohjelmaa";
uc8 Fi98[]="      EI USB      ";
uc8 Fi99[]="KUNTOILU TALLENNETTU USB:LLE";
uc8 Fi100[]="KUNTOILUN TALLENNUS EPAONNISTUI";
uc8 Fi101[]="VALMIS SIIRTYMAAN LEPOTILAAN";
uc8 Fi102[]="LCB ei toimi, harjoituksen loppu";
uc8 Fi103[]="Haluatko paivittaa WiFi:n kautta";
uc8 Fi104[]="   Kanava :       ";
uc8 Fi105[]="Aanenvoimakkuus:  ";
uc8 Fi106[]="     mykistys     ";
uc8 Fi107[]="Valitse hallinta nopeuden tai kaltevuuden nuolipainikkeilla";
uc8 Fi108[]="Asettaa maksimi kaltevuuden";

uc8* Message_Finnish[]=
{
  E0   ,Fi1  ,Fi2  ,Fi3  ,Fi4  ,Fi5  ,Fi6  ,Fi7  ,Fi8  ,Fi9  ,
  Fi10 ,Fi11 ,Fi12 ,Fi13 ,Fi14 ,Fi15 ,Fi16 ,Fi17 ,Fi18 ,Fi19 ,
  Fi20 ,Fi21 ,Fi22 ,Fi23 ,Fi24 ,Fi25 ,Fi26 ,Fi27 ,Fi28 ,Fi29 ,
  Fi30 ,E31  ,E32  ,E33  ,Fi34 ,Fi35 ,Fi36 ,Fi37 ,Fi38 ,Fi39 ,
  Fi40 ,Fi41 ,Fi42 ,Fi43 ,Fi44 ,Fi45 ,Fi46 ,Fi47 ,Fi48 ,Fi49 ,
  Fi50 ,Fi51 ,Fi52 ,Fi53 ,Fi54 ,Fi55 ,Fi56 ,Fi57 ,Fi58 ,Fi59 ,
  Fi60 ,Fi61 ,Fi62 ,Fi63 ,Fi64 ,Fi65 ,Fi66 ,Fi67 ,Fi68 ,Fi69 ,
  Fi70 ,Fi71 ,Fi72 ,Fi73 ,Fi74 ,Fi75 ,Fi76 ,Fi77 ,Fi78 ,Fi79 ,
  Fi80 ,Fi81 ,Fi82 ,Fi83 ,Fi84 ,Fi85 ,Fi86 ,Fi87 ,Fi88 ,E89  ,
  Fi90 ,Fi91 ,E92  ,Fi93 ,Fi94 ,Fi95 ,Fi96 ,Fi97 ,Fi98 ,Fi99 ,
  Fi100,Fi101,Fi102,Fi103,Fi104,Fi105,Fi106,Fi107,Fi108
  
};
*/
// 2013.05.06
/*#################################################################################################
  ###                                  Portugese message                                        ###
  #################################################################################################*/
/*
#ifdef UCBSoftware_A
    uc8 P1[]="Inicie o exercicio premindo INICIO RAPIDO ou uma das teclas de exercicio";
    uc8 P2[]="SelecCione o tempo de exercicio usando qualquer das teclas de seta ou o teclado numerico";
    uc8 P3[]="Seleccione o seu peso usando qualquer das teclas de seta ou o teclado numerico";
    uc8 P4[]="Seleccione o nivel do programa usando qualquer das teclas de seta ou o teclado numerico";
    uc8 P5[]="Seleccione a sua idade usando qualquer das teclas de seta ou o teclado numercio";
    uc8 P6[]="Seleccione a percentagem Batimento Cardiaco Alvo usando qualquer das teclas de seta ou o teclado numerico";
    uc8 P7[]="Seleccione o seu sexo usando qualquer das teclas de seta";
    uc8 P8[]="Seleccione a velocidade usando qualquer das teclas de seta ou o teclado numerico";
    uc8 P9[]="PRIMA COMECAR PARA INICIAR O EXERCICIO";
    uc8 P10[]="Velocidade inicial 4.8 Km/h Inclinacao Inicial 0%";
    uc8 P11[]=" O seu Batimento      Cardiaco Alvo e ";
    uc8 P12[]="      Manual      ";
    uc8 P13[]="        5K        ";
    uc8 P14[]="     Montanhas    ";
    uc8 P15[]="Queima de Gordura ";
    uc8 P16[]="  Rirmo Cardiaco  ";
    uc8 P17[]="      Gerkin      ";
    uc8 P18[]="        PEB       ";
    uc8 P19[]="     Exercito     ";
    uc8 P20[]="     Marinha      ";
    uc8 P21[]="   Forca Aerea    ";
    uc8 P22[]="    Fuzileiro     ";
    uc8 P23[]="  Peso  :         ";
    uc8 P24[]="  Nivel :         ";
    uc8 P25[]="  Idade :         ";
    uc8 P26[]=" Sexo : Masculino ";
    uc8 P27[]=" Sexo : Feminino  ";
    uc8 P28[]="Bat card alvo%:   ";
    uc8 P29[]="  Tempo :         ";
    uc8 P30[]=" Velocidade :     ";
    //uc8 P31[]="         3        ";
    //uc8 P32[]="         2        ";
    //uc8 P33[]="         1        ";
    uc8 P34[]="      Inicio      ";
    uc8 P35[]="LEVANTE O BOTAOO DE EMERGENCIA PARA REINICIAR";
    uc8 P36[]="  Fim do Programa ";
    uc8 P37[]="Pressione o botao IR OU PAUSAR para reiniciar o programa";
    uc8 P38[]="Ultrapassado batimento cardiaco maximo";
    uc8 P39[]="Tempo Total:      ";
    uc8 P40[]="  Numero do nivel  ";
    uc8 P41[]="Calorias queimadas ";
    uc8 P42[]="Nivel de Performance Superior";
    uc8 P43[]="Nivel de Performance Excelente";
    uc8 P44[]="Nivel de Performance Bom";
    uc8 P45[]="Nivel de Performance Regular";
    uc8 P46[]="Nivel de Performance Fraco";
    uc8 P47[]="Nivel de Performance Muito Fraco";
    uc8 P48[]=" Pontuacao:       ";
    uc8 P49[]="Performance Excepcional - Nivel Alto";
    uc8 P50[]="Performance Excepcional - Nivel Medio";
    uc8 P51[]="Performance Excepcional - Nivel Baixo";
    uc8 P52[]="Performance Excelente - Nivel Alto";
    uc8 P53[]="Performance Excelente - Nivel Medio";
    uc8 P54[]="Performance Excelente - Nivel Baixo";
    uc8 P55[]="Performance Boa - Nivel Alto";
    uc8 P56[]="Performance Boa - Nivel Medio";
    uc8 P57[]="Performance Boa - Nivel Baixo";
    uc8 P58[]="Performance Satisfatoria - Nivel Alto";
    uc8 P59[]="Performance Satisfatoria - Nivel Medio";
    uc8 P60[]="Performance Satisfatoria - Nivel Baixo";
    uc8 P61[]="Fora de Servico";
    uc8 P62[]="Seleccione a velocidade usando qualquer das teclas de seta ou o teclado numerico";
    uc8 P63[]="Seleccione a inclinacao usando qualquer das teclas de seta ou o teclado numerico";
    uc8 P64[]=" Inclinacao :     ";
    uc8 P65[]="Introduza o Codigo ";
    uc8 P66[]="A analisar o Codigo...";
    uc8 P67[]="Codigo aceite";
    uc8 P68[]="Codigo errado, confirme o seu codigo.";
    uc8 P69[]="Erro de comunicacao";
    uc8 P70[]="POR FAVOR,AGUARDE";
    uc8 P71[]="Recuperacao";
    uc8 P72[]="Redefinir para teste de aptidao fisica";
    uc8 P73[]="Redefinir para frequencia cardiaca";
    uc8 P74[]=" EM PAUSA :       ";
    uc8 P75[]="TEMPO TOTAL";
    uc8 P76[]="     DISTANCIA     ";
    uc8 P77[]=" RITMO ";
    //uc8 P78[]="VELOCIDADE MEDIA";// 无使用
    //uc8 P79[]=" METS  ";// 无使用
    uc8 P78[]="TESTE FINALIZADO";
    uc8 P79[]="BATIMENTO CARDIACO NAO DETECTADO. SEGURAR NOS SENSORES";
    uc8 P80[]=" WATTS ";
    uc8 P81[]="A DETECTAR BATIMENTO CARDIACO...";
    uc8 P82[]="A REINICIAR... AGUARDE O REINICIO DO SISTEMA DE SEGURANCA";
    uc8 P83[]="BATIMENTO CARDIACO DETECTADO";
    //uc8 P84[]="Velocidade inicial 4.8 Km/h Inclinacao Inicial 0%";
    uc8 P85[]="Redefinir para manual";
    uc8 P86[]="Redefinir para inclinacao variavel";
    uc8 P87[]="Redefinir para queima de calorias";
    //uc8 P90[]="BATIMENTO CARDIACO ALVO ";// 无使用
    //uc8 P91[]="BATIMENTO CARDIACO % ";// 无使用
    //uc8 P92[]="  INCLINACAO EM    10 SEG";// 无使用
    //uc8 P93[]="  INCLINACAO EM    5 SEG";// 无使用
    uc8 P88[]="Redefinir para corrida de 5 km";
    //uc8 P95[]=" Calorias ";// 无使用
    //uc8 E89[]="        WFI       ";
    uc8 P90[]="Introduza a sua altura utilizando qualquer uma das teclas de seta ou o teclado numerico";
    uc8 P91[]="  Altura :        ";
    //uc8 E92[]=" BMI   ";
    //uc8 E100[]=" VO2   ";// 无使用
    //uc8 P101[]="Inclinacao %"; // 无使用
    uc8 P93[]="Meta alcancada";
    uc8 P94[]=" Pontos ";
    uc8 P95[]="O USB tem novo software, prima Enter para actualizar";
    uc8 P96[]="Escolha o nome da actualizacao";
    uc8 P97[]="O software nao suporta esta consola";
    uc8 P98[]="    Nao e USB     ";
    uc8 P99[]="EXERCICIO GUARDADO NO USB";
    uc8 P100[]="FALHA AO GUARDAR O EXERCICIO";
    uc8 P101[]="PRONTO PARA ENTRAR NO MODO DE HIBERNACAO";
    uc8 P102[]="LCB SEM DESPERTAR";
    uc8 P103[]="Deseja atualizar via WiFi";
    uc8 P104[]="  Estacao :       ";
    uc8 P105[]="   Volume :       ";
    uc8 P106[]="    Silenciar     ";
    uc8 P107[]="Escolha o controle usando as teclas de seta de velocidade ou inclinacao";
    uc8 P108[]="Definir o valor maximo de inclinacao";
    
    uc8* Message_Portugese[]=
      {E0  ,P1  ,P2  ,P3  ,P4  ,P5  ,P6  ,P7  ,P8  ,P9  ,
       P10 ,P11 ,P12 ,P13 ,P14 ,P15 ,P16 ,P17 ,P18 ,P19 ,
       P20 ,P21 ,P22 ,P23 ,P24 ,P25 ,P26 ,P27 ,P28 ,P29 ,
       P30 ,E31 ,E32 ,E33 ,P34 ,P35 ,P36 ,P37 ,P38 ,P39 ,
       P40 ,P41 ,P42 ,P43 ,P44 ,P45 ,P46 ,P47 ,P48 ,P49 ,
       P50 ,P51 ,P52 ,P53 ,P54 ,P55 ,P56 ,P57 ,P58 ,P59 ,
       P60 ,P61 ,P62 ,P63 ,P64 ,P65 ,P66 ,P67 ,P68 ,P69 ,
       P70 ,P71 ,P72 ,P73 ,P74 ,P75 ,P76 ,P77 ,P78 ,P79 ,
       P80 ,P81 ,P82 ,P83 ,P10 ,P85 ,P86 ,P87 ,P88 ,E89 ,
       P90 ,P91 ,E92 ,P93 ,P94 ,P95 ,P96 ,P97 ,P98 ,P99 ,
       P100,P101,P102,P103,P104,P105,P106,P107,P108
      };   */
/*#################################################################################################
  ###                                     French message                                        ###
  #################################################################################################*/
/*    uc8 F1[]="Commencez votre seance d`entrainement en appuyant sur QUICK START ou sur l`un des programmes d`entrainement";
    uc8 F2[]="Entrez la duree de votre seance d`entrainement a l`aide des touches directionnelles ou du clavier numerique";
    uc8 F3[]="Entrez votre poids a l`aide des touches directionnelles ou du clavier numerique";
    uc8 F4[]="Choisissez votre niveau de programme a l`aide des touches directionnelles ou du clavier numerique";
    uc8 F5[]="Entrez votre age a l`aide des touches directionnelles ou du clavier numerique";
    uc8 F6[]="Choisissez votre pourcentage de frequence cardiaque cible a l`aide des touches directionnelles ou du clavier numerique";
    uc8 F7[]="Selectionnez votre sexe a l`aide des touches directionnelles";
    uc8 F8[]="Entrez votre vitesse a l`aide des touches directionnelles ou du clavier numerique";
    uc8 F9[]="Appuyez sur GO pour commencer votre seance d`entrainement";
    uc8 F10[]="Vitesse de demarrage : 4.8 km/h, inclinaison : 0%";
    uc8 F11[]="Votre frequence cardiaque cible est ";
    uc8 F12[]="      Manuel      ";
    uc8 F13[]="   Course 5 km    ";
    uc8 F14[]="Collines ondulantes";
    uc8 F15[]="Brulage des graisses";
    uc8 F16[]="Frequence cardiaque";
    uc8 F17[]="  Test de Gerkin  ";
    uc8 F18[]="        PEB       ";
    uc8 F19[]="  Armee de terre  ";
    uc8 F20[]="      Marine      ";
    uc8 F21[]="  Armee de l`air  ";
    uc8 F22[]="      Marine      ";
    uc8 F23[]="  Poids :         ";
    uc8 F24[]=" Niveau :         ";
    uc8 F25[]="    Age :         ";
    uc8 F26[]="   Sexe : Homme   ";
    uc8 F27[]="   Sexe : Femme   ";
    uc8 F28[]="Pourcentage FC:   ";
    uc8 F29[]="  Temps :         ";
    uc8 F30[]=" Vitesse :        ";
    //uc8 F31[]="         3        ";
    //uc8 F32[]="         2        ";
    //uc8 F33[]="         1        ";
    uc8 F34[]="     Commencer    ";
    uc8 F35[]="Arret d`urgence, soulevez pour reinitialiser";
    uc8 F36[]=" Fin de programme ";
    uc8 F37[]="Appuyez sur GO ou PAUSE pour redemarrer le programme";
    uc8 F38[]="Superieure a la frequence cardiaque maximum";
    uc8 F39[]="Temps total:      ";
    uc8 F40[]=" Numero de niveau  ";
    uc8 F41[]=" Calories brulees  ";
    uc8 F42[]="Le niveau de performance est SUPERIEUR";
    uc8 F43[]="Le niveau de performance est EXCELLENT";
    uc8 F44[]="Le niveau de performance est BON";
    uc8 F45[]="Le niveau de performance est ASSEZ BON";
    uc8 F46[]="Le niveau de performance est FAIBLE";
    uc8 F47[]="Le niveau de performance est TRES FAIBLE";
    uc8 F48[]="    Score :       ";
    uc8 F49[]="Le niveau de performance est EXCEPTIONNEL";
    uc8 F50[]="Le niveau de performance est MOYENNEMENT EXCEPTIONNEL";
    uc8 F51[]="Le niveau de performance est PEU EXCEPTIONNEL";
    uc8 F52[]="Le niveau de performance est EXCELLENT";
    uc8 F53[]="Le niveau de performance est MOYENNENEMENT EXCELLENT";
    uc8 F54[]="Le niveau de performance est A PEINE EXCELLENT";
    uc8 F55[]="Le niveau de performance est TRES BON";
    uc8 F56[]="Le niveau de performance est MOYENNENMENT BON";
    uc8 F57[]="Le niveau de performance est A PEINE BON";
    uc8 F58[]="Le niveau de performance est TRES SATISFAISANT";
    uc8 F59[]="Le niveau de performance est MOYEN";
    uc8 F60[]="Le niveau de performance est probatoire";
    uc8 F61[]="Hors service";
    uc8 F62[]="Entrez la vitesse a l`aide des touches directionnelles ou du clavier numerique";
    uc8 F63[]="Entrez l`inclinaison a l`aide des touches directionnelles ou du clavier numerique";
    uc8 F64[]=" Inclinaison :    ";
    uc8 F65[]="Merci de saisir votre ID utilisateur";
    uc8 F66[]="Validation de l`ID";
    uc8 F67[]="ID accepte";
    uc8 F68[]="ID incorrect, merci de retaper votre ID";
    uc8 F69[]="Reseau perdu";
    uc8 F70[]="Veuillez patienter";
    uc8 F71[]="Recuperation";
    uc8 F72[]="Reinitialisez le mode Test Fitness";
    uc8 F73[]="Reinitialisez le mode Frequence cardiaque";
    uc8 F74[]="    Pause :       ";//Paused  //Faire une pause:
    uc8 F75[]="    Temps total    ";
    uc8 F76[]="     Distance      ";
    uc8 F77[]=" Pas  ";
    //uc8 F78[]="  Vitesse moyenne  ";// 无使用
    //uc8 F79[]=" MET  ";// 无使用
    uc8 F78[]="Test termine";
    uc8 F79[]="Frequence cardiaque perdue - saisissez les capteurs";
    uc8 F80[]=" Watt  ";
    uc8 F81[]="Detection de la FC ";
    uc8 F82[]="Redemarrage... Le systeme de securite se reinitialise, veuillez patienter";
    uc8 F83[]="Frequence cardiaque detectee";
    //uc8 F84[]="Demarrage a partir de 4.8 km/h, inclinaison 0%";
    uc8 F85[]="Reinitialisez le mode Manuel";
    uc8 F86[]="Reinitialisez le mode Collines";
    uc8 F87[]="Reinitialisez le mode Brulage des graisses ";
    //uc8 F90[]="Frequence cardiaque cible  ";// 无使用
    //uc8 F91[]="% de frequence cardiaque ";// 无使用
    //uc8 F92[]="  Inclinaison dans 10 secondes";// 无使用
    //uc8 F93[]="  Inclinaison dans 5 secondes";// 无使用
    uc8 F88[]="Reinitialisez le mode Course 5km";
    //uc8 F95[]=" Calories ";// 无使用
    //uc8 F89[]="        WFI       ";
    uc8 F90[]="Entrez votre hauteur a l`aide des touches directionnelles ou du clavier numerique";
    uc8 F91[]=" Hauteur :        ";
    //uc8 F92[]=" BMI   ";
    //uc8 F100[]=" VO2   ";// 无使用
    //uc8 F101[]="  Inclinaison en  % ";//配合卷动需要加入空格 // 无使用
    uc8 F93[]="Objectif atteint"; 
    uc8 F94[]=" Points ";
    uc8 F95[]="L`USB fonctionne avec un nouveau logiciel, appuyez sur Enter pour le mettre a jour...";
    uc8 F96[]="Merci de selectionner Update Name";
    uc8 F97[]="Le logiciel n`est pas compatible avec cette console";
    uc8 F98[]="    Pas d`USB     ";
    uc8 F99[]="LA SEANCE D`ENTRAINEMENT EST ENREGISTREE SUR USB";
    uc8 F100[]="L`ENREGISTREMENT DE LA SEANCE D`ENTRAINEMENT A ECHOUE";
    uc8 F101[]="PRET A ENTRER EN MODE VEILLE";
    uc8 F102[]="PAS D`ACTIVATION DU LCB";
    uc8 F103[]="Actualisation Wi-Fi";
    uc8 F104[]="   Chaine :       ";
    uc8 F105[]="   Volume :       ";
    uc8 F106[]="     Silence      ";
    uc8 F107[]="Choisissez le mode de controle a l`aide des touches directionnelles pour la vitesse ou l`inclinaison";
    uc8 F108[]="Definir la valeur de la l`inclinaison maximale";
    
    uc8* Message_French[]=
      {E0  ,F1  ,F2  ,F3  ,F4  ,F5  ,F6  ,F7  ,F8  ,F9  ,
       F10 ,F11 ,F12 ,F13 ,F14 ,F15 ,F16 ,F17 ,F18 ,F19 ,
       F20 ,F21 ,F22 ,F23 ,F24 ,F25 ,F26 ,F27 ,F28 ,F29 ,
       F30 ,E31 ,E32 ,E33 ,F34 ,F35 ,F36 ,F37 ,F38 ,F39 ,
       F40 ,F41 ,F42 ,F43 ,F44 ,F45 ,F46 ,F47 ,F48 ,F49 ,
       F50 ,F51 ,F52 ,F53 ,F54 ,F55 ,F56 ,F57 ,F58 ,F59 ,
       F60 ,F61 ,F62 ,F63 ,F64 ,F65 ,F66 ,F67 ,F68 ,F69 ,
       F70 ,F71 ,F72 ,F73 ,F74 ,F75 ,F76 ,F77 ,F78 ,F79 ,
       F80 ,F81 ,F82 ,F83 ,F10 ,F85 ,F86 ,F87 ,F88 ,E89 ,
       F90 ,F91 ,E92 ,F93 ,F94 ,F95 ,F96 ,F97 ,F98 ,F99 ,
       F100,F101,F102,F103,F104,F105,F106,F107,F108
       
      };
#endif  */
/*#################################################################################################
  ###                                    Turkish message                                        ###
  #################################################################################################*/
/*#ifdef UCBSoftware_B

    uc8 Tu1[]="Antremaniniza ``GIT`` tusana basarak vaya herhangibir antreman butonuna basarak baslayabilirsiniz";
    uc8 Tu2[]="Ok tuslarini kullanarak antreman surenizi girin";
    uc8 Tu3[]="Ok tuslarini kullanarak klionuzu girin";
    uc8 Tu4[]="Ok tuslarini kullanarak program seviyenizi belirleyin";
    uc8 Tu5[]="Ok tuslarini kullanarak yasinizi girin";
    uc8 Tu6[]="Ok tuslarini kullanarak hedef kalp atim hizi yuzdenizi secin";
    uc8 Tu7[]="Ok tuslarini kullanarak cinsiyetinizi secin";
    uc8 Tu8[]="Ok tuslarini kullanarak hizinizi secin";
    uc8 Tu9[]="Antremana baslamak icin ``GIT`` tusuna basin";
    uc8 Tu10[]="Baslangic hizi 3.0MPH Baslangic egimi 0%";
    uc8 Tu11[]=" Kalp Atim Hiziniz ";
    uc8 Tu12[]="      Manuel      ";
    uc8 Tu13[]="    5 km Kosu     ";
    uc8 Tu14[]="Inisli/Cikisli Tepeler";
    uc8 Tu15[]="    Yag yakimi    ";
    uc8 Tu16[]="Kalp Atim Hiziniz ";
    uc8 Tu17[]="      Gerkin      ";
    uc8 Tu18[]="        PEB       ";
    uc8 Tu19[]=" Kara Kuvvetleri  ";
    uc8 Tu20[]="     Donanma      ";
    uc8 Tu21[]=" Hava Kuvvetleri  ";
    uc8 Tu22[]=" Piyade Teskilati ";
    uc8 Tu23[]="   Kilo :         ";
    uc8 Tu24[]=" Seviye :         ";
    uc8 Tu25[]="    Yas :         ";
    uc8 Tu26[]="Cinsiyet:  Erkek  ";
    uc8 Tu27[]="Cinsiyet:  Kadin  ";
    uc8 Tu28[]="THR Yuzdesi :     ";
    uc8 Tu29[]="  Zaman :         ";
    uc8 Tu30[]="    Hiz :         ";
    //uc8 Tu31[]="         3        ";
    //uc8 Tu32[]="         2        ";
    //uc8 Tu33[]="         1        ";
    uc8 Tu34[]="       Basla      ";
    uc8 Tu35[]="Sifirlamak icin Acil Durum Buttonu";
    uc8 Tu36[]=" Program Sonlandi ";
    uc8 Tu37[]="Programi yeniden baslatmak icin ``Git`` veya ``Durdurma`` tusuna bas";
    uc8 Tu38[]="Maksimum Kalp Atim Hizi Asildi";
    uc8 Tu39[]="Toplam Zaman:     ";
    uc8 Tu40[]="  Asama Numarasi   ";
    uc8 Tu41[]="  Yakilan Kalori   ";
    uc8 Tu42[]="Performans seviyesi olaganustu";
    uc8 Tu43[]="Performans seviyesi mukemmel";
    uc8 Tu44[]="Performans seviyesi Iyi";
    uc8 Tu45[]="Performans Seviyesi yeterli";
    uc8 Tu46[]="Performans seviyesi Yetersiz";
    uc8 Tu47[]="Performans Seviyesi cok Yetersiz";
    uc8 Tu48[]="    Skore :       ";
    uc8 Tu49[]="Performnce Seviyesi Siradisi-Yuksek";
    uc8 Tu50[]="Performans Seviyesi Siradisi-Orta";
    uc8 Tu51[]="Performans Seviyesi Siradisi-Dusuk";
    uc8 Tu52[]="Performans Seviyesi Mukemmel-Yuksek";
    uc8 Tu53[]="Performnas Seviyesi mukemmel-Orta";
    uc8 Tu54[]="Performans Seviyesi Mukemmel-Dusuk";
    uc8 Tu55[]="Performans Seviyesi Iyi-Yuksek";
    uc8 Tu56[]="Performans Seviyesi Iyi-Orta";
    uc8 Tu57[]="Performans Seviyesi Iyi-Dusuk";
    uc8 Tu58[]="Performans Seviyesi Katdadeger-Yuksek";
    uc8 Tu59[]="Performans Seviyesi Katdadeger-Orta";
    uc8 Tu60[]="Performans Seviyesi Kaydadeger-Dusuk";
    uc8 Tu61[]="Servid Disi";
    uc8 Tu62[]="Tustakimini veya Ok Tuslarini kullanarak Hizinizi Girin";
    uc8 Tu63[]="Tustakimini veya Ok Tuslarini kullanarak Egimi Girin";
    uc8 Tu64[]="    Egim :        ";
    uc8 Tu65[]="Lutfen Kullanici ID nizi Girin";
    uc8 Tu66[]="ID onay";
    uc8 Tu67[]="ID Kabul Edildi";
    uc8 Tu68[]="Zayif ID,Lutfen yeni bir ID Girin";
    uc8 Tu69[]="Sbeke bulunnamiyor ";
    uc8 Tu70[]="Lutfen Bekleyin";
    uc8 Tu71[]="Sogutma";
    uc8 Tu72[]="Fitness Test Sifirla";
    uc8 Tu73[]="Kalp Atim Hizi Sifirla";
    uc8 Tu74[]="Duraklatildi:     ";
    uc8 Tu75[]="Toplam Zaman ";
    uc8 Tu76[]="Mesafe ";
    uc8 Tu77[]="Ortalama Tempo ";
    //uc8 Tu78[]="Ortalama Hiz ";// 无使用
    //uc8 Tu79[]=" MET  ";// 无使用
    uc8 Tu78[]=" Test Sonlandirildi";
    uc8 Tu79[]="Nabiz Sensoru Kalp Atim Hizini Hissedemedi";
    uc8 Tu80[]=" Vatlar ";
    uc8 Tu81[]="    HR sensoru    ";
    uc8 Tu82[]="Tekrar baslatililyor... Korumali sistemin sifirlanmasi icin lutfen bekleyin";
    uc8 Tu83[]="Kalp Atim Hizi Algilandi";
    uc8 Tu84[]="Baslangic hizi4.8Km/h Baslangis Seviyesi 0%";
    uc8 Tu85[]="Manuel Sifirla";
    uc8 Tu86[]="Tepeler Sifirla";
    uc8 Tu87[]=" Yag yakimi Sifirla";
    //uc8 Tu90[]="Hedef HR ";// 无使用
    //uc8 Tu91[]="Kalp Atim Hizi % ";// 无使用
    //uc8 Tu92[]="Egim 10sn";// 无使用
    //uc8 Tu93[]="Egim 5sn";// 无使用
    uc8 Tu88[]="5K Kosusunu Sifirla";
    //uc8 Tu95[]=" Kaloriler ";// 无使用
    //uc8 Tu89[]="        WFI       ";
    uc8 Tu90[]="Tustakimini veya ok tuslarini kulllanarak boyunuzu Girin";
    uc8 Tu91[]="     Boy :        ";
    //uc8 Tu92[]=" BMI   ";
    //uc8 Tu100[]=" VO2   ";// 无使用
    //uc8 Tu101[]=" Egim % ";// 无使用
    uc8 Tu93[]="Hedefe Ulasildi";
    uc8 Tu94[]=" Puanlar ";
    uc8 Tu95[]="USB yeni bir Software e sahip.Guncellemek icin lutfen `Git` tusuna basin";
    uc8 Tu96[]="Lutfen guncel ismi secin";
    uc8 Tu97[]="Software bu konsolu desteklemiyor";
    uc8 Tu98[]="     USB yok      ";
    uc8 Tu99[]="Antreman USB ye kayit edildi";
    uc8 Tu100[]="Antreman kayit edilemed";
    uc8 Tu101[]="Uyku modu Gir";
    uc8 Tu102[]="LCB harekete Gecmedi";
    uc8 Tu103[]="WiFi vaistasi ile konsola software guncellenme";
    uc8 Tu104[]="    Kanal :       ";
    uc8 Tu105[]="      Ses :       ";
    uc8 Tu106[]="      Sessiz      ";
    uc8 Tu107[]="Hiz Kullanim kontrolunu veya egim ok tuslarini sec";
    uc8 Tu108[]="Minimum Egim degeri ayarla";
    
    uc8* Message_Turkish[]=
      {E0   ,Tu1  ,Tu2  ,Tu3  ,Tu4  ,Tu5  ,Tu6  ,Tu7  ,Tu8  ,Tu9  ,
       Tu10 ,Tu11 ,Tu12 ,Tu13 ,Tu14 ,Tu15 ,Tu16 ,Tu17 ,Tu18 ,Tu19 ,
       Tu20 ,Tu21 ,Tu22 ,Tu23 ,Tu24 ,Tu25 ,Tu26 ,Tu27 ,Tu28 ,Tu29 ,
       Tu30 ,E31  ,E32  ,E33  ,Tu34 ,Tu35 ,Tu36 ,Tu37 ,Tu38 ,Tu39 ,
       Tu40 ,Tu41 ,Tu42 ,Tu43 ,Tu44 ,Tu45 ,Tu46 ,Tu47 ,Tu48 ,Tu49 ,
       Tu50 ,Tu51 ,Tu52 ,Tu53 ,Tu54 ,Tu55 ,Tu56 ,Tu57 ,Tu58 ,Tu59 ,
       Tu60 ,Tu61 ,Tu62 ,Tu63 ,Tu64 ,Tu65 ,Tu66 ,Tu67 ,Tu68 ,Tu69 ,
       Tu70 ,Tu71 ,Tu72 ,Tu73 ,Tu74 ,Tu75 ,Tu76 ,Tu77 ,Tu78 ,Tu79 ,
       Tu80 ,Tu81 ,Tu82 ,Tu83 ,Tu84 ,Tu85 ,Tu86 ,Tu87 ,Tu88 ,E89  ,
       Tu90 ,Tu91 ,E92  ,Tu93 ,Tu94 ,Tu95 ,Tu96 ,Tu97 ,Tu98 ,Tu99 ,
       Tu100,Tu101,Tu102,Tu103,Tu104,Tu105,Tu106,Tu107,Tu108
       
      };
#endif
//
*/

    
