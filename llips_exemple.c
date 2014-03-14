/****************************************************************/
/* File : llips_exemple.c                                       */
/* Description :                                                */
/*   Main functions to demonstrate LLIPS possibility            */
/*                                                              */
/* Author : MPE                                                 */
/*                                                              */
/****************************************************************/

/****************************************************************/
/**           Includes                                          */
/****************************************************************/
#include "includes.h"

/****************************************************************/
/**           Global variables                                  */
/****************************************************************/
CPU_CHAR g_debug_mode = DEF_DISABLED;

/****************************************************************/
/**           Functions                                         */
/****************************************************************/
int main(int argc, char *argv[])
{
    CPU_INT16U i = 1;
    clock_t start, finish;
    double duration;

    t_img img_in1;
    t_img img_out1;

    CPU_CHAR img_in1_err,img_out1_err,contrast_det=0;
    /*
    t_img img_in2;
    t_img img_in3;

    t_img img_out2;


    t_vect movement;
    t_vect movement_origin;

    CPU_CHAR img_in1_err,img_in2_err,img_in3_err,img_out1_err,img_out2_err,img_out_move_err;
    CPU_CHAR img_diff_1_2=0,img_diff_2_3=0;

    t_area change_1_2;
    t_area change_2_3;
    */
    start = clock();
    int tolerance, quantity;
    while(i--)
    {


        // checking debug argument
        if (argc == 4)
        {

            if(strcmp(DEBUG_opt, argv[3]) == 0)
            {
                g_debug_mode = DEF_ENABLED;
                printf("%s activated\n",argv[3]);
            }
        }else
        {
            //nothing
        }
        g_debug_mode = DEF_ENABLED;
        if (argc < 3)
        {
            if (g_debug_mode == DEF_ENABLED)
            {
                printf("USAGE: %s <tolerance in percent> <quantity of pixel in perthousand> [opt:DEBUG]\nIncorrect Using default value\n",argv[0]);
            }else
            {
                //nothing
            }

            tolerance = DEFAULT_TOLERANCE;
            quantity = DEFAULT_QUANTITY;

        }else
        {
            sscanf (argv[1],"%d",&tolerance);
            sscanf (argv[2],"%d",&quantity);
            if ((tolerance < 0) || (tolerance > 100))
            {
                printf ("tolerance %d out of bound, using default value : %d\n",tolerance,DEFAULT_TOLERANCE);
                tolerance = DEFAULT_TOLERANCE;
            }
            if ((quantity < 0) || (quantity > 1000))
            {
                printf ("quantity %d out of bound, using default value : %d\n",quantity,DEFAULT_QUANTITY);
                quantity = DEFAULT_QUANTITY ;
            }
        }
        if (g_debug_mode == DEF_ENABLED)
        {
            printf ("tolerance %d\nquantity %d\n",tolerance,quantity);
            printf("Init data\n");
        }else
        {
            //nothing
        }

        init_img(&img_in1);
        init_img(&img_out1);

        /*
        init_img(&img_in2);
        init_img(&img_out2);

        init_img(&img_in3);


        //init_img(&img_out_move);

        img_in1_err = load_img(OLD, &img_in1);
        if (img_in1_err == NO_ERROR)
        {
        }else
        {
            if (g_debug_mode == DEF_ENABLED)
            {
                printf("Image %s not supported - err : %x\n",OLD,img_in1_err);
            }else
            {
                //nothing
            }

        }

        img_in2_err = load_img(BASE, &img_in2);
        if (img_in2_err == NO_ERROR)
        {
        }else
        {
            if (g_debug_mode == DEF_ENABLED)
            {
                printf("Image %s not supported - err : %x\n",BASE,img_in2_err);
            }else
            {
                //nothing
            }

        }

        img_in3_err = load_img(NEW, &img_in3);
        if (img_in3_err == NO_ERROR)
        {
        }else
        {
            if (g_debug_mode == DEF_ENABLED)
            {
                printf("Image %s not supported - err : %x\n",NEW,img_in2_err);
            }else
            {
                //nothing
            }

        }

        if ((img_in1_err == NO_ERROR) && (img_in2_err == NO_ERROR))
        {

            img_diff_1_2 = search_diff((CPU_CHAR)tolerance,(CPU_CHAR)quantity,&img_in1,&img_in2,&img_out1,&change_1_2);
            if (img_diff_1_2 == NO_DIFF)
            {
            }else
            {
            }

            //display_img_value(&img_out1,HEADER|BLUE|GREEN|RED|0);
            if((img_diff_1_2 & DIFF_HIGH_QUANTITY)!=0)
            {
                // save a copy
                img_out1_err = write_img(NEW_DIFF_1,&img_out1);
                if(img_out1_err == NO_ERROR)
                {
                }else
                {
                }
                //printf("1\n");
            }else
            {

                //printf("0\n");
            }


        }else
        {
            //printf("0\n");
        }

        if ((img_in2_err == NO_ERROR) && (img_in3_err == NO_ERROR))
        {

            img_diff_2_3 = search_diff((CPU_CHAR)tolerance,(CPU_CHAR)quantity,&img_in2,&img_in3,&img_out2,&change_2_3);
            if (img_diff_2_3 == NO_DIFF)
            {
            }else
            {
            }

            //display_img_value(&img_out1,HEADER|BLUE|GREEN|RED|0);
            if((img_diff_2_3 & DIFF_HIGH_QUANTITY)!=0)
            {
                // save a copy
                img_out2_err = write_img(NEW_DIFF_2,&img_out2);
                if(img_out2_err == NO_ERROR)
                {
                }else
                {
                }
                //printf("1\n");
            }else
            {

                //printf("0\n");
            }


        }else
        {
            //printf("0\n");
        }

        // Determine mouvement
        if(((img_diff_1_2 & DIFF_HIGH_QUANTITY)!=0) && ((img_diff_2_3 & DIFF_HIGH_QUANTITY)!=0))
        {
            init_img(&img_out2);
            //evaluate_move(&img_in2,&img_out_move,change_1_2,change_2_3);
            //img_out_move_err = write_img(MOVE,&img_out_move);
            movement = evaluate_move(&img_in2,&img_out2,change_1_2,change_2_3,&movement_origin);
            img_out_move_err = write_img(MOVE,&img_out2);
            if(img_out_move_err == NO_ERROR)
            {
                //printf("1\t%d\t%d\n",movement.x,movement_origin.x);
            }else
            {
                printf("0\t0\t0\n");
            }

        }else
        {
            printf("0\t0\t0\n");
        }


*/    }

/*********************************************************************************/
/*********************************************************************************/
/*********************************************************************************/
/*********************************************************************************/
    /* color filtering on BASE */
/*
    init_img(&img_in1);
    img_in1_err = load_img(BASE, &img_in1);
    if (img_in1_err == NO_ERROR)
    {
    }else
    {
        if (g_debug_mode == DEF_ENABLED)
        {
            printf("Image %s not supported - err : %x\n",BASE,img_in1_err);
        }else
        {
            //nothing
        }

    }

    init_img(&img_out1);
    color_filter(&img_in1,&img_out1, (RED));
    img_out1_err = write_img(IMG_RED,&img_out1);
    if(img_out1_err == NO_ERROR)
    {
        //printf("1\t%d\t%d\n",movement.x,movement_origin.x);
    }else
    {
        printf("write error\n");
    }


*/
/*********************************************************************************/
/*********************************************************************************/
/*********************************************************************************/
/*********************************************************************************/
    /* Contrast Detection on BASE */
    init_img(&img_in1);
    img_in1_err = load_img(BASE, &img_in1);
    if (img_in1_err == NO_ERROR)
    {
    }else
    {
        if (g_debug_mode == DEF_ENABLED)
        {
            printf("Image %s not supported - err : %x\n",BASE,img_in1_err);
        }else
        {
            //nothing
        }

    }

    tolerance = 5;
    contrast_det = 0;

    init_img(&img_out1);
    contrast_det = search_contrast((CPU_CHAR)tolerance, &img_in1,&img_out1,SetRGB(255,255,255),(RED|GREEN|BLUE),VER);
    img_out1_err = write_img(CONTRAST_OUTVRT,&img_out1);
    if(img_out1_err == NO_ERROR)
    {
        //printf("1\t%d\t%d\n",movement.x,movement_origin.x);
    }else
    {
        printf("write error\n");
    }

    init_img(&img_out1);
    contrast_det = search_contrast((CPU_CHAR)tolerance, &img_in1,&img_out1,SetRGB(255,255,255),(RED|GREEN|BLUE),HOR);
    img_out1_err = write_img(CONTRAST_OUTHRZ,&img_out1);
    if(img_out1_err == NO_ERROR)
    {
        //printf("1\t%d\t%d\n",movement.x,movement_origin.x);
    }else
    {
        printf("write error\n");
    }

    init_img(&img_out1);
    contrast_det = search_contrast((CPU_CHAR)tolerance, &img_in1,&img_out1,SetRGB(255,255,255),(RED|GREEN|BLUE),HOR|VER);
    img_out1_err = write_img(CONTRAST_OUTveho,&img_out1);
    if(img_out1_err == NO_ERROR)
    {
        //printf("1\t%d\t%d\n",movement.x,movement_origin.x);
    }else
    {
        printf("write error\n");
    }


/*********************************************************************************/
/*********************************************************************************/
/*********************************************************************************/
/*********************************************************************************/
    /* histogram calculation on BASE */
    init_img(&img_in1);
    img_in1_err = load_img(BASE, &img_in1);
    if (img_in1_err == NO_ERROR)
    {
    }else
    {
        if (g_debug_mode == DEF_ENABLED)
        {
            printf("Image %s not supported - err : %x\n",BASE,img_in1_err);
        }else
        {
            //nothing
        }

    }




    init_img(&img_out1);
    histogram(&img_in1,&img_out1);
    img_out1_err = write_img(IMG_HISTO,&img_out1);
    if(img_out1_err == NO_ERROR)
    {
        //printf("1\t%d\t%d\n",movement.x,movement_origin.x);
    }else
    {
        printf("write error\n");
    }

/*********************************************************************************/
/*********************************************************************************/
/*********************************************************************************/
/*********************************************************************************/
    /* luminance on BASE */
    init_img(&img_in1);
    img_in1_err = load_img(BASE, &img_in1);
    if (img_in1_err == NO_ERROR)
    {
    }else
    {
        if (g_debug_mode == DEF_ENABLED)
        {
            printf("Image %s not supported - err : %x\n",BASE,img_in1_err);
        }else
        {
            //nothing
        }

    }

    init_img(&img_out1);
    luminance(&img_in1,&img_out1);
    img_out1_err = write_img(IMG_LUM,&img_out1);
    if(img_out1_err == NO_ERROR)
    {
        //printf("1\t%d\t%d\n",movement.x,movement_origin.x);
    }else
    {
        printf("write error\n");
    }


    finish = clock();
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    printf( "duration : %f seconds\n", duration );

    return 0;
}
