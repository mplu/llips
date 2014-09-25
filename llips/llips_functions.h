/********************************************//**
 * \file
 * \brief Various functions for Llips
 * \author MPE
 *
 ***********************************************/
#ifndef __LLIPS_FUNCTIONS_H
#define __LLIPS_FUNCTIONS_H
/* ***************************************************************/
/* *           Define                                            */
/* ***************************************************************/

/* ***************************************************************/
/* *           Global variables                                  */
/* ***************************************************************/

/* ***************************************************************/
/* *           Prototypes                                        */
/* ***************************************************************/
CPU_INT32U mini(CPU_INT32U a,CPU_INT32U b);
CPU_INT32U maxi(CPU_INT32U a,CPU_INT32U b);
CPU_INT08U **createTableINT08U(CPU_INT16S nbLin, CPU_INT16S nbCol);
CPU_FP64 **createTableFP64(CPU_INT16S nbLin, CPU_INT16S nbCol);
CPU_VOID freeTableINT08U(CPU_INT08U **tab);
CPU_VOID freeTableFP64(CPU_FP64 **tab);
CPU_INT32S compare (const CPU_VOID * a, const CPU_VOID * b);

#endif
