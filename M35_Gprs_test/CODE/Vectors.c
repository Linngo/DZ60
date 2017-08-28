/** ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename  : Vectors.C
**     Project   : Gprs_pro
**     Processor : MC9S08DZ60MLH
**     Version   : Component 01.075, Driver 01.35, CPU db: 3.00.021
**     Compiler  : CodeWarrior HCS08 C Compiler
**     Date/Time : 2016-5-14, 11:35
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

void (* near const _vect[])(void) @0x87C0 = { /* Interrupt vector table */
         Cpu_ivVacmp2,                 /* Int.no. 31 Vacmp2 (at 87C0)                Unassigned */
         Cpu_ivVacmp1,                 /* Int.no. 30 Vacmp1 (at 87C2)                Unassigned */
         Cpu_ivVcantx,                 /* Int.no. 29 Vcantx (at 87C4)                Unassigned */
         Cpu_ivVcanrx,                 /* Int.no. 28 Vcanrx (at 87C6)                Unassigned */
         Cpu_ivVcanerr,                /* Int.no. 27 Vcanerr (at 87C8)               Unassigned */
         Cpu_ivVcanwu,                 /* Int.no. 26 Vcanwu (at 87CA)                Unassigned */
         TI1_Interrupt,                /* Int.no. 25 Vrtc (at 87CC)                  Used */
         Cpu_ivViic,                   /* Int.no. 24 Viic (at 87CE)                  Unassigned */
         Cpu_ivVadc,                   /* Int.no. 23 Vadc (at 87D0)                  Unassigned */
         Cpu_ivVport,                  /* Int.no. 22 Vport (at 87D2)                 Unassigned */
         AS1_InterruptTx,              /* Int.no. 21 Vsci2tx (at 87D4)               Used */
         AS1_InterruptRx,              /* Int.no. 20 Vsci2rx (at 87D6)               Used */
         AS1_InterruptError,           /* Int.no. 19 Vsci2err (at 87D8)              Used */
         Cpu_ivVsci1tx,                /* Int.no. 18 Vsci1tx (at 87DA)               Unassigned */
         Cpu_ivVsci1rx,                /* Int.no. 17 Vsci1rx (at 87DC)               Unassigned */
         Cpu_ivVsci1err,               /* Int.no. 16 Vsci1err (at 87DE)              Unassigned */
         Cpu_ivVspi,                   /* Int.no. 15 Vspi (at 87E0)                  Unassigned */
         Cpu_ivVtpm2ovf,               /* Int.no. 14 Vtpm2ovf (at 87E2)              Unassigned */
         Cpu_ivVtpm2ch1,               /* Int.no. 13 Vtpm2ch1 (at 87E4)              Unassigned */
         Cpu_ivVtpm2ch0,               /* Int.no. 12 Vtpm2ch0 (at 87E6)              Unassigned */
         Cpu_ivVtpm1ovf,               /* Int.no. 11 Vtpm1ovf (at 87E8)              Unassigned */
         Cpu_ivVtpm1ch5,               /* Int.no. 10 Vtpm1ch5 (at 87EA)              Unassigned */
         Cpu_ivVtpm1ch4,               /* Int.no.  9 Vtpm1ch4 (at 87EC)              Unassigned */
         Cpu_ivVtpm1ch3,               /* Int.no.  8 Vtpm1ch3 (at 87EE)              Unassigned */
         Cpu_ivVtpm1ch2,               /* Int.no.  7 Vtpm1ch2 (at 87F0)              Unassigned */
         Cpu_ivVtpm1ch1,               /* Int.no.  6 Vtpm1ch1 (at 87F2)              Unassigned */
         Cpu_ivVtpm1ch0,               /* Int.no.  5 Vtpm1ch0 (at 87F4)              Unassigned */
         Cpu_ivVlol,                   /* Int.no.  4 Vlol (at 87F6)                  Unassigned */
         Cpu_ivVlvd,                   /* Int.no.  3 Vlvd (at 87F8)                  Unassigned */
         Cpu_ivVirq,                   /* Int.no.  2 Virq (at 87FA)                  Unassigned */
         Cpu_ivVswi                    /* Int.no.  1 Vswi (at 87FC)                  Unassigned */
};

void (* near const _vectReset[])(void) @0xFFFE = { /* Interrupt vector table */
        _EntryPoint                    /* Int.no.  0 Vreset (at FFFE)                Reset vector */
};
/*
** ###################################################################
**
**     This file was created by Processor Expert 3.07 [04.34]
**     for the Freescale HCS08 series of microcontrollers.
**
** ###################################################################
*/
