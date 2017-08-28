/** ###################################################################
**     Filename  : Project3.C
**     Project   : Project3
**     Processor : MC9S08DZ60MLH
**     Version   : Driver 01.11
**     Compiler  : CodeWarrior HCS08 C Compiler
**     Date/Time : 2016-5-4, 13:48
**     Abstract  :
**         Main module.
**         This module contains user's application code.
**     Settings  :
**     Contents  :
**         No public methods
**
** ###################################################################*/
/* MODULE Project3 */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "AS1.h"
#include "TI1.h"
/* Include shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "FLASH.h"
#include  <string.h>
#include  <stdio.h>

/* User includes (#include below this line is not maintained by Processor Expert) */
//const byte a @0x9020=0x11;

byte* check_cmd(byte *str)
{
	char *strx=0;
	if(USART3_RX_STA&0X80)		//接收到一次数据了
	{ 
		USART3_RX_BUF[USART3_RX_STA&0X7F]=0;//添加结束符
		strx=strstr((const char*)USART3_RX_BUF,(const char*)str);
	} 
	return (byte*)strx;
}

byte chr2hex(byte chr)
{
	if(chr>='0'&&chr<='9')return chr-'0';
	if(chr>='A'&&chr<='F')return (chr-'A'+10);
	if(chr>='a'&&chr<='f')return (chr-'a'+10); 
	return 0;
}

byte sscan(byte* p,word* l,byte n)
{ byte u;
  *l=0;
  for(u=n; u>0; u--) 
  {
    *l|=chr2hex(*p++)<<((u-1)*4);
  }
  return 1;
}

int read_s19(byte *pc)
{

  word len;  //剩余字节数
  byte alen, //地址长度
       sum,  //校验计算
       line=0,
       terminate = 0,
       c,u,*tmp,
       imgdata[35];

  word addr,total,d,
       MAX_ADDRESS=0x8C56;
   
  total=0;
  tmp = pc;     

	while(!terminate)
	{

	  if((USART3_RX_STA&0X80)==0){
	   continue;
	  }
	  
	  USART3_RX_BUF[USART3_RX_STA&0X7F]=0;//添加结束符
	  
		line++;

		// S-records only
		if(*pc++ != 'S') 
			continue;

		// record type
		switch(c = *pc++)
		{
		case '0':	// S0 is skipped
			continue;
		case '1':	// S1 is accepted
			alen = 4;
			break;
		case '2':	// S2 is accepted
			alen = 6;
			break;
		case '3':	// S3 is accepted
			alen = 8;
			break;
		case '9':	// S9 terminates
		case '8':	// S8 terminates
		case '7':	// S7 terminates
			terminate = 1;
			continue;
		default:	// others are reported and skipped
	//		fprintf(stderr, "Skipping S%c record at line %d", c, line);
			continue;
		}

    sscan(pc, &len, 2);      
    sscan(pc+2, &addr, alen);

		if(addr >= MAX_ADDRESS)
		{
			return 0;//		地址错误
		}
	
	  pc += alen+2;

		sum = (byte)len;
		for(u=0; u<4; u++)
			sum += (addr >> (u*8)) & 0xff;

		len -= alen/2 + 1; //去地址及校验位长度
		
		for(u=0; u<len; u++)
		{
		  sscan(pc, &d, 2);
			if(d >= 0x100)
			{
//				fprintf(stderr, "S-record data parse error at line %d\n", line);
				return 0;
			}
			pc += 2;						
			imgdata[u] = (byte)d;			
			sum += (byte)d;			
			total++;
		}

//		printf("Bytes: %d", total);//写入的数据量

		
		sscan(pc, &d, 2);
		if(d <= 0x100 && ((sum+(byte)d)&0xff) == 0xff)// test CS
		{
		  SaveToFlash(addr, imgdata, len);
		}
		else
		{
			return 0;//		校验错误	  
		}
	  USART3_RX_STA=0;
	  pc = tmp;		
	}

	if (total == 0)
	{
		return -1;  //	没有程序
	}
	return 1;
}


void main(void)
{
  /* Write your local variable definition here */

  
  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/
  USART3_RX_STA=0;

  /* Write your code here */
  /* For example: for(;;) { } */
  for(;;){

        if(check_cmd("S1")) {
          
        if(read_s19((const byte*)USART3_RX_BUF))
        {
          USART3_RX_STA=0;
          asm jmp 0x1900;
        }
        }
  }
  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END Project3 */
/*
** ###################################################################
**
**     This file was created by Processor Expert 3.07 [04.34]
**     for the Freescale HCS08 series of microcontrollers.
**
** ###################################################################
*/
