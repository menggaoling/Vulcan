#ifndef _RF900_H_
#define _RF900_H_

#define _MaxVo 16

void RF900_Key(unsigned int by_Dat);
void RF900_MessageHoldTime(void);
char RF900_ShowMessage(void);
void RF900_ClearMessageNumber(void);
void RF900_DefaultChannel(char by_D1);
char RF900_Mute(void);
char RF900_MaxChannel(void);
void RF900_Initial(void);
void RF900_MuteClear(void);
void RF900_SetCommand(char by_Com,char by_Dat);

#endif // _RF900_H_
