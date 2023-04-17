#ifndef __type_a_h
#define __type_a_h
#include "include.h"

extern uint8_t   picc_atqa[2],picc_uid[15],picc_sak[3];

extern uint8_t  TypeA_Request(uint8_t  *pTagType);
extern uint8_t  TypeA_WakeUp(uint8_t  *pTagType);
extern uint8_t  TypeA_Anticollision(uint8_t  selcode,uint8_t  *pSnr);
extern uint8_t  TypeA_Select(uint8_t  selcode,uint8_t  *pSnr,uint8_t  *pSak);
extern uint8_t  TypeA_Halt(uint8_t  AnticollisionFlag);
extern uint8_t  TypeA_CardActive(uint8_t  *pTagType,uint8_t  *pSnr,uint8_t  *pSak);
uint8_t Add_TyteA_Card (void);
uint8_t Delete_Card (uint8_t id);
uint8_t TyteA_Comparation (void);

#endif
