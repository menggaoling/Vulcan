#ifndef _keyFunctionInfor_H_
#define _keyFunctionInfor_H_

#include "keyboard.h"

#define _KeyScanNumber 8   // 扫瞄PIN数 (最大16PIN)
#define _KeyDataNumber 8  // 资料PIN数 (最大16PIN)

/*##############################################################################    
  ####                        8x8    按键代码对应表                        #####
  ##############################################################################*/
/*const unsigned char _KeyTable[_KeyScanNumber][_KeyDataNumber]=
    {// BIT0     BIT1        BIT2         BIT3          BIT4      BIT5      BIT6       BIT7      
    {K_InclineD ,K_MANUAL    ,K_TargetHR ,0            ,0        ,K_Mute    ,K_CC      ,K_TV_VU  }, //Key Scan1          
    {K_InclineU ,K_Training  ,K_T1       ,K_T2         ,K_T3     ,K_TV_VD   ,K_CoolDown,K_GO     }, //Key Scan2 
    {K_TimeD    ,K_TimeU     ,0          ,0            ,0        ,K_STOP    ,K_TV_CU   ,K_TV_LAST}, //Key Scan3    
    {K_1        ,K_2         ,K_3        ,K_FitnessTest,K_T4     ,K_TV_CD   ,K_TV_POWER,0        }, //Key Scan4     
    {K_4        ,K_5         ,K_6        ,K_FatBurn    ,K_Rolling,0         ,0         ,0        }, //Key Scan5      
    {K_7        ,K_8         ,K_9        ,0            ,K_PAUSE  ,0         ,0         ,0        }, //Key Scan6
    {K_BACK     ,K_0         ,K_ENTER    ,K_SpeedU     ,K_SpeedD ,0         ,0         ,0        }, //Key Scan7    
    {0          ,0           ,0          ,0            ,0        ,0         ,0         ,0        }  //Key Scan8
    };    */
//const unsigned char _KeyTable[_KeyScanNumber][_KeyDataNumber]=
//    {// DA8        DA7         DA6           DA5           DA4           DA3          DA2         DA1
//    {0            ,0          ,0            ,0            ,0            ,0           ,0          ,0         }, //Key Scan8
//    {0            ,0          ,0            ,K_SpeedD     ,K_SpeedU     ,K_ENTER     ,K_0        ,K_BACK    }, //Key Scan7
//    {0            ,0          ,0            ,K_PAUSE      ,0            ,K_9         ,K_8        ,K_7       }, //Key Scan6
//    {0            ,0          ,0            ,0            ,K_T2         ,K_6         ,K_5        ,K_4       }, //Key Scan5
//    {0            ,K_TV_POWER ,K_TV_CD      ,0            ,K_FitnessTest,K_3         ,K_2        ,K_1       }, //Key Scan4
//    {K_TV_LAST    ,K_TV_CU    ,K_STOP       ,0            ,K_Rolling    ,K_FatBurn   ,K_TimeU    ,K_TimeD   }, //Key Scan3
//    {K_GO         ,K_CoolDown ,K_TV_VD      ,0            ,0            ,K_T1        ,K_Training ,K_InclineU}, //Key Scan2
//    {K_TV_VU      ,K_CC       ,K_Mute       ,0            ,0            ,K_TargetHR  ,K_MANUAL   ,K_InclineD}  //Key Scan1
//    };
/*      
    {K_3              ,K_6        ,K_9              ,K_ENTER       ,K_FAN      ,0        ,0          ,0              ,0 ,0 ,K_QuickGO ,K_QuickSTOP ,K_CoolDown}, //Key Scan8
    {K_5              ,K_8        ,K_0              ,K_Language    ,K_TV_ENTER ,0        ,K_TV_CD    ,K_TV_POWER     ,0,0,0,0,0 }, //Key Scan7
    {K_1              ,K_4        ,K_7              ,K_BACK        ,0          ,0        ,K_TV_7     ,K_TV_0         ,0,0,0,0,0 }, //Key Scan6
    {K_ToggleDisplay3 ,K_2        ,K_ToggleDisplay4 ,K_STOP        ,0          ,0        ,K_TV_8     ,K_TV_9         ,0,0,0,0,0 }, //Key Scan5
    {K_TimeU          ,K_5K       ,K_Sprint8        ,K_FitnessTest ,0          ,K_TV_5   ,K_TV_6     ,K_TV_SUB       ,0,0,0,0,0 }, //Key Scan4
    {K_TimeD          ,K_MANUAL   ,K_Training       ,K_TargetHR    ,0          ,K_TV_4   ,K_TV_CU    ,K_TV_VD        ,0,0,0,0,0 }, //Key Scan3
    {K_SpeedU         ,K_SpeedD   ,K_ToggleDisplay2 ,K_GO          ,K_TV_2     ,K_TV_3   ,K_Mute     ,K_ClosedCaption,K_QuickSpeedU  ,K_QuickSpeedD  ,0,0,0}, //Key Scan2
    {K_InclineU       ,K_InclineD ,K_ToggleDisplay1 ,K_PAUSE       ,K_TV_1     ,K_TV_VU  ,K_TV_LAST  ,K_Home         ,K_QuickInclineU,K_QuickInclineD,0,0,0}  //Key Scan1
    };     
*/

const unsigned char _KeyTable[]=
{
  0,
  K_SpeedU,
  0,
  K_SpeedD,
  K_ENTER,
  0,
  0,
  K_3,
  K_6,
  K_9,
  K_0,
  0,
  0,
  K_2,
  K_5,
  K_8,
  K_BACK,
  0,
  0,
  K_1,
  K_4,
  K_7,
  0,
  K_STOP,
  0,
  0,
  0,
  0,
  0,
  K_GO,
  K_InclineD,   
  K_InclineU,
  0,
  0,
  0,
  K_CoolDown,

};

/*##############################################################################    
  ####                        特殊复合按键代码对应表                       #####
  #### 前面3个BYTE为特殊组合键对应码用,最后一个BYTE为特殊键代码            #####
  ##############################################################################*/
#define _SpecialKeyGroup 2      // 特殊复合按键组数
#define _SpecialKeyInforNumber 4 // 特殊复合按键内的数据数 (当需要用到2个组合键以上时需要注意为不同Data Pin脚)

 const unsigned char _SpecialKey[_SpecialKeyGroup][_SpecialKeyInforNumber]=
    {
      {K_SpeedD , K_InclineD , 0 , K_IDSD},
      {K_SpeedD , K_InclineD , 0 , K_IDSD}
    };

/*##############################################################################    
  ####                          HOLD按键代码对应表                         #####
  #### 各按键触发时间对应表                                                #####
  ###  HoldKeyTime  范例说明 :                                               ###
  ###      1 => 表示正常加速条件                                             ###
  ###   3000 => 表示按住3秒成立条件)                                         ### 
  ##############################################################################*/
#define _HoldKeyTimeGroup 4          // Hold key delay time组数
#define _HoldKeyNumber 5             // 每组 Hold key 数 

 const unsigned short _HoldKeyTime[_HoldKeyTimeGroup]={1,1,3000,3000};
 const unsigned char _HoldKey[_HoldKeyTimeGroup][_HoldKeyNumber]=
    {
    {K_InclineU,K_InclineD, K_SpeedU  ,K_SpeedD   ,K_TimeU    }, 
    {K_TimeD   ,K_TV_CU   , K_TV_CD   ,K_TV_VU    ,K_TV_VD    },
    {K_T1      ,K_T2      , K_T3      ,K_T4       ,K_IDSD     },
    {K_MANUAL  ,K_STOP    ,          0,          0,          0}
    };
 
#endif // _keyFunctionInfor_H_





