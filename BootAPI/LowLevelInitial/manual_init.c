/******************************************************************************
.................... MATRIX A1/A3 software history ............................ 
.. S001-beta-01 ..
.. 99.03.29 ..



*******************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include  <stdio.h>
#include  <stdarg.h>
#include "stm32f2xx_hal.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Declare the sections. */
/*
#pragma section = ".special"
#pragma section = ".special_init"
void UCB_memoryInitial(void) 
{
  uint32_t size;
  uint32_t *targetAddr = __section_begin(".special");
  uint32_t const *sourceAddr = __section_begin(".special_init");
  uint32_t const *sourceAddrEnd = __section_end(".special_init");
  size = sourceAddrEnd - sourceAddr;
  do
  {
    *targetAddr++ = *sourceAddr++;
    size -= 1;
  } while (size != 0);
  return ;
}


#pragma section = ".bss"
void UCB_memoryInitialZero(void) 
{
  uint32_t size;
  uint32_t *targetAddr = __section_begin(".bss");
  uint32_t *targetAddrEnd = __section_end(".bss");
  size = targetAddrEnd - targetAddr;
  do
  {
    *targetAddr++ = 0 ;
    size -= 1;
  } while (size != 0);

  return ;
}
*/
//typedef uint32_t const * init_fun_t(uint32_t const *);
//
//void __manual_data_init(void)
//{
//  uint32_t const * p = (uint32_t const *)0x0803F000 ;
//  while( *p != 0 && *p != 0xFFFFFFFF )
//      {
//      init_fun_t * fun = (init_fun_t *)*p++;
//      p = fun(p) ;
//      }
//  return ;
//}  
// The type of a pointer into the init table.
typedef void const * table_ptr_t;

// The type of an initialization routine. It takes a pointer to the start of
// its entry (after the function pointer) in the init table and returns a
// pointer to after its entry.
typedef table_ptr_t init_fun_t( table_ptr_t );

typedef struct
{
  int32_t mOff;
} FAddr;
#pragma section = "Region$$Table"
void __manual_data_init(void)//Because the function initial is difference from iar5.4,so this section need modify GarryMeng
{
  FAddr const * pi = __section_begin("Region$$Table");
  table_ptr_t pe = __section_end ("Region$$Table");
  while ( pi != pe )
  {
    init_fun_t * fun = (init_fun_t *) ( (uint32_t) pi + pi->mOff );
    ++pi;
    pi = fun( pi );
  }
}  
//------------------------------------------------------------------------------

