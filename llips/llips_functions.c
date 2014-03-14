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
CPU_INT08U **createTable(CPU_INT16S nbLin, CPU_INT16S nbCol)
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
/* freeTable()                                                  */
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
CPU_VOID freeTable(CPU_INT08U **tab)
{
	free(tab[0]);
	free(tab);
}
