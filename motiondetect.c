#include "includes.h"

extern unsigned char g_debug_mode;

unsigned char search_diff(unsigned char tolerance,unsigned short quantity, t_img * img_in1,t_img * img_in2,t_img * img_out)
{
    unsigned char ret = NO_DIFF;
    int i,j;
    unsigned int raw_tolerance,raw_quantity;
    unsigned int quantity_of_diff_pixel = 0;

    t_area change_img1;

    // calculte raw tolerance and quantity
    raw_quantity = ((img_in1->he * img_in1->wi)*quantity)/1000;
    raw_tolerance = ((PIXEL_8bit_RANGE)*tolerance)/100;

    if((img_in1->he == img_in2->he) && (img_in1->wi == img_in2->wi))
    {
        init_area(&change_img1,img_in1->wi,img_in1->he);

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
                    change_img1.BotLeft.x   = min(change_img1.BotLeft.x,j);
                    change_img1.BotLeft.y   = min(change_img1.BotLeft.y,i);
                    change_img1.BotRight.x  = max(change_img1.BotRight.x,j);
                    change_img1.BotRight.y  = min(change_img1.BotRight.y,i);
                    change_img1.TopLeft.x   = min(change_img1.TopLeft.x,j);
                    change_img1.TopLeft.y   = max(change_img1.TopLeft.y,i);
                    change_img1.TopRight.x  = max(change_img1.TopRight.x,j);
                    change_img1.TopRight.y  = max(change_img1.TopRight.y,i);

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


    highlight_area(img_out,&change_img1,SetRGB(255,0,0));

    return ret;
}



void init_area(t_area * area,unsigned short maxwidth,unsigned short maxheight)
{
    area->BotLeft.x = maxwidth;
    area->BotLeft.y = maxheight;
    area->BotRight.x = 0;
    area->BotRight.y = maxheight;
    area->TopLeft.x = maxwidth;
    area->TopLeft.y = 0;
    area->TopRight.x = 0;
    area->TopRight.y = 0;
}

void highlight_area(t_img * img,t_area * area,unsigned long RGB)
{
    int i,j;

    for(i=0;i< img->he ;i++)
    {
        for(j=0 ; j< img->wi ;j++)
        {
            if  (
                    (((j >= area->BotLeft.x)  && (j <= area->BotRight.x)) && ((i == area->BotLeft.y)  || (i == area->BotRight.y)))
                ||  (((j >= area->TopLeft.x)  && (j <= area->TopRight.x)) && ((i == area->TopLeft.y)  || (i == area->TopRight.y)))
                ||  (((i >= area->BotLeft.y)  && (i <= area->TopLeft.y) ) && ((j == area->BotLeft.x)  || (j == area->TopLeft.x)) )
                ||  (((i >= area->BotRight.y) && (i <= area->TopRight.y)) && ((j == area->BotRight.x) || (j == area->TopRight.x)))
                )
            {
                img->Green[i][j]= GetGreen(RGB);
                img->Blue[i][j] = GetBlue(RGB);
                img->Red[i][j]  = GetRed(RGB);
            }
        }
    }
}

void printf_area(t_area * area)
{
    printf("\n%d,%d\t- \t-\t-\t - %d,%d\n| \t- \t-\t-\t - |\n| \t- \t-\t-\t - |\n%d,%d\t- \t-\t-\t - %d,%d\n\n",
    area->TopLeft.x,
    area->TopLeft.y,
    area->TopRight.x,
    area->TopRight.y,
    area->BotLeft.x,
    area->BotLeft.y,
    area->BotRight.x,
    area->BotRight.y);
}
