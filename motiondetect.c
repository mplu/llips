#include "includes.h"

extern unsigned char g_debug_mode;

unsigned char search_diff(unsigned char tolerance,unsigned short quantity, t_img * img_in1,t_img * img_in2,t_img * img_out,t_area * change_img)
{
    unsigned char ret = NO_DIFF;
    int i,j;
    unsigned int raw_tolerance,raw_quantity;
    unsigned int quantity_of_diff_pixel = 0;

    // calculte raw tolerance and quantity
    raw_quantity = ((img_in1->he * img_in1->wi)*quantity)/1000;
    raw_tolerance = ((PIXEL_8bit_RANGE)*tolerance)/100;

    if((img_in1->he == img_in2->he) && (img_in1->wi == img_in2->wi))
    {
        init_area(change_img,img_in1->wi,img_in1->he);

        //Write Header
        for(i=0;i<img_in2->FileHeader_size;i++)
        {
            img_out->FileHeader[i] = img_in2->FileHeader[i];
        }
        img_out->signature = img_in2->signature;
        img_out->depth = img_in2->depth;
        img_out->wi = img_in2->wi;
        img_out->he = img_in2->he;
        img_out->FileHeader_size = img_in2->FileHeader_size;

        //same size
        for(i=0;i< img_in1->he ;i++)
        {
            for(j=0 ; j< img_in1->wi ;j++)
            {
                img_out->Blue[i][j]  = abs((int)(img_in1->Blue[i][j])  - (int)(img_in2->Blue[i][j]));
                if (img_out->Blue[i][j] > raw_tolerance)
                {
                    ret |= DIFF_BLUE;
                }
                img_out->Green[i][j] = abs((int)(img_in1->Green[i][j]) - (int)(img_in2->Green[i][j]));
                if (img_out->Green[i][j] > raw_tolerance)
                {
                    ret |= DIFF_GREEN;
                }
                img_out->Red[i][j]   = abs((int)(img_in1->Red[i][j])   - (img_in2->Red[i][j]));
                if (img_out->Red[i][j] > raw_tolerance)
                {
                    ret |= DIFF_RED;
                }

                if ((img_out->Blue[i][j] > raw_tolerance) || (img_out->Green[i][j] > raw_tolerance) || (img_out->Red[i][j] > raw_tolerance))
                {
                    img_out->Green[i][j] = (unsigned char)(float)img_in2->Blue[i][j]*2.5;
                    img_out->Blue[i][j] = img_in2->Blue[i][j]/2;
                    img_out->Red[i][j] = img_in2->Red[i][j]/2;
                    quantity_of_diff_pixel ++;

                    /* identify area of change */
                    change_img->BotLeft.x   = min(change_img->BotLeft.x,j);
                    change_img->BotLeft.y   = min(change_img->BotLeft.y,i);
                    change_img->BotRight.x  = max(change_img->BotRight.x,j);
                    change_img->BotRight.y  = min(change_img->BotRight.y,i);
                    change_img->TopLeft.x   = min(change_img->TopLeft.x,j);
                    change_img->TopLeft.y   = max(change_img->TopLeft.y,i);
                    change_img->TopRight.x  = max(change_img->TopRight.x,j);
                    change_img->TopRight.y  = max(change_img->TopRight.y,i);

                }else
                {
                    img_out->Green[i][j] = img_in2->Green[i][j];
                    img_out->Blue[i][j] = img_in2->Blue[i][j];
                    img_out->Red[i][j] = img_in2->Red[i][j];
                }
            }
        }
    }else
    {
        // size different
        ret |= DIFF_SIZE;
    }
    if (quantity_of_diff_pixel > raw_quantity)
    {
        ret |= DIFF_HIGH_QUANTITY;
    }
    if (g_debug_mode == DEF_ENABLED)
    {
        printf("raw_tolerance : %d\n",raw_tolerance);
        printf("quantity_of_diff_pixel : %d/%d, threshold at %d\n",quantity_of_diff_pixel,(int)(img_in1->he * img_in1->wi),(int)raw_quantity);
    }else{/*nothing*/}


    highlight_area(img_out,change_img,SetRGB(255,0,0));

    return ret;
}


t_vect evaluate_move(t_img * p_img_base,t_img * p_img_target,t_area area_1,t_area area_2, t_vect * p_movement_origin)
{
    t_vect ret = {0};
    t_pixel temporary_pixel_1,temporary_pixel_2,temporary_pixel_3;
    t_area area_1_center,area_2_center;
    int i;

    //Write Header for target img, base on base img data
    for(i=0;i<p_img_base->FileHeader_size;i++)
    {
        p_img_target->FileHeader[i] = p_img_base->FileHeader[i];
    }
    p_img_target->signature = p_img_base->signature;
    p_img_target->depth = p_img_base->depth;
    p_img_target->wi = p_img_base->wi;
    p_img_target->he = p_img_base->he;
    p_img_target->FileHeader_size = p_img_base->FileHeader_size;


    temporary_pixel_1.x = (area_1.BotLeft.x + area_1.TopRight.x)/2 ;
    temporary_pixel_1.y = (area_1.BotLeft.y + area_1.TopRight.y)/2 ;

    temporary_pixel_2.x = (area_2.BotLeft.x + area_2.TopRight.x)/2 ;
    temporary_pixel_2.y = (area_2.BotLeft.y + area_2.TopRight.y)/2 ;

    temporary_pixel_3.x = p_img_base->wi / 2;
    temporary_pixel_3.y = p_img_base->he / 2;

    area_1_center.TopLeft.x    = temporary_pixel_1.x - 1;
    area_1_center.TopLeft.y    = temporary_pixel_1.y + 1;
    area_1_center.TopRight.x   = temporary_pixel_1.x + 1;
    area_1_center.TopRight.y   = temporary_pixel_1.y + 1;
    area_1_center.BotLeft.x    = temporary_pixel_1.x - 1;
    area_1_center.BotLeft.y    = temporary_pixel_1.y - 1;
    area_1_center.BotRight.x   = temporary_pixel_1.x + 1;
    area_1_center.BotRight.y   = temporary_pixel_1.y - 1;

    area_2_center.TopLeft.x    = temporary_pixel_2.x - 1;
    area_2_center.TopLeft.y    = temporary_pixel_2.y + 1;
    area_2_center.TopRight.x   = temporary_pixel_2.x + 1;
    area_2_center.TopRight.y   = temporary_pixel_2.y + 1;
    area_2_center.BotLeft.x    = temporary_pixel_2.x - 1;
    area_2_center.BotLeft.y    = temporary_pixel_2.y - 1;
    area_2_center.BotRight.x   = temporary_pixel_2.x + 1;
    area_2_center.BotRight.y   = temporary_pixel_2.y - 1;

    highlight_area(p_img_target,&area_1,SetRGB(255,0,0));
    highlight_area(p_img_target,&area_1_center,SetRGB(255,0,0));
    highlight_area(p_img_target,&area_2,SetRGB(0,255,0));
    highlight_area(p_img_target,&area_2_center,SetRGB(0,255,0));

    ret = pixels_to_vector(temporary_pixel_1,temporary_pixel_2);
    *p_movement_origin = pixels_to_vector(temporary_pixel_3,temporary_pixel_1);

    return ret;
}


