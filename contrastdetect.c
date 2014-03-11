#include "includes.h"

extern unsigned char g_debug_mode;

unsigned char search_contrast(unsigned char tolerance, t_img * img_in1,t_img * img_out,unsigned long RGB, unsigned long color)
{
    unsigned char ret = NO_ERROR;
    int i,j;
    unsigned int raw_tolerance;

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

    //
    for(i=0;i< (img_in1->he - 1) ;i++)
    {
        for(j=0 ; (j< img_in1->wi );j++)
        {
            if ((color & BLUE)!=0)
            {

                img_out->Blue[i][j] = abs((int)(img_in1->Blue[i][j])  - (int)(img_in1->Blue[i+1][j]));
                if (img_out->Blue[i][j] > raw_tolerance)
                {
                    ret |= DIFF_BLUE;
                }
            }
            if ((color & GREEN)!=0)
            {

                img_out->Green[i][j] = abs((int)(img_in1->Green[i][j]) - (int)(img_in1->Green[i+1][j]));
                if (img_out->Green[i][j] > raw_tolerance)
                {
                    ret |= DIFF_GREEN;
                }
            }
            if ((color & GREEN)!=0)
            {

                img_out->Red[i][j]   = abs((int)(img_in1->Red[i][j])   - (img_in1->Red[i+1][j]));
                if (img_out->Red[i][j] > raw_tolerance)
                {
                    ret |= DIFF_RED;
                }
            }

            if ((img_out->Blue[i][j] > raw_tolerance) || (img_out->Green[i][j] > raw_tolerance) || (img_out->Red[i][j] > raw_tolerance))
            {
                img_out->Green[i][j] = GetGreen(RGB);
                img_out->Blue[i][j] = GetBlue(RGB);
                img_out->Red[i][j] = GetRed(RGB);
            }else
            {
                img_out->Green[i][j] = 0;//img_in1->Green[i][j];
                img_out->Blue[i][j] = 0;//img_in1->Blue[i][j];
                img_out->Red[i][j] = 0;//img_in1->Red[i][j];
            }
        }

    }
    return ret;
}
