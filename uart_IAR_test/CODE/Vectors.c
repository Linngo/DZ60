/** ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename  : Vectors.C
**     Project   : Project3
**     Processor : MC9S08DZ60MLH
**     Version   : Component 01.075, Driver 01.35, CPU db: 3.00.021
**     Compiler  : CodeWarrior HCS08 C Compiler
**     Date/Time : 2016-5-6, 9:11
**     Abstract  :
**         This bean "MC9S08DZ60_64" contains initialization of the
**         CPU and provides basic methods and events for CPU core
**         settings.
**     Settings  :
**
**
**     Copyright : 1997 - 2009 Freescale Semiconductor, Inc. All Rights Reserved.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################*/


#include "Cpu.h"
#include "AS1.h"
#include "TI1.h"
extern near void _EntryPoint(void);

void (* near const _vect[])(void) @0xFFC0 = { /* Interrupt vector table */
         Cpu_Interrupt,                /* Int.no. 31 Vacmp2 (at FFC0)                Unassigned */
         Cpu_Interrupt,                /* Int.no. 30 Vacmp1 (at FFC2)                Unassigned */
         Cpu_Interrupt,                /* Int.no. 29 Vcantx (at FFC4)                Unassigned */
         Cpu_Interrupt,                /* Int.no. 28 Vcanrx (at FFC6)                Unassigned */
         Cpu_Interrupt,                /* Int.no. 27 Vcanerr (at FFC8)               Unassigned */
         Cpu_Interrupt,                /* Int.no. 26 Vcanwu (at FFCA)                Unassigned */
         TI1_Interrupt,                /* Int.no. 25 Vrtc (at FFCC)                  Used */
         Cpu_Interrupt,                /* Int.no. 24 Viic (at FFCE)                  Unassigned */
         Cpu_Interrupt,                /* Int.no. 23 Vadc (at FFD0)                  Unassigned */
         Cpu_Interrupt,                /* Int.no. 22 Vport (at FFD2)                 Unassigned */
         AS1_InterruptTx,              /* Int.no. 21 Vsci2tx (at FFD4)               Used */
         AS1_InterruptRx,              /* Int.no. 20 Vsci2rx (at FFD6)               Used */
         AS1_InterruptError,           /* Int.no. 19 Vsci2err (at FFD8)              Used */
         Cpu_Interrupt,                /* Int.no. 18 Vsci1tx (at FFDA)               Unassigned */
         Cpu_Interrupt,                /* Int.no. 17 Vsci1rx (at FFDC)               Unassigned */
         Cpu_Interrupt,                /* Int.no. 16 Vsci1err (at FFDE)              Unassigned */
         Cpu_Interrupt,                /* Int.no. 15 Vspi (at FFE0)                  Unassigned */
         Cpu_Interrupt,                /* Int.no. 14 Vtpm2ovf (at FFE2)              Unassigned */
         Cpu_Interrupt,                /* Int.no. 13 Vtpm2ch1 (at FFE4)              Unassigned */
         Cpu_Interrupt,                /* Int.no. 12 Vtpm2ch0 (at FFE6)              Unassigned */
         Cpu_Interrupt,                /* Int.no. 11 Vtpm1ovf (at FFE8)              Unassigned */
         Cpu_Interrupt,                /* Int.no. 10 Vtpm1ch5 (at FFEA)              Unassigned */
         Cpu_Interrupt,                /* Int.no.  9 Vtpm1ch4 (at FFEC)              Unassigned */
         Cpu_Interrupt,                /* Int.no.  8 Vtpm1ch3 (at FFEE)              Unassigned */
         Cpu_Interrupt,                /* Int.no.  7 Vtpm1ch2 (at FFF0)              Unassigned */
         Cpu_Interrupt,                /* Int.no.  6 Vtpm1ch1 (at FFF2)              Unassigned */
         Cpu_Interrupt,                /* Int.no.  5 Vtpm1ch0 (at FFF4)              Unassigned */
         Cpu_Interrupt,                /* Int.no.  4 Vlol (at FFF6)                  Unassigned */
         Cpu_Interrupt,                /* Int.no.  3 Vlvd (at FFF8)                  Unassigned */
         Cpu_Interrupt,                /* Int.no.  2 Virq (at FFFA)                  Unassigned */
         Cpu_Interrupt,                /* Int.no.  1 Vswi (at FFFC)                  Unassigned */
         _EntryPoint                   /* Int.no.  0 Vreset (at FFFE)                Reset vector */
};
/*
** ###################################################################
**
**     This file was created by Processor Expert 3.07 [04.34]
**     for the Freescale HCS08 series of microcontrollers.
**
** ###################################################################
*/

