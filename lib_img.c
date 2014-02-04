#include "includes.h"

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

extern unsigned char g_debug_mode;

void init_img(t_img * img)
{
    int i,j;
    img->signature = 0;
    img->depth = 0;
    img->wi = 0;
    img->he = 0;
    for(i=0;i<MAX_WIDTH;i++)
    {
        for(j=0;j<MAX_HEIGHT;j++)
        {
           img->Blue[i][j] = 0;
           img->Green[i][j] = 0;
           img->Red[i][j] = 0;
        }
    }
}

unsigned char decode_img(char * imgname, t_img * img)
{
    int i,j,offset;
    unsigned char ret = NO_ERROR;
    FILE* fichier = NULL;
    unsigned char dummy=0;


    long int position_fin = 0;

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

void display_img_value(t_img * img,short int colors)
{
    int i,j;

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
        //for(i=0;i<img1.he;i++)
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
        //for(i=0;i<img->he;i++)
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
        //for(i=0;i<img->he;i++)
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

unsigned char write_img(char * imgname, t_img * img)
{
    unsigned char ret = NO_ERROR;
    int i,j,offset;
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
