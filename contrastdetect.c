#include "includes.h"

extern unsigned char g_debug_mode;

unsigned char search_contrast(unsigned char tolerance, t_img * img_in1,t_img * img_out,unsigned long RGB, unsigned long color,unsigned char direction)
{
    unsigned char ret = NO_ERROR;
    int i,j,limit_he,limit_wi;
    unsigned int raw_tolerance;
    unsigned char next_i,next_j;

    raw_tolerance = ((PIXEL_8bit_RANGE)*tolerance)/100;

    //Write Header
    for(i=0;i<img_in1->FileHeader_size;i++)
    {
        img_out->FileHeader[i] = img_in1->FileHeader[i];
    }
    img_out->signature = img_in1->signature;
    img_out->depth = img_in1->depth;
    img_out->wi = img_in1->wi;
    img_out->he = img_in1->he;
    img_out->FileHeader_size = img_in1->FileHeader_size;

    if (direction == HOR)
    {
        limit_wi = img_in1->wi - 1;
        limit_he = img_in1->he ;
        next_i = 0;
        next_j = 1;
    }else if (direction == VER)
    {
        limit_wi = img_in1->wi ;
        limit_he = img_in1->he - 1;
        next_i = 1;
        next_j = 0;
    }else if (direction == (HOR|VER))
    {
        limit_wi = img_in1->wi - 1;
        limit_he = img_in1->he - 1;
        next_i = 1;
        next_j = 1;
    }else
    {
        limit_wi = 0;
        limit_he = 0;
        next_i = 0;
        next_j = 0;
    }

    for(i=0;i< (limit_he ) ;i++)
    {
        for(j=0 ; (j< limit_wi );j++)
        {
            if ((color & BLUE)!=0)
            {

                img_out->Blue[i][j] = abs((int)(img_in1->Blue[i][j])  - (int)(img_in1->Blue[i+next_i][j+next_j]));
                if (img_out->Blue[i][j] > raw_tolerance)
                {
                    ret |= DIFF_BLUE;
                    img_out->Blue[i][j] = GetBlue(RGB);
                }else
                {
                    img_out->Blue[i][j] = 0;
                }
            }else
            {
                img_out->Blue[i][j] = 0;
            }

            if ((color & GREEN)!=0)
            {

                img_out->Green[i][j] = abs((int)(img_in1->Green[i][j]) - (int)(img_in1->Green[i+next_i][j+next_j]));
                if (img_out->Green[i][j] > raw_tolerance)
                {
                    ret |= DIFF_GREEN;
                    img_out->Green[i][j] = GetGreen(RGB);
                }else
                {
                    img_out->Green[i][j] = 0;
                }
            }else
            {
                img_out->Green[i][j] = 0;
            }

            if ((color & RED)!=0)
            {

                img_out->Red[i][j]   = abs((int)(img_in1->Red[i][j])   - (img_in1->Red[i+next_i][j+next_j]));
                if (img_out->Red[i][j] > raw_tolerance)
                {
                    ret |= DIFF_RED;
                    img_out->Red[i][j] = GetRed(RGB);
                }else
                {
                    img_out->Red[i][j] = 0;
                }
            }else
            {
                img_out->Red[i][j] = 0;
            }
        }

    }
    return ret;
}
