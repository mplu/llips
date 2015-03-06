/* ***************************************************************/
/* File : llips_exemple.c                                       */
/* Description :                                                */
/*   Main functions to demonstrate LLIPS possibility            */
/*                                                              */
/* Author : MPE                                                 */
/*                                                              */
/* ***************************************************************/

/* ***************************************************************/
/* *           Includes                                          */
/* ***************************************************************/
#include "includes.h"

/* ***************************************************************/
/* *           Global variables                                  */
/* ***************************************************************/
CPU_CHAR g_debug_mode = DEF_DISABLED;

/* ***************************************************************/
/* *           Functions                                         */
/* ***************************************************************/
int main(int argc, char *argv[])
{

    clock_t start, finish;
    double duration;

    t_img img_in1;
    t_img img_inter1;
    t_img img_out1;

    CPU_FP64 ** filter0;
    CPU_FP64 ** filter1;
    CPU_FP64 ** filter2;
    CPU_FP64 ** filter3;

    CPU_INT16S filter1size;
    CPU_INT16S filter2size;

    CPU_FP64 sigma;

    CPU_CHAR outputfilename[32];

    start = clock();
    //int tolerance, quantity;


    init_img(&img_in1);
    init_img(&img_out1);
    init_img(&img_inter1);

    load_img((CPU_CHAR *)"imgnew.bmp", &img_in1);

    filter0 = createTableFP64(7,7);
    filter1 = createTableFP64(3,3);
    filter2 = createTableFP64(5,5);

    filter3 = createTableFP64(3,3);

    filter3 = createTableFP64(3,3);

    filter3[0][0] = -3.0 / 15.0  ;
    filter3[0][1] = -3.0 / 15.0;
    filter3[0][2] = 5.0 / 15.0;

    filter3[1][0] = -3.0 / 15.0;
    filter3[1][1] = 0 / 15.0;
    filter3[1][2] = 5.0 / 15.0;

    filter3[2][0] = -3.0 / 15.0;
    filter3[2][1] = -3.0 / 15.0;
    filter3[2][2] = -3.0 / 15.0;


    //ok for RPi-kee
    filter1size = 7;
    sigma = 2.5;
    create_gauss_filter(filter0,filter1size,sigma);
    apply_linfilter(&img_in1,filter0,filter1size,GREEN|BLUE|RED,&img_inter1);
    sprintf((char *)outputfilename,"%s_gauss%s",IMG_OUT,BMP_EXT);
    //write_img(outputfilename,&img_inter1);
    search_contrast(1,&img_inter1,&img_out1,SetRGB(255,255,255),GREEN|BLUE|RED,HOR|VER);
    sprintf((char *)outputfilename,"%s_contdetect%s",IMG_OUT,BMP_EXT);
    //write_img(outputfilename,&img_out1);

    /*// laplace 1
    filter1size = create_laplacian_filter(filter1,1);
    apply_linfilter(&img_inter1,filter1,filter1size,GREEN|BLUE|RED,&img_out1);
    sprintf((char *)outputfilename,"%s_lap1%s",IMG_OUT,BMP_EXT);
    write_img(outputfilename,&img_out1);

    // laplace 2
    filter1size = create_laplacian_filter(filter1,2);
    apply_linfilter(&img_inter1,filter1,filter1size,GREEN|BLUE|RED,&img_out1);
    sprintf((char *)outputfilename,"%s_lap2%s",IMG_OUT,BMP_EXT);
    write_img(outputfilename,&img_out1);

    // laplace 3
    filter1size = create_laplacian_filter(filter1,3);
    apply_linfilter(&img_inter1,filter1,filter1size,GREEN|BLUE|RED,&img_out1);
    sprintf((char *)outputfilename,"%s_lap3%s",IMG_OUT,BMP_EXT);
    write_img(outputfilename,&img_out1);

    // laplace 4
    filter1size = create_laplacian_filter(filter1,4);
    apply_linfilter(&img_inter1,filter1,filter1size,GREEN|BLUE|RED,&img_out1);
    sprintf((char *)outputfilename,"%s_lap4%s",IMG_OUT,BMP_EXT);
    write_img(outputfilename,&img_out1);*/

    apply_linfilter(&img_inter1,filter3,3,GREEN|BLUE|RED,&img_out1);
    sprintf((char *)outputfilename,"%s_kirchH1%s",IMG_OUT,BMP_EXT);
    //write_img(outputfilename,&img_out1);

    // laplace 5
    filter2size = create_laplacian_filter(filter2,5);
    apply_linfilter(&img_inter1,filter2,filter2size,GREEN|BLUE|RED,&img_out1);
    sprintf((char *)outputfilename,"%s_lap5%s",IMG_OUT,BMP_EXT);
    //write_img(outputfilename,&img_out1);



    finish = clock();
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    printf( "duration : %f seconds\n", duration );

    return 0;
}

