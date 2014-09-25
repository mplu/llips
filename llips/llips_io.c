/********************************************//**
 * \file
 * \brief About importing and exporting image from/to storage
 * \author MPE
 *
 ***********************************************/

/* ***************************************************************/
/* *           Includes                                          */
/* ***************************************************************/
#include "llips_includes.h"

/* ***************************************************************/
/* *           Global variables                                  */
/* ***************************************************************/

/* ***************************************************************/
/* *           Functions                                         */
/* ***************************************************************/


/********************************************//**
 * \brief Initialize and allocate memory for a potentional load_img()
 *
 * \param img t_img* - initialized image buffer
 * \return CPU_VOID
 *
 ***********************************************/
CPU_VOID init_img(t_img * img)
{
    img->signature = 0;
    img->depth = 0;
    img->wi = 0;
    img->he = 0;
    img->Blue = createTableINT08U(MAX_HEIGHT,MAX_WIDTH);
    img->Green = createTableINT08U(MAX_HEIGHT,MAX_WIDTH);
    img->Red = createTableINT08U(MAX_HEIGHT,MAX_WIDTH);
}


/********************************************//**
 * \brief Load a bmp file into an organized memory area
 *
 * \param imgname CPU_CHAR* - Image name and path
 * \param img t_img* - initialized image buffer
 * \return CPU_CHAR - status of operation
 *
 ***********************************************/
CPU_CHAR load_img(CPU_CHAR * imgname, t_img * img)
{
    CPU_INT16S i,j,offset;
    CPU_CHAR ret = ERR_NONE;
    FILE* fichier = NULL;
    CPU_CHAR dummy=0;

    // Open file
    fichier = fopen((const char *)imgname, "rb");
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
                            fseek (fichier,OFFSET_IMG_DATA_WIN24bit,SEEK_SET);
                            offset = 0;
                            for(i=0;i< img->he ;i++)
                            {
                                for(j=0 ; j< img->wi ;j++)
                                {
                                    fread (&(img->Blue[i][j]),PIXEL_size,1,fichier);
                                    fread (&(img->Green[i][j]),PIXEL_size,1,fichier);
                                    fread (&(img->Red[i][j]),PIXEL_size,1,fichier);
                                    offset ++;

                                }
                                while(offset%4 != 0)
                                {
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


/********************************************//**
 * \brief Write an image buffer into a BMP file
 *
 * \param imgname CPU_CHAR* - Image name and path
 * \param img t_img* - image buffer
 * \return CPU_CHAR - status of operation
 *
 ***********************************************/
CPU_CHAR write_img(CPU_CHAR * imgname, t_img * img)
{
    CPU_CHAR ret = ERR_NONE;
    CPU_INT16S i,j;
    CPU_INT32U offset;
    FILE* fichier = NULL;
    fichier = fopen((const char *)imgname, "wb");
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
            while(offset%4 != 0)
            {
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


/********************************************//**
 * \brief Copy source image into destination image
 *
 * \param imgsrc t_img* - source image
 * \param imgdest t_img* - destination image
 * \return CPU_CHAR - status of operation
 *
 ***********************************************/
CPU_CHAR copy_img( t_img * imgsrc, t_img * imgdest)
{
    CPU_CHAR ret = ERR_NONE;
    CPU_INT16S i,i_img,j_img;

//Write Header
    for(i=0;i<imgsrc->FileHeader_size;i++)
    {
        imgdest->FileHeader[i] = imgsrc->FileHeader[i];
    }
    imgdest->signature = imgsrc->signature;
    imgdest->depth = imgsrc->depth;
    imgdest->wi = imgsrc->wi;
    imgdest->he = imgsrc->he;
    imgdest->FileHeader_size = imgsrc->FileHeader_size;


    for(i_img=0;i_img< (imgsrc->he ) ;i_img++)
    {

        for(j_img=0 ; (j_img< imgsrc->wi );j_img++)
        {
            imgdest->Red[i_img][j_img] = imgsrc->Red[i_img][j_img];
            imgdest->Green[i_img][j_img] = imgsrc->Green[i_img][j_img];
            imgdest->Blue[i_img][j_img] = imgsrc->Blue[i_img][j_img];
        }
    }
    return ret;
}
