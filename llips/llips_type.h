/****************************************************************/
/* Light Library for Image ProcesS                              */
/* File : llips_type.h                                          */
/* Description :                                                */
/*   Configuration varaible and define for LLIPS                */
/*                                                              */
/* Author : MPE                                                 */
/*                                                              */
/****************************************************************/
#ifndef __LLIPS_TYPE_H
#define __LLIPS_TYPE_H

/****************************************************************/
/**           Define                                            */
/****************************************************************/
#define DEF_ENABLED         1
#define DEF_DISABLED        0

/****************************************************************/
/**           Typedef                                           */
/****************************************************************/
/* Custom type used for portability */
typedef            void      CPU_VOID;
typedef  unsigned  char      CPU_CHAR;                          /*  8-bit character                                     */
typedef  unsigned  char      CPU_BOOLEAN;                       /*  8-bit boolean or logical                            */
typedef  unsigned  char      CPU_INT08U;                        /*  8-bit unsigned integer                              */
typedef    signed  char      CPU_INT08S;                        /*  8-bit   signed integer                              */
typedef  unsigned  short     CPU_INT16U;                        /* 16-bit unsigned integer                              */
typedef    signed  short     CPU_INT16S;                        /* 16-bit   signed integer                              */
typedef  unsigned  long      CPU_INT32U;                        /* 32-bit unsigned integer                              */
typedef    signed  long      CPU_INT32S;                        /* 32-bit   signed integer                              */
typedef            float     CPU_FP32;                          /* 32-bit floating point                                */
typedef            double    CPU_FP64;                          /* 64-bit floating point                                */

/* typedef for image treatment */
typedef struct s_img
{
    CPU_INT16U  signature ;
    CPU_INT16U  depth ;
    CPU_INT32U   wi ;
    CPU_INT32U   he ;
    CPU_INT08U ** Blue;
    CPU_INT08U ** Green;
    CPU_INT08U ** Red;

    CPU_INT08U   FileHeader[MAX_HEADER_size];
    CPU_INT08U   FileHeader_size;

}t_img;

typedef struct s_pixel
{
    CPU_INT16U  x ;
    CPU_INT16U  y ;
}t_pixel;

typedef struct s_vect
{
    CPU_INT16S  x ;
    CPU_INT16S  y ;
}t_vect;

typedef struct s_area
{
    t_pixel BotLeft;
    t_pixel BotRight;
    t_pixel TopLeft;
    t_pixel TopRight;
}t_area;


#endif
