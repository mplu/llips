#include "includes.h"

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
    g_debug_mode = DEF_ENABLED;

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

