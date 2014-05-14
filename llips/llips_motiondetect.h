/****************************************************************/
/* Light Library for Image ProcesS                              */
/* File : llips_motiondetec.h                                   */
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
CPU_CHAR search_diff(CPU_INT16U tolerance,CPU_INT16U quantity, t_img * img_in1,t_img * img_in2,t_img * img_out,t_area * change_img);
t_vect evaluate_move(t_img * p_img_base,t_img * p_img_target,t_area area_1,t_area area_2, t_vect * p_movement_origin);
CPU_CHAR search_diff_x(CPU_INT16U tolerance,CPU_INT16U quantity, t_img * img_in1,t_img * img_in2,t_img * img_out,t_area * change_img);
t_pixel look_for_match(t_img * img_in1,t_img * img_in2,CPU_INT16U areasize,t_pixel area_from_1,CPU_INT16U raw_tolerance);
