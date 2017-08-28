/** ###################################################################
**     Filename  : Project2.C
**     Project   : Project2
**     Processor : MC9S08DZ60MLH
**     Version   : Driver 01.11
**     Compiler  : CodeWarrior HCS08 C Compiler
**     Date/Time : 2016-4-2, 17:11
**     Abstract  :
**         Main module.
**         This module contains user's application code.
**     Settings  :
**     Contents  :
**         No public methods
**
** ###################################################################*/
/* MODULE Project2 */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "AS1.h"

/* Include shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include <stdio.h>
#include <math.h>
/* User includes (#include below this line is not maintained by Processor Expert) */
#include "lis3dh_driver.h"

#define filter 40  //1:²»ÂË²¨

#define test1

status_t response;

void main(void)
{
  /* Write your local variable definition here */
  byte position=0, old_position=0;
  AxesRaw_t data,handle;
       
  byte a,i=0;
  long x,y,z;
 
 #ifdef test2
  char *adr;
  uint8_t buffer[26];
 #endif
 
  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/
  /* Write your code here */
  /* For example: for(;;) { } */
  
  
  PTDDD_PTDDD5=1;
  PTDD_PTDD5=1;       //CS=1
    
  PTFDD_PTFDD7=1;
  PTFD_PTFD7=0;       //SDO/SA0=0
  
  PTEDD_PTEDD4=1;     
  
  
  //Inizialize MEMS Sensor
  //set ODR (turn ON device)
  response = LIS3DH_SetODR(LIS3DH_ODR_100Hz);
  if(response==1){

  }
  //set PowerMode 
  response = LIS3DH_SetMode(LIS3DH_NORMAL);
  if(response==1){

  }
  //set Fullscale
  response = LIS3DH_SetFullScale(LIS3DH_FULLSCALE_8);
  if(response==1){

  }
  //set axis Enable
  response = LIS3DH_SetAxis(LIS3DH_X_ENABLE | LIS3DH_Y_ENABLE | LIS3DH_Z_ENABLE);
  if(response==1){

  }


  #ifdef test1
  
  if(LIS3DH_GetWHO_AM_I(&a)){
  if(a==0x33)//IICÕýÈ· 
  {
    response = LIS3DH_GetAccAxesRaw(&data);
    x=data.AXIS_X;   
    y=data.AXIS_Y;    
    z=data.AXIS_Z;
        
    for(;;){               
                 
    response = LIS3DH_GetAccAxesRaw(&data);
    if(response==1){
    //print data values
    
    x = (data.AXIS_X + (filter-1)*x)/filter;       
    y = (data.AXIS_Y + (filter-1)*y)/filter;
    z = (data.AXIS_Z + (filter-1)*z)/filter;

    handle.AXIS_X = x>>2;       
    handle.AXIS_Y = y>>2;
    handle.AXIS_Z = z>>2;
    
   
                              
    AS1_SendChar(0x03);Cpu_Delay100US(10);AS1_SendChar(~0x03);Cpu_Delay100US(10);
    AS1_SendChar(handle.AXIS_X);Cpu_Delay100US(10);
    AS1_SendChar(handle.AXIS_X>>8);Cpu_Delay100US(10);
    AS1_SendChar(data.AXIS_X>>2);Cpu_Delay100US(10);
    AS1_SendChar(data.AXIS_X>>10);Cpu_Delay100US(10);
    AS1_SendChar(handle.AXIS_Y);Cpu_Delay100US(10);
    AS1_SendChar(handle.AXIS_Y>>8);Cpu_Delay100US(10);
    AS1_SendChar(data.AXIS_Y>>2);Cpu_Delay100US(10);
    AS1_SendChar(data.AXIS_Y>>10);Cpu_Delay100US(10);
    AS1_SendChar(handle.AXIS_Z);Cpu_Delay100US(10);
    AS1_SendChar(handle.AXIS_Z>>8);Cpu_Delay100US(10);
    AS1_SendChar(data.AXIS_Z>>2);Cpu_Delay100US(10);
    AS1_SendChar(data.AXIS_Z>>10);Cpu_Delay100US(10);
    AS1_SendChar(~0x03);Cpu_Delay100US(10);AS1_SendChar(0x03);Cpu_Delay100US(10);      
  
      }  
    }
  } 
}
 #endif
 
 #ifdef test2
 
 
 adr=buffer;
  
 //set Interrupt Threshold 
 response = LIS3DH_SetInt1Threshold(100);
 if(response==1){
 
  }
 //set Interrupt configuration (all enabled)
 response = LIS3DH_SetIntConfiguration(LIS3DH_INT1_ZHIE_ENABLE | LIS3DH_INT1_ZLIE_ENABLE |
									   LIS3DH_INT1_YHIE_ENABLE | LIS3DH_INT1_YLIE_ENABLE |
									   LIS3DH_INT1_XHIE_ENABLE | LIS3DH_INT1_XLIE_ENABLE ); 
 if(response==1){

  }
 //set Interrupt Mode
 response = LIS3DH_SetIntMode(LIS3DH_INT_MODE_6D_POSITION);
 if(response==1){

  }

 for(;;){
  //get 6D Position
  response = LIS3DH_Get6DPosition(&position);
  if((response==1) && (old_position!=position)){
    switch (position){
    case LIS3DH_UP_SX:   sprintf((char*)buffer,"\n\rposition = UP_SX  \n\r\0");   break;
    case LIS3DH_UP_DX:   sprintf((char*)buffer,"\n\rposition = UP_DX  \n\r\0");   break;
    case LIS3DH_DW_SX:   sprintf((char*)buffer,"\n\rposition = DW_SX  \n\r\0");   break;              
    case LIS3DH_DW_DX:   sprintf((char*)buffer,"\n\rposition = DW_DX  \n\r\0");   break; 
    case LIS3DH_TOP:     sprintf((char*)buffer,"\n\rposition = TOP    \n\r\0");   break; 
    case LIS3DH_BOTTOM:  sprintf((char*)buffer,"\n\rposition = BOTTOM \n\r\0");   break; 
    default:      sprintf((char*)buffer,"\n\rposition = unknown\n\r\0");   break;
    }
  for(i=0;i<23;i++) { AS1_SendChar(buffer[i]);Cpu_Delay100US(1000);}
  }
 }
  
 #endif   
  
  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/
/* END Project2 */
/*
** ###################################################################
**
**     This file was created by Processor Expert 3.07 [04.34]
**     for the Freescale HCS08 series of microcontrollers.
**
** ###################################################################
*/
