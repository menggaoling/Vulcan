#ifndef __GLOBAL_VAR_H
#define __GLOBAL_VAR_H

#ifdef __GLOBAL_VAR
#define VAR_TYPE 
#else
#define VAR_TYPE extern
#endif /*__GLOBAL_VAR */

/*********declare global var here*******/
VAR_TYPE char SpText[64];	//for debug sprinf() use	
VAR_TYPE char DisplayBuf[20];



/*********end of declare global var*****/
#endif /* __GLOBAL_VAR_H */

