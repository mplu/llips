/****************************************************************/
/* File : llips_functions.c                                     */
/* Description :                                                */
/*   Miscellaneous functions used in llips                      */
/*                                                              */
/* Author : MPE                                                 */
/*                                                              */
/****************************************************************/

/****************************************************************/
/**           Includes                                          */
/****************************************************************/
#include "llips_includes.h"

/****************************************************************/
/**           Global variables                                  */
/****************************************************************/

/****************************************************************/
/**           Functions                                         */
/****************************************************************/

/****************************************************************/
/* min()                                                        */
/* Description :                                                */
/*   compare two CPU_INT32U to find the smallest                */
/* Input:                                                       */
/*   a - first value                                            */
/*   b - second value                                           */
/* Output:                                                      */
/*   na                                                         */
/* Return:                                                      */
/*   minimal value                                              */
/*                                                              */
/****************************************************************/
CPU_INT32U min(CPU_INT32U a,CPU_INT32U b)
{
    CPU_INT32U ret=0;
    if(a >= b)
    {
        ret = b;
    }else
    {
        ret = a;
    }
    return ret;
}

/****************************************************************/
/* max()                                                        */
/* Description :                                                */
/*   compare two CPU_INT32U to find the biggest                 */
/* Input:                                                       */
/*   a - first value                                            */
/*   b - second value                                           */
/* Output:                                                      */
/*   na                                                         */
/* Return:                                                      */
/*   maximal value                                              */
/*                                                              */
/****************************************************************/
CPU_INT32U max(CPU_INT32U a,CPU_INT32U b)
{
    CPU_INT32U ret=0;
    if(a >= b)
    {
        ret = a;
    }else
    {
        ret = b;
    }
    return ret;
}

/****************************************************************/
/* createTable()                                                */
/* Description :                                                */
/*   allocate memory for a CPU_INT08U 2D table                  */
/* Input:                                                       */
/*   nbLin - number of line (height)                            */
/*   nbCol - number of column (widht)                           */
/* Output:                                                      */
/*   na                                                         */
/* Return:                                                      */
/*   pointer on a CPU_INT08U 2D table                           */
/*                                                              */
/****************************************************************/
CPU_INT08U **createTableINT08U(CPU_INT16S nbLin, CPU_INT16S nbCol)
{
	CPU_INT16S i;
	CPU_INT08U **tab1 = (CPU_INT08U **)malloc(sizeof(CPU_INT08U*)*nbLin);
	CPU_INT08U *tab2 = (CPU_INT08U *)malloc(sizeof(CPU_INT08U)*nbCol*nbLin);
	for(i = 0 ; i < nbLin ; i++){
		tab1[i] = &tab2[i*nbCol];
	}
	return tab1;
}

/****************************************************************/
/* createTableFP64()                                            */
/* Description :                                                */
/*   allocate memory for a CPUFP64 2D table                     */
/* Input:                                                       */
/*   nbLin - number of line (height)                            */
/*   nbCol - number of column (widht)                           */
/* Output:                                                      */
/*   na                                                         */
/* Return:                                                      */
/*   pointer on a CPU_FP64 2D table                             */
/*                                                              */
/****************************************************************/
CPU_FP64 **createTableFP64(CPU_INT16S nbLin, CPU_INT16S nbCol)
{
	CPU_INT16S i;
	CPU_FP64 **tab1 = (CPU_FP64 **)malloc(sizeof(CPU_FP64*)*nbLin);
	CPU_FP64 *tab2 = (CPU_FP64 *)malloc(sizeof(CPU_FP64)*nbCol*nbLin);
	for(i = 0 ; i < nbLin ; i++){
		tab1[i] = &tab2[i*nbCol];
	}

	return tab1;
}

/****************************************************************/
/* freeTableINT08U()                                            */
/* Description :                                                */
/*   deallocate memory for a CPU_INT08U 2D table                */
/* Input:                                                       */
/*   tab - table to destroy                                     */
/* Output:                                                      */
/*   na                                                         */
/* Return:                                                      */
/*   na                                                         */
/*                                                              */
/****************************************************************/
CPU_VOID freeTableINT08U(CPU_INT08U **tab)
{
	free(tab[0]);
	free(tab);
}

/****************************************************************/
/* freeTableINT08U()                                            */
/* Description :                                                */
/*   deallocate memory for a CPU_INT08U 2D table                */
/* Input:                                                       */
/*   tab - table to destroy                                     */
/* Output:                                                      */
/*   na                                                         */
/* Return:                                                      */
/*   na                                                         */
/*                                                              */
/****************************************************************/
CPU_VOID freeTableFP64(CPU_FP64 **tab)
{
	free(tab[0]);
	free(tab);
}


int compare (const CPU_VOID * a, const CPU_VOID * b)
{
  return ( *(CPU_INT16S*)a - *(CPU_INT16S*)b );
}
