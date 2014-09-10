/****************************************************************/
/* Light Library for Image ProcesS                              */
/* File : llips_general.c                                       */
/* Description :                                                */
/*   About general image handling                               */
/*                                                              */
/* Author : MPE                                                 */
/*                                                              */
/****************************************************************/
/* http://www.commentcamarche.net/contents/1200-bmp-format-bmp
0x0000  Entête du fichier
0x0000  La signature (sur 2 octets), indiquant qu'il s'agit d'un fichier BMP à l'aide des deux caractères.
        BM, 424D en hexadécimal, indique qu'il s'agit d'un Bitmap Windows.
0x0002  La taille totale du fichier en octets (codée sur 4 octets)
0x0006  Un champ réservé (sur 4 octets)
0x000A  L'offset de l'image (sur 4 octets), en français décalage,

0x000E  Entête de l'image
0x000E  La taille de l'entête de l'image en octets (codée sur 4 octets).
        28 pour Windows 3.1x, 95, NT, ...
        0C pour OS/2 1.x
        F0 pour OS/2 2.x
0x0012  La largeur de l'image (sur 4 octets), c'est-à-dire le nombre de pixels horizontalement (en anglais width)
0x0016  La hauteur de l'image (sur 4 octets), c'est-à-dire le nombre de pixels verticalement (en anglais height)
0x001A  Le nombre de plans (sur 2 octets). Cette valeur vaut toujours 1
0x001C  La profondeur de codage de la couleur(sur 2 octets), c'est-à-dire le nombre de bits utilisés pour coder
        la couleur. Cette valeur peut-être égale à 1, 4, 8, 16, 24 ou 32
0x001E  La méthode de compression (sur 4 octets). Cette valeur vaut 0 lorsque l'image n'est pas compressée, ou
        bien 1, 2 ou 3 suivant le type de compression utilisé :
        1 pour un codage RLE de 8 bits par pixel
        2 pour un codage RLE de 4 bits par pixel
        3 pour un codage bitfields, signifiant que la couleur est codé par un triple masque représenté par la palette
0x0022  La taille totale de l'image en octets (sur 4 octets).
0x0026  La résolution horizontale (sur 4 octets), c'est-à-dire le nombre de pixels par mètre horizontalement
0x002A  La résolution verticale (sur 4 octets), c'est-à-dire le nombre de pixels par mètre verticalement
0x002E  Le nombre de couleurs de la palette (sur 4 octets)
0x0032  Le nombre de couleurs importantes de la palette (sur 4 octets). Ce champ peut être égal à 0 lorsque chaque couleur a son importance.

0x0036  Palette de l'image
        La palette est optionnelle. Lorsqu'une palette est définie, elle contient successivement 4 octets pour chacune de ses entrées représentant :
        http://fr.wikipedia.org/wiki/Windows_bitmap
        La composante bleue (sur un octet)
        La composante verte (sur un octet)
        La composante rouge (sur un octet)
        Un champ réservé (sur un octet)

0x0xxx  Codage de l'image (dépend de la palette)
0x0036  avec bmp 24bit windows
        Les images en couleurs réelles utilisent 24 bits par pixel,
        ce qui signifie qu'il faut 3 octets pour coder chaque pixel,
        en prenant soin de respecter l'ordre de l'alternance
        bleu,
        vert
        et rouge.
*/

/****************************************************************/
/**           Includes                                          */
/****************************************************************/
#include "llips_includes.h"

/****************************************************************/
/**           Global variables                                  */
/****************************************************************/

/****************************************************************/
/**           Functions                                         */
/****************************************************************/

/****************************************************************/
/* display_img_value()                                          */
/* Description :                                                */
/*   Display each color of image in ascii                       */
/* Input:                                                       */
/*   img - image to display                                     */
/*   colors - color layer to display                            */
/* Output:                                                      */
/*   na                                                         */
/* Return:                                                      */
/*   na                                                         */
/*                                                              */
/****************************************************************/
CPU_VOID display_img_value(t_img * img,CPU_INT16S colors)
{
    CPU_INT16S i,j;

    if((colors & HEADER) != 0)
    {
        printf("Header size : %d\n",img->FileHeader_size);
        printf("Header\n");
        for(i=0;i<img->FileHeader_size;i++)
        {
           printf("%2x",img->FileHeader[i]);
           if(((i+1)%16)==0)
           {
               printf("\n");
           }
        }
    }
    printf("\n\n");

    if((colors & BLUE) != 0)
    {
        printf("Blue Pixel\n");
        for(i=img->he-1;i>=0;i--)
        {
            for(j=0;j<img->wi;j++)
            {
               printf("%2x",img->Blue[i][j]);
            }
            printf("\n");
        }
        printf("\n");
    }

    if((colors & GREEN) != 0)
    {
        printf("Green Pixel\n");
        for(i=img->he-1;i>=0;i--)
        {
            for(j=0;j<img->wi;j++)
            {
               printf("%2x",img->Green[i][j]);
            }
            printf("\n");
        }
        printf("\n");
    }

    if((colors & RED) != 0)
    {
        printf("Red Pixel\n");
        for(i=img->he-1;i>=0;i--)
        {
            for(j=0;j<img->wi;j++)
            {
               printf("%2x",img->Red[i][j]);
            }
            printf("\n");
        }
        printf("\n");
    }
}

/****************************************************************/
/* highlight_line()                                             */
/* Description :                                                */
/*   Draw a line on image in a given color                      */
/* Input:                                                       */
/*   img - image to modify (also output)                        */
/*   pix1 - first point of line                                 */
/*   pix2 - second point of line                                */
/*   RBG - color of the line                                    */
/* Output:                                                      */
/*   img                                                        */
/* Return:                                                      */
/*   vector of the line                                         */
/*                                                              */
/****************************************************************/
t_vect highlight_line(t_img * img,t_pixel pix1,t_pixel pix2,CPU_INT32U RGB)
{
    t_vect move;

    move.x = pix2.x - pix1.x ;
    move.y = pix2.y - pix1.y ;
    //draw the line... I don't know how...
    return move;
}

/****************************************************************/
/* pixels_to_vector()                                           */
/* Description :                                                */
/*   Convert two pixel into a vector                            */
/* Input:                                                       */
/*   pix1 - starting point of vector                            */
/*   pix2 - ending point of vector                              */
/* Output:                                                      */
/*   na                                                         */
/* Return:                                                      */
/*   vector corresponding between pix1 and pix2                 */
/*                                                              */
/****************************************************************/
t_vect pixels_to_vector(t_pixel pix1,t_pixel pix2)
{
    t_vect move;

    move.x = pix2.x - pix1.x ;
    move.y = pix2.y - pix1.y ;

    return move;
}

/****************************************************************/
/* vectormodule()                                               */
/* Description :                                                */
/*   Convert two pixel into a vector                            */
/* Input:                                                       */
/*   pix1 - starting point of vector                            */
/*   pix2 - ending point of vector                              */
/* Output:                                                      */
/*   na                                                         */
/* Return:                                                      */
/*   vector corresponding between pix1 and pix2                 */
/*                                                              */
/****************************************************************/
CPU_INT16U vectormodule(t_vect vect)
{
    CPU_INT16U module;

    module = (CPU_INT16U)sqrt(vect.x * vect.x + vect.y * vect.y);


    return module;
}

/****************************************************************/
/* pixel_to_area()                                              */
/* Description :                                                */
/*   Convert a pixel into a area                                */
/* Input:                                                       */
/*   pix - pixel                                                */
/* Output:                                                      */
/*   na                                                         */
/* Return:                                                      */
/*   area                                                       */
/*                                                              */
/****************************************************************/
t_area pixel_to_area(t_pixel pix)
{
    t_area ret;
    ret.TopLeft.x  = pix.x;
    ret.TopLeft.y  = pix.y;
    ret.TopRight.x = pix.x;
    ret.TopRight.y = pix.y;
    ret.BotLeft.x  = pix.x;
    ret.BotLeft.y  = pix.y;
    ret.BotRight.x = pix.x;
    ret.BotRight.y = pix.y;

    return ret;
}

/****************************************************************/
/* init_area()                                                  */
/* Description :                                                */
/*   Initialize for corner of an area                           */
/* Input:                                                       */
/*   maxwidth - width of area                                   */
/*   maxheight - height of area                                 */
/* Output:                                                      */
/*   area - pointer to initialized area                         */
/* Return:                                                      */
/*   area                                                       */
/*                                                              */
/****************************************************************/
CPU_VOID init_area(t_area * area,CPU_INT16U maxwidth,CPU_INT16U maxheight)
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

/****************************************************************/
/* highlight_area()                                             */
/* Description :                                                */
/*   Draw a square area on image in a given color               */
/* Input:                                                       */
/*   img - image to modify (also output)                        */
/*   area - area to highlight                                   */
/*   RBG - color of the line                                    */
/* Output:                                                      */
/*   na                                                         */
/* Return:                                                      */
/*   img                                                        */
/*                                                              */
/****************************************************************/
CPU_VOID highlight_area(t_img * img,t_area * area,CPU_INT32U RGB)
{
    CPU_INT16S i,j;

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

/****************************************************************/
/* printf_area()                                                */
/* Description :                                                */
/*   Display area corner coordonate in ascii                    */
/* Input:                                                       */
/*   area - area to display                                     */
/* Output:                                                      */
/*   na                                                         */
/* Return:                                                      */
/*   na                                                         */
/*                                                              */
/****************************************************************/
CPU_VOID printf_area(t_area * area)
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

/****************************************************************/
/* color_filter()                                               */
/* Description :                                                */
/*   Create an image with only the color provided               */
/* Input:                                                       */
/*   img_in - input image                                       */
/*   color - range of color to work on                          */
/* Output:                                                      */
/*   img_out - output image                                     */
/* Return:                                                      */
/*   status of operation                                        */
/*                                                              */
/****************************************************************/
CPU_CHAR color_filter(t_img * img_in,t_img * img_out, CPU_INT32U color)
{
    CPU_CHAR ret = ERR_NONE;
    CPU_INT16S i,j;

    //Write Header
    for(i=0;i<img_in->FileHeader_size;i++)
    {
        img_out->FileHeader[i] = img_in->FileHeader[i];
    }
    img_out->signature = img_in->signature;
    img_out->depth = img_in->depth;
    img_out->wi = img_in->wi;
    img_out->he = img_in->he;
    img_out->FileHeader_size = img_in->FileHeader_size;

    //
    for(i=0;i< (img_in->he ) ;i++)
    {
        for(j=0 ; (j< img_in->wi );j++)
        {
            if ((color & RED)!=0)
            {
                img_out->Red[i][j] = img_in->Red[i][j];
            }else
            {
                img_out->Red[i][j] = 0;
            }
            if ((color & GREEN)!=0)
            {
                img_out->Green[i][j] = img_in->Green[i][j];
            }else
            {
                img_out->Green[i][j] = 0;
            }
            if ((color & BLUE)!=0)
            {
                img_out->Blue[i][j] = img_in->Blue[i][j];
            }else
            {
                img_out->Blue[i][j] = 0;
            }
        }
    }


    return ret;
}

/****************************************************************/
/* histogram()                                                  */
/* Description :                                                */
/*   Create an image with a view of histogram (RGBY). Width and */
/*   precision of histogram depend on image width               */
/* Input:                                                       */
/*   img_in - input image                                       */
/* Output:                                                      */
/*   img_out - output image                                     */
/* Return:                                                      */
/*   status of operation                                        */
/*                                                              */
/****************************************************************/
CPU_CHAR histogram(t_img * img_in,t_img * img_out)
{
    CPU_CHAR ret = ERR_NONE;
    CPU_INT16U i,j;

    CPU_INT32U histowidth ;
    CPU_INT32U histoheigh ;
    CPU_INT32U histo_x = 1 ;
    CPU_INT32U histo_y_red = 1,histo_y_green,histo_y_blue,histo_y_lum ;

    CPU_INT32U * redpixel = calloc((PIXEL_8bit_RANGE+1),sizeof(CPU_INT32U));
    CPU_INT32U * greenpixel = calloc((PIXEL_8bit_RANGE+1),sizeof(CPU_INT32U));
    CPU_INT32U * bluepixel = calloc((PIXEL_8bit_RANGE+1),sizeof(CPU_INT32U));
    CPU_INT32U * luminancepixel = calloc((PIXEL_8bit_RANGE+1),sizeof(CPU_INT32U));
    CPU_CHAR Luminance = 0;

    CPU_INT32U index,histomaxred=0,histomaxgreen=0,histomaxblue=0,histomaxlum =0;

    // configure historgam
    histowidth = (img_in->wi)/4.5;

    if(histowidth>PIXEL_8bit_RANGE)
    {
         histowidth = PIXEL_8bit_RANGE;
    }
    histoheigh = histowidth / 2 ;

    histo_y_green = histo_y_red + histowidth + 2;
    histo_y_blue = histo_y_green + histowidth + 2;
    histo_y_lum = histo_y_blue + histowidth + 2;

    //Write Header
    for(i=0;i<img_in->FileHeader_size;i++)
    {
        img_out->FileHeader[i] = img_in->FileHeader[i];
    }
    img_out->signature = img_in->signature;
    img_out->depth = img_in->depth;
    img_out->wi = img_in->wi;
    img_out->he = img_in->he;
    img_out->FileHeader_size = img_in->FileHeader_size;

    //count distribution of red pixel
    for(i=0;i< (img_in->he ) ;i++)
    {
        for(j=0 ; (j< img_in->wi );j++)
        {
            //getting red pixel
            index = img_in->Red[i][j] / (PIXEL_8bit_RANGE/(histowidth-1));
            redpixel[index]++;
            histomaxred = maxi(histomaxred,redpixel[index]);

            //getting green pixel
            index = img_in->Green[i][j] / (PIXEL_8bit_RANGE/(histowidth-1));
            greenpixel[index]++;
            histomaxgreen = maxi(histomaxgreen,greenpixel[index]);
            //getting red pixel
            index = img_in->Blue[i][j] / (PIXEL_8bit_RANGE/(histowidth-1));
            bluepixel[index]++;
            histomaxblue = maxi(histomaxblue,bluepixel[index]);

            //calcultate luminance Y = 0,299 R + 0,587 G + 0,114 B
            Luminance = (CPU_CHAR)(  (CPU_FP32)img_in->Red[i][j]   * 0.299
                                        + (CPU_FP32)img_in->Green[i][j] * 0.587
                                        + (CPU_FP32)img_in->Blue[i][j]  * 0.114 );

            //getting lumi pixel
            index = Luminance / (PIXEL_8bit_RANGE/(histowidth-1));
            luminancepixel[index]++;
            histomaxlum = maxi(histomaxlum,luminancepixel[index]);
        }
    }

    // normalize histogramm according to highest pixel distribution
    // and height of histogram, and to histogram width
    for(i=0;i<histowidth;i++)
    {
        redpixel[i] = redpixel[i] * histoheigh / histomaxred ;
        greenpixel[i] = greenpixel[i] * histoheigh / histomaxgreen ;
        bluepixel[i] = bluepixel[i] * histoheigh / histomaxblue ;

        luminancepixel[i] = luminancepixel[i] * histoheigh / histomaxblue ;

    }

    //copy input img to output including histogram
    for(i=0;i< (img_in->he ) ;i++)
    {
        for(j=0 ; (j< img_in->wi );j++)
        {
            // Looking for red drawing area
            if(    ((j>=histo_y_red) && (j<=(histo_y_red+histowidth)))
                && ((i>=histo_x) && (i<(histo_x+histoheigh)))
              )
            {
                if(redpixel[j-histo_y_red] >= (i-histo_x))
                {
                    img_out->Red[i][j]   = 255;
                    img_out->Green[i][j] = 0;
                    img_out->Blue[i][j]  = 0;
                }else
                {
                    img_out->Red[i][j]   = (img_in->Red[i][j])/2;
                    img_out->Green[i][j] = (img_in->Green[i][j])/2;
                    img_out->Blue[i][j]  = (img_in->Blue[i][j])/2;
                }
            }// Looking for green drawing area
            else if(   ((j>=histo_y_green) && (j<=(histo_y_green+histowidth)))
                    && ((i>=histo_x) && (i<(histo_x+histoheigh)))
                   )
            {
                if(greenpixel[j-histo_y_green] >= (i-histo_x))
                {
                    img_out->Red[i][j]   = 0;
                    img_out->Green[i][j] = 255;
                    img_out->Blue[i][j]  = 0;
                }else
                {
                    img_out->Red[i][j]   = (img_in->Red[i][j])/2;
                    img_out->Green[i][j] = (img_in->Green[i][j])/2;
                    img_out->Blue[i][j]  = (img_in->Blue[i][j])/2;
                }
            }// Looking for blue drawing area
            else if(   ((j>=histo_y_blue) && (j<=(histo_y_blue+histowidth)))
                    && ((i>=histo_x) && (i<(histo_x+histoheigh)))
                   )
            {
                if(bluepixel[j-histo_y_blue] >= (i-histo_x))
                {
                    img_out->Red[i][j]   = 0;
                    img_out->Green[i][j] = 0;
                    img_out->Blue[i][j]  = 255;
                }else
                {
                    img_out->Red[i][j]   = (img_in->Red[i][j])/2;
                    img_out->Green[i][j] = (img_in->Green[i][j])/2;
                    img_out->Blue[i][j]  = (img_in->Blue[i][j])/2;
                }
            }// looking for lum drawing area
            else if(   ((j>=histo_y_lum) && (j<=(histo_y_lum+histowidth)))
                    && ((i>=histo_x) && (i<(histo_x+histoheigh)))
                   )
            {
                if(luminancepixel[j-histo_y_lum] >= (i-histo_x))
                {
                    img_out->Red[i][j]   = 180;
                    img_out->Green[i][j] = 180;
                    img_out->Blue[i][j]  = 180;
                }else
                {
                    img_out->Red[i][j]   = (img_in->Red[i][j])/2;
                    img_out->Green[i][j] = (img_in->Green[i][j])/2;
                    img_out->Blue[i][j]  = (img_in->Blue[i][j])/2;
                }
            }
            else
            {
                img_out->Red[i][j]   = img_in->Red[i][j];
                img_out->Green[i][j] = img_in->Green[i][j];
                img_out->Blue[i][j]  = img_in->Blue[i][j];
            }
        }
    }




    return ret;
}

/****************************************************************/
/* luminance()                                                  */
/* Description :                                                */
/*   Create an image only luminance data (desaturated image)    */
/* Input:                                                       */
/*   img_in - input image                                       */
/* Output:                                                      */
/*   img_out - output image                                     */
/* Return:                                                      */
/*   status of operation                                        */
/*                                                              */
/****************************************************************/
CPU_CHAR luminance(t_img * img_in,t_img * img_out)
{
    CPU_CHAR ret = ERR_NONE;
    CPU_INT16S i,j;

    CPU_CHAR Luminance = 0;

    //Write Header
    for(i=0;i<img_in->FileHeader_size;i++)
    {
        img_out->FileHeader[i] = img_in->FileHeader[i];
    }
    img_out->signature = img_in->signature;
    img_out->depth = img_in->depth;
    img_out->wi = img_in->wi;
    img_out->he = img_in->he;
    img_out->FileHeader_size = img_in->FileHeader_size;

    for(i=0;i< (img_in->he ) ;i++)
    {
        for(j=0 ; (j< img_in->wi );j++)
        {
            //calcultate luminance Y = 0,299 R + 0,587 G + 0,114 B
            Luminance = (CPU_CHAR)(  (CPU_FP32)img_in->Red[i][j]   * 0.299
                                        + (CPU_FP32)img_in->Green[i][j] * 0.587
                                        + (CPU_FP32)img_in->Blue[i][j]  * 0.114 );

            img_out->Red[i][j] = Luminance;
            img_out->Green[i][j] = Luminance;
            img_out->Blue[i][j] = Luminance;
        }
    }
    return ret;
}


/****************************************************************/
/* apply_linfilter()                                               */
/* Description :                                                */
/*   smooth image                                               */
/* Input:                                                       */
/*   img_in - input image                                       */
/* Output:                                                      */
/*   img_out - output image                                     */
/* Return:                                                      */
/*   status of operation                                        */
/*                                                              */
/****************************************************************/
CPU_CHAR apply_linfilter(t_img * img_in,CPU_FP64 ** tab_filtre,CPU_INT16S filtersize,CPU_INT32U color,t_img * img_out)
{

    CPU_CHAR ret = ERR_NONE;
    CPU_INT16S i,j,i_img,j_img;
    CPU_FP64 one_pixel[c_color_size];
    CPU_INT16S filter_range =(filtersize -1)/2;

    //Write Header
    for(i=0;i<img_in->FileHeader_size;i++)
    {
        img_out->FileHeader[i] = img_in->FileHeader[i];
    }
    img_out->signature = img_in->signature;
    img_out->depth = img_in->depth;
    img_out->wi = img_in->wi;
    img_out->he = img_in->he;
    img_out->FileHeader_size = img_in->FileHeader_size;

    //printf("%f\n",tab_filtre[0][0]);

    for(i_img=0;i_img< (img_in->he ) ;i_img++)
    {

        for(j_img=0 ; (j_img< img_in->wi );j_img++)
        {

            one_pixel[c_Red] = 0;
            one_pixel[c_Green] = 0;
            one_pixel[c_Blue] = 0;
            for (i=0;i<filtersize;i++)
            {
                //printf("one_pixel[c_Red]1 %f\n",one_pixel[c_Red]);
                for (j=0;j<filtersize;j++)
                {
                    if(     ((i_img-filter_range+i )>=0)
                        &&  ((j_img-filter_range+j )>=0)
                        &&  ((i_img-filter_range+i )<img_in->he)
                        &&  ((j_img-filter_range+j )<img_in->wi )
                      )
                    {
                        if ((color & BLUE)!=0)
                        {
                            one_pixel[c_Blue] = one_pixel[c_Blue]
                                                + (CPU_FP64)(img_in->Blue[i_img-filter_range+i][j_img-filter_range+j]) * tab_filtre[i][j] ;
                        }else
                        {
                            one_pixel[c_Blue]= 0;
                        }
                        if ((color & RED)!=0)
                        {
                            one_pixel[c_Red] =  one_pixel[c_Red]
                                                + (CPU_FP64)(img_in->Red[i_img-filter_range+i][j_img-filter_range+j]) * tab_filtre[i][j] ;
                        }else
                        {
                            one_pixel[c_Red] = 0;
                        }
                        if ((color & GREEN)!=0)
                        {
                            one_pixel[c_Green]= one_pixel[c_Green]
                                                + (CPU_FP64)(img_in->Green[i_img-filter_range+i][j_img-filter_range+j]) * tab_filtre[i][j] ;
                        }else
                        {
                            one_pixel[c_Green] = 0;
                        }
                    }
                }
            }
            img_out->Red[i_img][j_img] = one_pixel[c_Red] ;
            img_out->Green[i_img][j_img] = one_pixel[c_Green] ;
            img_out->Blue[i_img][j_img] = one_pixel[c_Blue] ;
        }
    }
    return ret;
}

/****************************************************************/
/* conv_gauss()                                                 */
/* Description :                                                */
/*   gaussian convolution                                       */
/* Input:                                                       */
/*                                                              */
/* Output:                                                      */
/*                                                              */
/* Return:                                                      */
/*   status of operation                                        */
/*                                                              */
/****************************************************************/
CPU_FP64 conv_gauss(CPU_INT16S x,CPU_INT16S y,CPU_FP64 sig)
{
    CPU_FP64 value = 0,value1 = 0,value2 = 0;
    //x++;
    //y++;


    value1 =  1.0/(2.0*PI*sig*sig);
    value2 =   (0.0 -(x*x+y*y))/(2*sig*sig);

    //value1 =  (PI*sig*sig)/2.0;
    //value2 =  ((x*x+y*y)/(2*sig*sig));

    //value1 =  1.0/(sig*sqrt(2*PI));
    //value2 =   ( 0.0 -((x*x)+(y*y)) / (2*sig*sig) );

    value = value1 * exp(value2);
    return value;

}

/****************************************************************/
/* create_gauss_filter()                                        */
/* Description :                                                */
/*   gaussian convolution                                       */
/* Input:                                                       */
/*                                                              */
/* Output:                                                      */
/*                                                              */
/* Return:                                                      */
/*   status of operation                                        */
/*                                                              */
/****************************************************************/
void create_gauss_filter(CPU_FP64 ** tab_filtre,CPU_INT16S filtersize,CPU_FP64 sigma)
{
    CPU_INT16U filter_range =(filtersize -1)/2;
    CPU_INT16S i,j;

    CPU_FP64 value,sum=0,sum2=0;
    for (i=0;i<filtersize;i++)
    {
        for (j=0;j<filtersize;j++)
        {
            value = conv_gauss(i-filter_range,j-filter_range,sigma);
            sum = sum + value;
            tab_filtre[i][j] = value;
        }
    }

    for (i=0;i<filtersize;i++)
    {
        for (j=0;j<filtersize;j++)
        {
            tab_filtre[i][j] = tab_filtre[i][j] / sum;
            sum2 = sum2 + tab_filtre[i][j];
        }
    }
}

/****************************************************************/
/* create_average_filter()                                      */
/* Description :                                                */
/*   gaussian convolution                                       */
/* Input:                                                       */
/*                                                              */
/* Output:                                                      */
/*                                                              */
/* Return:                                                      */
/*   status of operation                                        */
/*                                                              */
/****************************************************************/
void create_average_filter(CPU_FP64 ** tab_filtre,CPU_INT16S filtersize)
{
    CPU_INT16S i,j;
    for (i=0;i<filtersize;i++)
    {
        for (j=0;j<filtersize;j++)
        {
            tab_filtre[i][j] = 1.0/filtersize;
        }
    }
}

/****************************************************************/
/* create_laplacian_filter()                                       */
/* Description :                                                */
/*   gaussian convolution                                       */
/* Input:                                                       */
/*                                                              */
/* Output:                                                      */
/*                                                              */
/* Return:                                                      */
/*   status of operation                                        */
/*                                                              */
/****************************************************************/
CPU_INT16S create_laplacian_filter(CPU_FP64 ** tab_filtre,CPU_INT16S filtersize)
{
    CPU_INT16S i,j;
    for (i=0;i<filtersize;i++)
    {
        for (j=0;j<filtersize;j++)
        {
            tab_filtre[i][j] = 0.5/8.0;
        }
    }

    tab_filtre[1][1] = -0.5;

/*    tab_filtre[0][0] = 0;
    tab_filtre[0][1] = 1;
    tab_filtre[0][2] = 0;

    tab_filtre[1][0] = 1;

    tab_filtre[1][2] = 1;

    tab_filtre[2][0] = 0;
    tab_filtre[2][1] = 1;
    tab_filtre[2][2] = 0;
*/


    return filtersize;
}

/****************************************************************/
/* apply_median_filter()                                        */
/* Description :                                                */
/*   smooth image                                               */
/* Input:                                                       */
/*   img_in - input image                                       */
/* Output:                                                      */
/*   img_out - output image                                     */
/* Return:                                                      */
/*   status of operation                                        */
/*                                                              */
/****************************************************************/
CPU_CHAR apply_median_filter(t_img * img_in,CPU_INT16S filtersize,t_img * img_out)
{

    CPU_CHAR ret = ERR_NONE;
    CPU_INT16S i,i_img,j_img;
    CPU_INT16S filter_range =0;

    if(filtersize%2 != 1)
    {
        filtersize ++;
    }
    filter_range = (filtersize -1)/2;

    //Write Header
    for(i=0;i<img_in->FileHeader_size;i++)
    {
        img_out->FileHeader[i] = img_in->FileHeader[i];
    }
    img_out->signature = img_in->signature;
    img_out->depth = img_in->depth;
    img_out->wi = img_in->wi;
    img_out->he = img_in->he;
    img_out->FileHeader_size = img_in->FileHeader_size;

    //printf("%f\n",tab_filtre[0][0]);

    for(i_img=0;i_img< (img_in->he ) ;i_img++)
    {

        for(j_img=0 ; (j_img< img_in->wi );j_img++)
        {
            if(     ((i_img-filter_range )>=0)
                &&  ((j_img-filter_range )>=0)
                &&  ((i_img-filter_range )<img_in->he)
                &&  ((j_img-filter_range )<img_in->wi )
              )
            {

                img_out->Red[i_img][j_img]  = get_median(img_in->Red,filter_range,i_img,j_img);
                img_out->Green[i_img][j_img]= get_median(img_in->Green,filter_range,i_img,j_img);
                img_out->Blue[i_img][j_img] = get_median(img_in->Blue,filter_range,i_img,j_img);

            }
        }
    }
    return ret;
}

CPU_INT08U get_median(CPU_INT08U ** table2D,CPU_INT16S filter_range,CPU_INT16S i, CPU_INT16S j)
{
    CPU_INT08U median;
    CPU_INT16U ii,jj;
    CPU_INT08U *table = (CPU_INT08U *)malloc(sizeof(CPU_INT08U)*(filter_range*2+1)*(filter_range*2+1));
    for(ii =(0 - filter_range);ii<= filter_range;ii++)
    {
        for(jj =(0 - filter_range);jj<= filter_range;jj++)
        {
            table[ii+jj] = table2D[i+ii][j+jj];
            printf("ii %d jj %d i %d j %d\n",ii,jj,i,j);
        }
    }
    printf("plop\n");
    qsort (table, (filter_range*2+1)*(filter_range*2+1), sizeof(CPU_INT08U), compare);
    median = table[(filter_range*2+1)];
    return median;
}
