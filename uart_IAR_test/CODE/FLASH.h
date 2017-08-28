#ifndef __FLASH_H
#define __FLASH_H
#include "Events.h"

typedef  unsigned  char       uint8;
typedef  unsigned  short      uint16;
 
void     EraseFlash(  uint16 Base_Flash);
void     SaveToFlash(uint16 Base_Flash, uint8 *pData, uint16 SaveBytes);
 
#endif 
