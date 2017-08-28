#include "FLASH.h"
/*****************************************************************************/
uint8 FlashMem[32] =
{
    0xA6,0x10,0xC7,0x18,0x25,0xA6,0x00,0xC7,0xFD,0x00,0xA6,0x20,0xC7,0x18,0x26,0xA6,
    0xC0,0xC7,0x18,0x25,0x9D,0x9D,0x9D,0x9D,0xC6,0x18,0x25,0xA5,0x40,0x27,0xF9,0x81
};  //A6   10   45   18   25   F7   A6   00   C7   08   30   A6   20   C7   18   26
    //A6   C0   F7   9D   9D   9D   9D   C6   18   25   A5   40   27   F9   81

/*****************************************************************************/
#define  FlashData       FlashMem[6]                             //  �����ֽ�,��:0x00
#define  FlashAddr       (*((uint16 *)(&FlashMem[8])) )          //  ���ݵ�ַ,��:0xFD00
#define  FlashCmd       FlashMem[11]                             //  ����,     ��:0x20
#define  RunFlashCmd    ((void (*)(void))((uint16)FlashMem))     // �������
/*****************************************************************************/
/***  �˺�����Ҫ�жϱ���:                                                   ***/
/*****************************************************************************/
void  EraseFlash(uint16 Base_Flash)
{
    __RESET_WATCHDOG();
    FlashAddr  =  Base_Flash;     // Flash��ַ
    FlashCmd  =  0x40;            // ����ɾ������: 0x40
    RunFlashCmd();             
}
/*****************************************************************************/
/***  �˺�����Ҫ�жϱ���:                                                   ***/
/*****************************************************************************/
void  SaveToFlash(uint16 Base_Flash, uint8 *pData, uint16 SaveBytes)
{
  //  Assert( SaveBytes>0 );
    while( SaveBytes-- )
    {
        __RESET_WATCHDOG();
        FlashData  =  *pData++;         // ��������
        FlashAddr  =  Base_Flash++;     //  ���µ�ַ
        FlashCmd  =  mByteProg;        //  �ֽڱ������: 0x20
        RunFlashCmd();                 
    }
}
/******************************** End of file ********************************/