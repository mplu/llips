/********************************************//**
 * \file
 * \brief About contrast and edge detection on image
 * \author MPE
 *
 ***********************************************/
#ifndef __LLIPS_CONTRAST_DETECTION_H
#define __LLIPS_CONTRAST_DETECTION_H
/* ***************************************************************/
/* *           Define                                            */
/* ***************************************************************/
#define HOR                 0x0001
#define VER                 0x0002
#define DIAG                0x0004

/* ***************************************************************/
/* *           Global variables                                  */
/* ***************************************************************/

/* ***************************************************************/
/* *           Prototypes                                        */
/* ***************************************************************/
CPU_CHAR search_contrast(CPU_CHAR tolerance, t_img * img_in1,t_img * img_out,CPU_INT32U RGB, CPU_INT32U color,CPU_CHAR direction);
CPU_VOID analyse_result(t_img * img_in1,t_img * img_out,CPU_INT32U colorvalue, CPU_INT32U colortype,CPU_INT16S sizeofsearch,t_simplearea* areaofsearch);

#endif
