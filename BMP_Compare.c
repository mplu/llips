#include <stdio.h>
#include <stdlib.h>
#include<string.h>

#define BASE       "imgbase.bmp"
#define NEW        "imgnew.bmp"
#define NEW_DIFF   "imgnew_diff.bmp"

#define DEFAULT_TOLERANCE       11
#define DEFAULT_QUANTITY         1

#define MAX_WIDTH       400
#define MAX_HEIGHT      300
#define MAX_HEADER_size 200

#define OFFSET_SIGN                 0x0000
#define OFFSET_IMG_P_DATA           0x000A
#define OFFSET_IMG_WIDTH            0x0012
#define OFFSET_IMG_HEIGHT           0x0016
#define OFFSET_IMG_DEPTH            0x001C

#define OFFSET_SIGN_size                 2
#define OFFSET_IMG_P_DATA_size           4
#define OFFSET_IMG_WIDTH_size            4
#define OFFSET_IMG_HEIGHT_size           4
#define OFFSET_IMG_DEPTH_size            2
#define PIXEL_size                       1

#define WIN_BMP                     0x4D42
#define OFFSET_IMG_DATA_WIN24bit    0x0036
#define DEPTH_24bit                     24
#define PIXEL_8bit_RANGE               255


#define NO_ERROR            0x0000
#define ERR_NOFILE          0x0001
#define ERR_NOT_WINBMP      0x0002
#define ERR_NOT_24bit       0x0003
#define ERR_TOO_WIDTH       0x0004
#define ERR_TOO_HEIGHT      0x0005
#define ERR_HEADER_TOO_BIG  0x0006
#define ERR_OPENFILE        0x0007

#define NO_DIFF             0x0000
#define DIFF_SIZE           0x0001
#define DIFF_BLUE           0x0002
#define DIFF_GREEN          0x0004
#define DIFF_RED            0x0008
#define DIFF_HIGH_QUANTITY  0x0010

#define DEF_ENABLED     1
#define DEF_DISABLED    0

#define DEBUG_opt   "debug"

#define BLUE    0x0001
#define GREEN   0x0010
#define RED     0x0100
#define HEADER  0x1000


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
typedef struct s_img
{
    unsigned short  signature ;
    unsigned short  depth ;
    unsigned long   wi ;
    unsigned long   he ;
    unsigned char   Blue[MAX_WIDTH][MAX_HEIGHT];
    unsigned char   Green[MAX_WIDTH][MAX_HEIGHT];
    unsigned char   Red[MAX_WIDTH][MAX_HEIGHT];
    unsigned char   FileHeader[MAX_HEADER_size];
    unsigned char   FileHeader_size;

}t_img;

unsigned char decode_img(char * imgname, t_img * img);
void init_img(t_img * img);
void display_img_value(t_img * img,short int colors);
unsigned char write_img(char * imgname, t_img * img);
unsigned char search_diff(unsigned char tolerance,unsigned short quantity, t_img * img_in1,t_img * img_in2,t_img * img_out);

unsigned char g_debug_mode = DEF_DISABLED;

int main(int argc , char *argv[])
{

    t_img img1;
    t_img img2;
    t_img img3;

    unsigned char img1_err,img2_err,img3_err;
    unsigned char img_diff;


    int tolerance, quantity;

    // checking debug argument
	if (argc == 4)
    {

        if(strcmp(DEBUG_opt, argv[3]) == 0)
        {
            g_debug_mode = DEF_ENABLED;
            printf("%s activated\n",argv[3]);
        }
    }else
    {/*nothing*/}

    if (argc < 3)
	{
	    if (g_debug_mode == DEF_ENABLED)
		{
			printf("USAGE: %s <tolerance in percent> <quantity of pixel in perthousand> [opt:DEBUG]\nIncorrect Using default value\n",argv[0]);
		}else{/*nothing*/}
		
		tolerance = DEFAULT_TOLERANCE;
		quantity = DEFAULT_QUANTITY;

	}else
	{
        sscanf (argv[1],"%d",&tolerance);
        sscanf (argv[2],"%d",&quantity);
        if ((tolerance < 0) || (tolerance > 100))
        {
            printf ("tolerance %d out of bound, using default value : %d\n",tolerance,DEFAULT_TOLERANCE);
            tolerance = DEFAULT_TOLERANCE;
        }
        if ((quantity < 0) || (quantity > 1000))
        {
            printf ("quantity %d out of bound, using default value : %d\n",quantity,DEFAULT_QUANTITY);
            quantity = DEFAULT_QUANTITY ;
        }
    }











	if (g_debug_mode == DEF_ENABLED)
    {
        printf ("tolerance %d\nquantity %d\n",tolerance,quantity);
        printf("Init data\n");
    }else{/*nothing*/}

    init_img(&img1);
    init_img(&img2);
    init_img(&img3);

    img1_err = decode_img(BASE, &img1);
    if (img1_err == NO_ERROR)
    {
        if (g_debug_mode == DEF_ENABLED)
        {
            // Display info
            printf("Image %s is valid :\n%x signature\n%d depth\n%d width\n%d height\n\n"  \
                    ,BASE,img1.signature,img1.depth,(short int)img1.wi,(short int)img1.he);
            // Display Image
            display_img_value(&img1,/*HEADER|BLUE|GREEN|RED|*/0);
        }else{/*nothing*/}

    }else
    {
        if (g_debug_mode == DEF_ENABLED)
        {
            printf("Image %s not supported - err : %x\n",BASE,img1_err);
        }else{/*nothing*/}

    }

    img2_err = decode_img(NEW, &img2);
    if (img2_err == NO_ERROR)
    {
        if (g_debug_mode == DEF_ENABLED)
        {
            // Display info
            printf("Image %s is valid :\n%x signature\n%d depth\n%d width\n%d height\n\n"  \
                    ,NEW,img2.signature,img2.depth,(short int)img2.wi,(short int)img2.he);
            // Display Image
            display_img_value(&img1,/*HEADER|BLUE|GREEN|RED|*/0);
        }else{/*nothing*/}

    }else
    {
        if (g_debug_mode == DEF_ENABLED)
        {
            printf("Image %s not supported - err : %x\n",NEW,img2_err);
        }else{/*nothing*/}

    }

    if ((img2_err == NO_ERROR) && (img2_err == NO_ERROR))
    {

        img_diff = search_diff((unsigned char)tolerance,(unsigned char)quantity,&img1,&img2,&img3);
        if (g_debug_mode == DEF_ENABLED)
        {
            printf("Images %s and %s comparison report :\n",BASE,NEW);
            printf("img_diff = %2x\n",img_diff);
        }else{/*nothing*/}

        if (img_diff == NO_DIFF)
        {
            if (g_debug_mode == DEF_ENABLED)
            {
                printf("Seems Identical\n");
            }else{/*nothing*/}

        }else
        {
            if (g_debug_mode == DEF_ENABLED)
            {
                printf("Different on ");
            }else{/*nothing*/}

            if((img_diff & DIFF_SIZE)!=0)
            {
                if (g_debug_mode == DEF_ENABLED)
                {
                    printf("size ");
                }else{/*nothing*/}
            }
            if((img_diff & DIFF_BLUE)!=0)
            {
                if (g_debug_mode == DEF_ENABLED)
                {
                    printf("blue pixels ");
                }else{/*nothing*/}
            }
            if((img_diff & DIFF_GREEN)!=0)
            {
                if (g_debug_mode == DEF_ENABLED)
                {
                    printf("green pixels ");
                }else{/*nothing*/}
            }
            if((img_diff & DIFF_RED)!=0)
            {
                if (g_debug_mode == DEF_ENABLED)
                {
                    printf("red pixels ");
                }else{/*nothing*/}
            }
            if (g_debug_mode == DEF_ENABLED)
            {
                printf("\n");
            }else{/*nothing*/}

        }

        //display_img_value(&img3,HEADER|BLUE|GREEN|RED|0);
        if((img_diff & DIFF_HIGH_QUANTITY)!=0)
        {

            if (g_debug_mode == DEF_ENABLED)
            {
                printf("Too many differences observed\n");
            }else{/*nothing*/}
            // save a copy
            img3_err = write_img(NEW_DIFF,&img3);
            if(img3_err == NO_ERROR)
            {
                if (g_debug_mode == DEF_ENABLED)
                {
                    printf("Write Successful\n");
                }else{/*nothing*/}
            }else
            {
                if (g_debug_mode == DEF_ENABLED)
                {
                    printf("Write output image error\n");
                }else{/*nothing*/}
            }
            printf("1\n");
        }else
        {

            if (g_debug_mode == DEF_ENABLED)
            {
                printf("Not enough differences using actuel parameters\n");
                // save a copy
                img3_err = write_img(NEW_DIFF,&img3);
                if(img3_err == NO_ERROR)
                {
                    if (g_debug_mode == DEF_ENABLED)
                    {
                        printf("Write Successful\n");
                    }else{/*nothing*/}
                }else
                {
                    if (g_debug_mode == DEF_ENABLED)
                    {
                        printf("Write output image error\n");
                    }else{/*nothing*/}
                }
            }else{/*nothing*/}
            printf("0\n");
        }


    }else
    {
        printf("0\n");
    }

    return 0;
}

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

unsigned char search_diff(unsigned char tolerance,unsigned short quantity, t_img * img_in1,t_img * img_in2,t_img * img_out)
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

    return ret;
}
