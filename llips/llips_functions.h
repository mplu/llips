/****************************************************************/
/* Light Library for Image ProcesS                              */
/* File : llips_constrastdetect.h                               */
/* Description :                                                */
/*   About contrast and edge detection on image                 */
/*                                                              */
/* Author : MPE                                                 */
/*                                                              */
/****************************************************************/

/****************************************************************/
/**           Define                                            */
/****************************************************************/

/****************************************************************/
/**           Global variables                                  */
/****************************************************************/

/****************************************************************/
/**           Prototypes                                        */
/****************************************************************/
CPU_INT32U min(CPU_INT32U a,CPU_INT32U b);
CPU_INT32U max(CPU_INT32U a,CPU_INT32U b);
CPU_INT08U **createTable(CPU_INT16S nbLin, CPU_INT16S nbCol);
CPU_VOID freeTable(CPU_INT08U **tab);
