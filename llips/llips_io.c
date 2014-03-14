/****************************************************************/
/* Light Library for Image ProcesS                              */
/* File : llips_io.c                                            */
/* Description :                                                */
/*   About importing and exporting image from/to storage        */
/*                                                              */
/* Author : MPE                                                 */
/*                                                              */
/****************************************************************/

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
/* init_img()                                                   */
/* Description :                                                */
/*   Initialize and allocate memory for a potentional           */
/*   image load                                                 */
/* Input:                                                       */
/*   img - See output                                           */
/*   colors - color layer to display                            */
/* Output:                                                      */
/*   img - initialized image buffer                             */
/* Return:                                                      */
/*   na                                                         */
/*                                                              */
/****************************************************************/
CPU_VOID init_img(t_img * img)
{
    img->signature = 0;
    img->depth = 0;
    img->wi = 0;
    img->he = 0;
    img->Blue = createTable(MAX_HEIGHT,MAX_WIDTH);
    img->Green = createTable(MAX_HEIGHT,MAX_WIDTH);
    img->Red = createTable(MAX_HEIGHT,MAX_WIDTH);
}

/****************************************************************/
/* load_img()                                                   */
/* Description :                                                */
/*   Initialize and allocate memory for a potentional           */
/*   image load                                                 */
/* Input:                                                       */
/*   imgname - path to image to load in memory                  */
/* Output:                                                      */
/*   img - initialized image buffer                             */
/* Return:                                                      */
/*   status of operation                                        */
/*                                                              */
/****************************************************************/
CPU_CHAR load_img(CPU_CHAR * imgname, t_img * img)
{
    CPU_INT16S i,j,offset;
    CPU_CHAR ret = NO_ERROR;
    FILE* fichier = NULL;
    CPU_CHAR dummy=0;


    CPU_INT32U position_fin = 0;

    // Open file
    fichier = fopen(imgname, "rb");
    if (fichier != NULL)
    {
        //Get signature
        fseek (fichier,OFFSET_SIGN,SEEK_SET);
        fread (&(img->signature),OFFSET_SIGN_size,1,fichier);
        if(img->signature == WIN_BMP)
        {
            //Get depth
            fseek (fichier,OFFSET_IMG_DEPTH,SEEK_SET);
            fread (&(img->depth),OFFSET_IMG_DEPTH_size,1,fichier);
            if(img->depth ==DEPTH_24bit)
            {
                //Get width
                fseek (fichier,OFFSET_IMG_WIDTH,SEEK_SET);
                fread (&(img->wi),OFFSET_IMG_WIDTH_size,1,fichier);
                if(img->wi <= MAX_WIDTH)
                {
                    //Get height
                    fseek (fichier,OFFSET_IMG_HEIGHT,SEEK_SET);
                    fread (&(img->he),OFFSET_IMG_HEIGHT_size,1,fichier);
                    if(img->he <= MAX_HEIGHT)
                    {
                        // Here, it is knwon that the BMP is a 24bit Windows BMP
                        // with width and height below 1kpix


                        //Get image offset to deduct header size
                        fseek (fichier,OFFSET_IMG_P_DATA,SEEK_SET);
                        fread (&(img->FileHeader_size),OFFSET_IMG_P_DATA_size,1,fichier);


                        if(img->FileHeader_size < MAX_HEADER_size)
                        {
                            //copy header ...
                            fseek (fichier,0,SEEK_SET);
                            for(i=0;i<img->FileHeader_size;i++)
                            {
                                fread (&(img->FileHeader[i]),PIXEL_size,1,fichier);
                            }
                            //copy IMG data
                            fseek(fichier,0,SEEK_END);
                            position_fin = ftell(fichier);
                            fseek (fichier,OFFSET_IMG_DATA_WIN24bit,SEEK_SET);
                            offset = 0;
                            for(i=0;i< img->he ;i++)
                            {
                                for(j=0 ; j< img->wi ;j++)
                                {
                                    //printf("offset : %d\n",offset);
                                    fread (&(img->Blue[i][j]),PIXEL_size,1,fichier);
                                    fread (&(img->Green[i][j]),PIXEL_size,1,fichier);
                                    fread (&(img->Red[i][j]),PIXEL_size,1,fichier);
                                    offset ++;

                                }
                                //printf("offset fin ligne : %d\n",offset);
                                while(offset%4 != 0)
                                {
                                    //printf("dummy read at offset : %d\n",offset);
                                    fread (&dummy,PIXEL_size,1,fichier);
                                    offset ++;
                                }
                            }
                        }else
                        {
                            ret = ERR_HEADER_TOO_BIG;
                        }
                    }else
                    {
                        ret = ERR_TOO_HEIGHT;
                    }
                }else
                {
                    ret = ERR_TOO_WIDTH;
                }
            }else
            {
                ret = ERR_NOT_24bit;
            }
        }else
        {
            ret = ERR_NOT_WINBMP;
        }
        fclose(fichier);
    }else
    {
        ret = ERR_NOFILE;
    }

    return ret;
};

/****************************************************************/
/* write_img()                                                  */
/* Description :                                                */
/*   Write an image buffer into a BMP file                      */
/*   image load                                                 */
/* Input:                                                       */
/*   imgname - path to image to write on storage                */
/*   img - image buffer                                         */
/* Output:                                                      */
/*   na                                                         */
/* Return:                                                      */
/*   status of operation                                        */
/*                                                              */
/****************************************************************/
CPU_CHAR write_img(CPU_CHAR * imgname, t_img * img)
{
    CPU_CHAR ret = NO_ERROR;
    CPU_INT16S i,j;
    CPU_INT32U offset;
    FILE* fichier = NULL;
    fichier = fopen(imgname, "wb");
    if(fichier != NULL)
    {
        //Write Header
        for(i=0;i<img->FileHeader_size;i++)
        {
            fputc(img->FileHeader[i], fichier);
        }

        //Write color
        offset = 0;
        for(i=0;i< img->he ;i++)
        {
            for(j=0 ; j< img->wi ;j++)
            {
                fputc(img->Blue[i][j], fichier);
                fputc(img->Green[i][j], fichier);
                fputc(img->Red[i][j], fichier);
                offset ++;

            }
            //printf("offset fin ligne : %d\n",offset);
            while(offset%4 != 0)
            {
                //printf("dummy read at offset : %d\n",offset);
                fputc(0, fichier);
                offset ++;
            }
        }
        fclose(fichier);
    }
    else
    {
        ret = ERR_OPENFILE;
    }
    return ret;
}
