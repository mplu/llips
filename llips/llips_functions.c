/********************************************//**
 * \file
 * \brief Miscellaneous functions used in llips
 * \author MPE
 *
 ***********************************************/

/* ***************************************************************/
/* *           Includes                                          */
/* ***************************************************************/
#include "llips_includes.h"

/* ***************************************************************/
/* *           Global variables                                  */
/* ***************************************************************/

/* ***************************************************************/
/* *           Functions                                         */
/* ***************************************************************/

/********************************************//**
 * \brief Compare two CPU_INT32U to find the smallest
 *
 * \param a CPU_INT32U - first value
 * \param b CPU_INT32U - second value
 * \return CPU_INT32U - minimal value
 *
 ***********************************************/
CPU_INT32U mini(CPU_INT32U a,CPU_INT32U b)
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


/********************************************//**
 * \brief compare two CPU_INT32U to find the biggest
 *
 * \param a CPU_INT32U - first value
 * \param b CPU_INT32U - second value
 * \return CPU_INT32U - maximal value
 *
 ***********************************************/
CPU_INT32U maxi(CPU_INT32U a,CPU_INT32U b)
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

/********************************************//**
 * \brief Allocate memory for a CPU_INT08U 2D table
 *
 * \param nbLin CPU_INT16S - number of line (height)
 * \param nbCol CPU_INT16S - number of column (widht)
 * \return CPU_INT08U** - pointer on a CPU_INT08U 2D table
 *
 ***********************************************/
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


/********************************************//**
 * \brief Allocate memory for a CPU_FP64 2D table
 *
 * \param nbLin CPU_INT16S - number of line (height)
 * \param nbCol CPU_INT16S - number of column (widht)
 * \return CPU_FP64** - pointer on a CPU_FP64 2D table
 *
 ***********************************************/
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



/********************************************//**
 * \brief Deallocate memory for a CPU_INT08U 2D table
 *
 * \param tab CPU_INT08U** - table to destroy
 * \return CPU_VOID
 *
 ***********************************************/
CPU_VOID freeTableINT08U(CPU_INT08U **tab)
{
	free(tab[0]);
	free(tab);
}

/********************************************//**
 * \brief Deallocate memory for a CPU_FP64 2D table
 *
 * \param tab CPU_FP64** - table to destroy
 * \return CPU_VOID
 *
 ***********************************************/
CPU_VOID freeTableFP64(CPU_FP64 **tab)
{
	free(tab[0]);
	free(tab);
}

/********************************************//**
 * \brief Compare function for qsort function
 *
 * \param a const CPU_VOID * - first param
 * \param b const CPU_VOID * - second param
 * \return CPU_INT32S - difference between both
 *
 ***********************************************/
CPU_INT32S compare (const CPU_VOID * a, const CPU_VOID * b)
{
  return ( *(CPU_INT16S*)a - *(CPU_INT16S*)b );
}
