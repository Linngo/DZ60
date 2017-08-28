/** ###################################################################
**     Filename  : Gprs_pro.C
**     Project   : Gprs_pro
**     Processor : MC9S08DZ60MLH
**     Version   : Driver 01.11
**     Compiler  : CodeWarrior HCS08 C Compiler
**     Date/Time : 2016-5-5, 13:50
**     Abstract  :
**         Main module.
**         This module contains user's application code.
**     Settings  :
**     Contents  :
**         No public methods
**
** ###################################################################*/
/* MODULE Gprs_pro */


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

/* User includes (#include below this line is not maintained by Processor Expert) */

void main(void)
{
  /* Write your local variable definition here */

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

  USART3_RX_STA=0;
    
  PTDDD_PTDDD2=1;
  PTDD_PTDD2=1;
  
  PTEDD_PTEDD2=1;
  PTED_PTED2=0;       
  Cpu_Delay100US(100);
  PTED_PTED2=1;
  Cpu_Delay100US(1100);
  PTED_PTED2=0;  

  /* Write your code here */
  /* For example: for(;;) { } */
  for(;;){
  
  test();
  
  }
  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END Gprs_pro */
/*
** ###################################################################
**
**     This file was created by Processor Expert 3.07 [04.34]
**     for the Freescale HCS08 series of microcontrollers.
**
** ###################################################################
*/
