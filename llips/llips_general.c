/********************************************//**
 * \file
 * \brief About general image handling
 * \author MPE
 *
 ***********************************************/
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

/* ***************************************************************/
/* *           Includes                                          */
/* ***************************************************************/
#include "llips_includes.h"

/* ***************************************************************/
/* *           Global variables                                  */
/* ***************************************************************/
//Predefined laplacian filter
const CPU_FP64 lapl_filter_1[3][3] = {{ 0,-1, 0},
                                      {-1, 4,-1},
                                      { 0,-1, 0}};

const CPU_FP64 lapl_filter_2[3][3] ={{0, 1, 0},
                                     {1,-4, 1},
                                     {0, 1, 0}};

const CPU_FP64 lapl_filter_3[3][3] = {{-1,-1,-1},
                                      {-1, 8,-1},
                                      {-1,-1,-1}};

const CPU_FP64 lapl_filter_4[3][3] = {{ 1,-2, 1},
                                      {-2, 4,-2},
                                      { 1,-2, 1}};

const CPU_FP64 lapl_filter_5[5][5] = {  { 0, 0,-1, 0, 0},
                                        { 0,-1,-2,-1, 0},
                                        {-1,-2,16,-2,-1},
                                        { 0,-1,-2,-1, 0},
                                        { 0, 0,-1, 0, 0}};
/* ***************************************************************/
/* *           Functions                                         */
/* ***************************************************************/

/********************************************//**
 * \brief Display each color of image in ascii
 *
 * \param img t_img* - image to display
 * \param colors CPU_INT16S - color layer to display
 * \return CPU_VOID
 *
 ***********************************************/
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


/********************************************//**
 * \brief Draw a line on image in a given color
 * \warning NOT FUNCTIONNAL
 * \param img t_img* - image to modify (also output)
 * \param pix1 t_pixel - first point of line
 * \param pix2 t_pixel - second point of line
 * \param RGB CPU_INT32U - color of the line
 * \return t_vect - vector representing the line
 *
 * Needs to be done
 * Helpfull macro to define color is SetRGB(r,g,b), with value between 0 and 255
 ***********************************************/
t_vect highlight_line(t_img * img,t_pixel pix1,t_pixel pix2,CPU_INT32U RGB)
{
    t_vect move;

    move.x = pix2.x - pix1.x ;
    move.y = pix2.y - pix1.y ;
    //draw the line... I don't know how...
    return move;
}



/********************************************//**
 * \brief Convert two pixel into a vector
 *
 * \param pix1 t_pixel - starting point of vector
 * \param pix2 t_pixel - ending point of vector
 * \return t_vect - corresponding vector between pix1 and pix2
 *
 ***********************************************/
t_vect pixels_to_vector(t_pixel pix1,t_pixel pix2)
{
    t_vect move;

    move.x = pix2.x - pix1.x ;
    move.y = pix2.y - pix1.y ;

    return move;
}


/********************************************//**
 * \brief Give module of a vector, in "pixel"
 *
 * \param vect t_vect - vector to process
 * \return CPU_INT16U - module of the vector
 *
 ***********************************************/
CPU_INT16U vectormodule(t_vect vect)
{
    CPU_INT16U module;

    module = (CPU_INT16U)sqrt(vect.x * vect.x + vect.y * vect.y);


    return module;
}


/********************************************//**
 * \brief Convert a pixel into a area of 1 x 1
 *
 * \param pix t_pixel - pixel
 * \return t_area - corresponding area
 *
 * This function is use to be able to draw a pixel using highlight_area() function
 ***********************************************/
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



/********************************************//**
 * \brief Initialize an area from 0,0 to given values
 *
 * \param area t_area* - pointer to initialized area
 * \param maxwidth CPU_INT16U - width of area
 * \param maxheight CPU_INT16U - height of area
 * \return CPU_VOID
 *
 ***********************************************/
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


/********************************************//**
 * \brief Draw a rectangular area on image in a given color
 *
 * \param img t_img* - image to modify (also output)
 * \param area t_area* - area to highlight
 * \param RGB CPU_INT32U - highlight color
 * \return CPU_VOID
 *
 * Helpfull macro to define color is SetRGB(r,g,b), with value between 0 and 255
 ***********************************************/
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


/********************************************//**
 * \brief Display area corner coordonate in a console
 *
 * \param area t_area* - area to display
 * \return CPU_VOID
 *
 ***********************************************/
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


/********************************************//**
 * \brief Create an image with only the color provided
 *
 * \param img_in t_img* - input image
 * \param img_out t_img* - output image
 * \param color CPU_INT32U - range of color to work on
 * \return CPU_CHAR error
 *
 * The parameter color must be a combination of RED | GREEN | BLUE
 ***********************************************/
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


/********************************************//**
 * \brief Create an image with a view of histogram (RGBY).
 *
 * \param img_in t_img* - input image
 * \param img_out t_img* - output image
 * \return CPU_CHAR - status of operation
 *
 * Width and precision of histogram depend on image width
 ***********************************************/
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


/********************************************//**
 * \brief Create an image only luminance data (desaturated image)
 *
 * \param img_in t_img* - input image
 * \param img_out t_img* - output image
 * \return CPU_CHAR - status of operation
 *
 ***********************************************/
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



/********************************************//**
 * \brief Apply a predefined filter on the image
 *
 * \param img_in t_img* - input image
 * \param tab_filtre CPU_FP64** - pointer to a 2D Table containing the filter
 * \param filtersize CPU_INT16S - size of the matrix representing the filter
 * \param color CPU_INT32U - color that will be impacted by the filter. If not selected, will be set to zero
 * \param img_out t_img* - output image
 * \return CPU_CHAR - status of operation
 *
 ***********************************************/
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


    for(i_img=0;i_img< (img_in->he ) ;i_img++)
    {

        for(j_img=0 ; (j_img< img_in->wi );j_img++)
        {

            one_pixel[c_Red] = 0;
            one_pixel[c_Green] = 0;
            one_pixel[c_Blue] = 0;
            for (i=0;i<filtersize;i++)
            {
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


/********************************************//**
 * \brief Gaussian convolution, used to create gaussian filter
 *
 * \param x CPU_INT16S - coordinate x in the matrix
 * \param y CPU_INT16S - coordinate y in the matrix
 * \param sig CPU_FP64 - standard deviation of Gaussian distribution
 * \return CPU_FP64 - result
 *
 * http://en.wikipedia.org/wiki/Gaussian_filter or http://fr.wikipedia.org/wiki/Filtre_de_Gauss
 *
 ***********************************************/
CPU_FP64 conv_gauss(CPU_INT16S x,CPU_INT16S y,CPU_FP64 sig)
{
    CPU_FP64 value = 0,value1 = 0,value2 = 0;

    value1 =  1.0/(2.0*PI*sig*sig);
    value2 =   (0.0 -(x*x+y*y))/(2*sig*sig);

    value = value1 * exp(value2);
    return value;

}


/********************************************//**
 * \brief Create a Gaussian filter used to smooth and image
 *
 * \param tab_filtre CPU_FP64** - pointer to a 2D table (matrix) that will receive the filter
 * \param filtersize CPU_INT16S - size of the matrix
 * \param sigma CPU_FP64 - standard deviation of Gaussian distribution (higher will smooth more)
 * \return CPU_VOID
 *
 ***********************************************/
CPU_VOID create_gauss_filter(CPU_FP64 ** tab_filtre,CPU_INT16S filtersize,CPU_FP64 sigma)
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


/********************************************//**
 * \brief Create an average filter for smoothing
 *
 * \param tab_filtre CPU_FP64** - pointer to a 2D table (matrix) that will receive the filter
 * \param filtersize CPU_INT16S - size of the matrix
 * \return CPU_VOID
 *
 ***********************************************/
CPU_VOID create_average_filter(CPU_FP64 ** tab_filtre,CPU_INT16S filtersize)
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


/********************************************//**
 * \brief Create a laplacian filter for edge detection (return matrix must have a size of 3 or 5)
 *
 * \param tab_filtre CPU_FP64** - pointer to a 2D table (matrix) that will receive the filter of size (minimum size 3)
 * \param filtertype CPU_INT16S - type of filter to use
 * \return CPU_INT16S - filter size
 *
 * type 1   { 0,-1, 0}     type 2  {0, 1, 0}
 *          {-1, 4,-1}             {1,-4, 1}
 *          { 0,-1, 0}             {0, 1, 0}
 *
 * type 3   {-1,-1,-1}     type 4  { 1,-2, 1}
 *          {-1, 8,-1}             {-2, 4,-2}
 *          {-1,-1,-1}             { 1,-2, 1}
 *
 * type 5   { 0, 0,-1, 0, 0}
 *          { 0,-1,-2,-1, 0}
 *          {-1,-2,16,-2,-1}
 *          { 0,-1,-2,-1, 0}
 *          { 0, 0,-1, 0, 0}
 *
 ***********************************************/
CPU_INT16S create_laplacian_filter(CPU_FP64 ** tab_filtre,CPU_INT08U filtertype)
{
    CPU_INT16S filtersize;
    CPU_INT16S i,j;
    switch(filtertype)
    {
        case 1:
            filtersize = 3;
            for(i=0;i<filtersize;i++)
            {
                for(j=0;j<filtersize;j++)
                {
                    tab_filtre[i][j] = lapl_filter_1[i][j];

                }

            }
            break;
        case 2:
            filtersize = 3;
            for(i=0;i<filtersize;i++)
            {
                for(j=0;j<filtersize;j++)
                {
                    tab_filtre[i][j] = lapl_filter_2[i][j];

                }

            }
            break;
        case 3:
            filtersize = 3;
            for(i=0;i<filtersize;i++)
            {
                for(j=0;j<filtersize;j++)
                {
                    tab_filtre[i][j] = lapl_filter_3[i][j];

                }

            }
            break;
        case 4:
            filtersize = 3;
            for(i=0;i<filtersize;i++)
            {
                for(j=0;j<filtersize;j++)
                {
                    tab_filtre[i][j] = lapl_filter_4[i][j];

                }

            }
            break;
        case 5:
            filtersize = 5;
            for(i=0;i<filtersize;i++)
            {
                for(j=0;j<filtersize;j++)
                {
                    tab_filtre[i][j] = lapl_filter_5[i][j];

                }

            }
            break;
        default:
            tab_filtre = 0;
            break;
    }

    return filtersize;
}


/********************************************//**
 * \brief Apply a median filter on an image
 *
 * \param img_in t_img* - input image
 * \param filtersize CPU_INT16S - filter size
 * \param img_out t_img* - output image
 * \return CPU_CHAR - status of operation
 *
 * Creating a matrix is not necessary prior to use this filter
 ***********************************************/
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

/********************************************//**
 * \brief Calcultate the median value of an area surrounding a pixel
 *
 * \param table2D CPU_INT08U** - pixel table
 * \param filter_range CPU_INT16S - size of the filter
 * \param i CPU_INT16S - coordinate of pixel
 * \param j CPU_INT16S - coordinate of pixel
 * \return CPU_INT08U - median value
 *
 ***********************************************/
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
        }
    }
    qsort (table, (filter_range*2+1)*(filter_range*2+1), sizeof(CPU_INT08U), (int (*)(const void*, const void*))compare);
    median = table[(filter_range*2+1)];
    return median;
}



/** \brief Display values contained in a square filter
 *
 * \param tab_filtre CPU_FP64** - pointer to matrix filter to display
 * \param filtersize CPU_INT08U - Size of the filter
 * \return CPU_VOID
 *
 */
CPU_VOID display_filter_value(CPU_FP64 ** tab_filtre,CPU_INT08U filtersize)
{

    CPU_INT16S i,j;

    for(i=0;i<filtersize;i++)
    {
        for(j=0;j<filtersize;j++)
        {
            printf("%.2f ",tab_filtre[i][j]);
        }
        printf("\n");
    }
}
