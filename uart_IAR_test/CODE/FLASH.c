#include "FLASH.h"
/*****************************************************************************/
uint8 FlashMem[32] =
{
    0xA6,0x10,0xC7,0x18,0x25,0xA6,0x00,0xC7,0xFD,0x00,0xA6,0x20,0xC7,0x18,0x26,0xA6,
    0xC0,0xC7,0x18,0x25,0x9D,0x9D,0x9D,0x9D,0xC6,0x18,0x25,0xA5,0x40,0x27,0xF9,0x81
};  //A6   10   45   18   25   F7   A6   00   C7   08   30   A6   20   C7   18   26
    //A6   C0   F7   9D   9D   9D   9D   C6   18   25   A5   40   27   F9   81

/*****************************************************************************/
#define  FlashData       FlashMem[6]                             //  数据字节,如:0x00
#define  FlashAddr       (*((uint16 *)(&FlashMem[8])) )          //  数据地址,如:0xFD00
#define  FlashCmd       FlashMem[11]                             //  命令,     如:0x20
#define  RunFlashCmd    ((void (*)(void))((uint16)FlashMem))     // 函数入口
/*****************************************************************************/
/***  此函数需要中断保护:                                                   ***/
/*****************************************************************************/
void  EraseFlash(uint16 Base_Flash)
{
    __RESET_WATCHDOG();
    FlashAddr  =  Base_Flash;     // Flash地址
    FlashCmd  =  0x40;            // 扇区删除命令: 0x40
    RunFlashCmd();             
}
/*****************************************************************************/
/***  此函数需要中断保护:                                                   ***/
/*****************************************************************************/
void  SaveToFlash(uint16 Base_Flash, uint8 *pData, uint16 SaveBytes)
{
  //  Assert( SaveBytes>0 );
    while( SaveBytes-- )
    {
        __RESET_WATCHDOG();
        FlashData  =  *pData++;         // 更新数据
        FlashAddr  =  Base_Flash++;     //  更新地址
        FlashCmd  =  mByteProg;        //  字节编程命令: 0x20
        RunFlashCmd();                 
    }
}
/******************************** End of file ********************************/