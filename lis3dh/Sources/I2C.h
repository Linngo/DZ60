#ifndef I2C_H
#define I2C_H
#include <hidef.h> /* for EnableInterrupts macro */
#include "Events.h" /* include peripheral declarations */

void DogHello();

void Start_I2c();
void Stop_I2c();
void SendByte(unsigned char c);
unsigned char  RcvByte();
void Ack_I2c(byte a);

void Use3V3I2C();
void Use5VI2C();
void UseFMI2C();

void delay1us(void);
void delayxus(byte x);
void delayx100us(int x);
void delayxms(int x);

///////////////////////////////////////////////////////////////////////////////////
byte ISendByte_NoSub(unsigned char sla,unsigned char c);
byte ISendByte(unsigned char sla,unsigned char suba,unsigned char c);

byte ISendStr_NoSub(unsigned char sla,unsigned char *s,unsigned char no);
byte ISendStr(unsigned char sla,unsigned char suba,unsigned char *s,unsigned char no);

byte IRcvStr_NoSub(unsigned char sla,unsigned char *s,unsigned char no);
byte IRcvStr(unsigned char sla,unsigned char suba,unsigned char *s,unsigned char no);

byte IRcvByte_NoSub(unsigned char sla,unsigned char *c);
byte IRcvByte(unsigned char sla,unsigned char suba,unsigned char *c);
extern byte  ack;           /*应答标志位*/


#define SDA_3V3           PTED_PTED5
#define SDA_3V3_D         PTEDD_PTEDD5
#define SCL_3V3           PTED_PTED4
#define SCL_3V3_D         PTEDD_PTEDD4

#endif