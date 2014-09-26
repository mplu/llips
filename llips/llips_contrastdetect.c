/********************************************//**
 * \file
 * \brief About contrast and edge detection on image
 * \author MPE
 *
 ***********************************************/
/* ***************************************************************/
/*            Includes                                           */
/* ***************************************************************/
#include "llips_includes.h"

/* ***************************************************************/
/*           Global variables                                    */
/* ***************************************************************/

/* ***************************************************************/
/*           Functions                                           */
/* ***************************************************************/

/********************************************//**
 * \brief Search constrast between two adjacent pixel
 *
 * \param tolerance CPU_CHAR - tolerance in % between two pixel
 * \param img_in1 t_img* - input image
 * \param img_out t_img* - output image
 * \param RGB CPU_INT32U - replacement color for detected pixel
 * \param color CPU_INT32U - range of color to work on
 * \param direction CPU_CHAR - HOR,VER or DIAG to indicate direction of detection
 * \return CPU_CHAR - status of search
 *
 * Search constrast between two adjacent pixel on a given
 * direction, according to a color
 ***********************************************/
CPU_CHAR search_contrast(CPU_CHAR tolerance, t_img * img_in1,t_img * img_out,CPU_INT32U RGB, CPU_INT32U color,CPU_CHAR direction)
{
    CPU_CHAR ret = ERR_NONE;
    CPU_INT32S i,j,limit_he,limit_wi;
    CPU_INT32U raw_tolerance;
    CPU_CHAR next_i,next_j;

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

                img_out->Blue[i][j] = abs((CPU_INT16S)(img_in1->Blue[i][j])  - (CPU_INT16S)(img_in1->Blue[i+next_i][j+next_j]));
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

                img_out->Green[i][j] = abs((CPU_INT16S)(img_in1->Green[i][j]) - (CPU_INT16S)(img_in1->Green[i+next_i][j+next_j]));
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

                img_out->Red[i][j]   = abs((CPU_INT16S)(img_in1->Red[i][j])   - (CPU_INT16S)(img_in1->Red[i+next_i][j+next_j]));
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


/********************************************//**
 * \brief Search block of predefined color
 * \warning NOT FINISH, may be for later use
 * \param img_in1 t_img* - input image
 * \param img_out t_img* - output image
 * \param colorvalue CPU_INT32U - pattern to look for
 * \param colortype CPU_INT32U - color range
 * \param sizeofsearch CPU_INT08U - size of area
 * \param areaofsearch t_simplearea - serach area
 * \return CPU_VOID
 *
 * Search in a part "areaofsearch" in "img_in1", a square "sizeofsearch" containing value higher than "colorvalue", on "colortype" only.
 ***********************************************/
CPU_VOID analyse_result(t_img * img_in1,t_img * img_out,CPU_INT32U colorvalue, CPU_INT32U colortype,CPU_INT16S sizeofsearch,t_simplearea* areaofsearch)
{
    CPU_INT32S i,j,ilocal,jlocal;
    CPU_INT16S halfsize;
    CPU_INT16S occurence = 0;

    if(sizeofsearch%2!=1)
    {
        sizeofsearch ++;
    }
    halfsize = sizeofsearch>>2;

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

    if(areaofsearch == NULL)
    {
        areaofsearch->BotLeft.x = 0;
        areaofsearch->BotLeft.y = 0;

        areaofsearch->TopRight.x = img_out->wi;
        areaofsearch->TopRight.y = img_out->he;
    }

    for(i=areaofsearch->BotLeft.y ; i< (img_in1->he - areaofsearch->TopRight.y ) ;i=i+halfsize)
    {
        for(j=areaofsearch->BotLeft.x ; j< (img_in1->wi -areaofsearch->TopRight.x ) ;j=j+halfsize)
        {
            // lets search around pixel
            for(ilocal= i - halfsize ; ilocal< (i + halfsize ) ;ilocal++)
            {
                for(jlocal= j - halfsize ; jlocal< (j + halfsize ) ;jlocal++)
                {

                    if ((colortype & BLUE)!=0)
                    {
                        if(img_in1->Blue[i][j]>GetBlue(colorvalue) )
                        {
                            occurence ++;
                        }
                    }

                    if ((colortype & GREEN)!=0)
                    {
                        if(img_in1->Blue[i][j]>GetBlue(colorvalue) )
                        {
                            occurence ++;
                        }
                    }

                    if ((colortype & RED)!=0)
                    {
                        if(img_in1->Blue[i][j]>GetBlue(colorvalue) )
                        {
                            occurence ++;
                        }
                    }
                }
            }

            if (occurence > sizeofsearch * sizeofsearch)
            {
                // color zone if ok
                for(ilocal= i - halfsize ; ilocal< (i + halfsize ) ;ilocal++)
                {
                    for(jlocal= j - halfsize ; jlocal< (j + halfsize ) ;jlocal++)
                    {

                        if ((colortype & BLUE)!=0)
                        {
                            img_out->Blue[i][j] = 200;
                        }else
                        {
                            img_out->Blue[i][j] = 0;
                        }

                        if ((colortype & GREEN)!=0)
                        {
                            img_out->Green[i][j] = 200;
                        }else
                        {
                            img_out->Green[i][j] = 0;
                        }

                        if ((colortype & RED)!=0)
                        {
                            img_out->Red[i][j] = 200;
                        }else
                        {
                            img_out->Red[i][j] = 0;
                        }
                    }

                }
            }



        }

    }

}

