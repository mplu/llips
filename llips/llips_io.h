/****************************************************************/
/* Light Library for Image ProcesS                              */
/* File : llips_io.h                                            */
/* Description :                                                */
/*   About importing and exporting image from/to storage        */
/*                                                              */
/* Author : MPE                                                 */
/*                                                              */
/****************************************************************/

/****************************************************************/
/**           Define                                            */
/****************************************************************/

/****************************************************************/
/**           Prototypes                                        */
/****************************************************************/
CPU_VOID init_img(t_img * img);
CPU_CHAR load_img(CPU_CHAR * imgname, t_img * img);
CPU_CHAR write_img(CPU_CHAR * imgname, t_img * img);
