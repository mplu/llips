/****************************************************************/
/* Light Library for Image ProcesS                              */
/* File : llips_general.h                                       */
/* Description :                                                */
/*   About general image handling                               */
/*                                                              */
/* Author : MPE                                                 */
/*                                                              */
/****************************************************************/

/****************************************************************/
/**           Define                                            */
/****************************************************************/
/* Offset of different data field in BMP images */
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

/* Color masks */
#define BLUE    0x0001
#define GREEN   0x0010
#define RED     0x0100
#define HEADER  0x1000

/* Error management */
#define NO_ERROR            0x0000
#define ERR_NOFILE          0x0001
#define ERR_NOT_WINBMP      0x0002
#define ERR_NOT_24bit       0x0003
#define ERR_TOO_WIDTH       0x0004
#define ERR_TOO_HEIGHT      0x0005
#define ERR_HEADER_TOO_BIG  0x0006
#define ERR_OPENFILE        0x0007

/* Color difference management */
#define NO_DIFF             0x0000
#define DIFF_SIZE           0x0001
#define DIFF_BLUE           0x0002
#define DIFF_GREEN          0x0004
#define DIFF_RED            0x0008
#define DIFF_HIGH_QUANTITY  0x0010

/****************************************************************/
/**           Macro                                             */
/****************************************************************/
/* Get and Set for getting specific primary color */
#define GetBlue(c)  0xFF&(c)
#define GetGreen(c) 0xFF&(c>>8)
#define GetRed(c)   0xFF&(c>>16)
#define SetRGB(r,g,b)   ((0xFF&r)<<16) | ((0xFF&g)<<8 ) | (0xFF&b)

/****************************************************************/
/**           Global variables                                  */
/****************************************************************/

/****************************************************************/
/**           Prototypes                                        */
/****************************************************************/
CPU_VOID display_img_value(t_img * img,CPU_INT16S colors);
CPU_VOID init_area(t_area * area,CPU_INT16U maxwidth,CPU_INT16U maxheight);
CPU_VOID highlight_area(t_img * img,t_area * area,CPU_INT32U RGB);
CPU_VOID printf_area(t_area * area);
t_vect highlight_line(t_img * img,t_pixel pix1,t_pixel pix2,CPU_INT32U RGB);
t_area pixel_to_area(t_pixel pix);
t_vect pixels_to_vector(t_pixel pix1,t_pixel pix2);
CPU_CHAR color_filter(t_img * img_in,t_img * img_out, CPU_INT32U color);
CPU_CHAR histogram(t_img * img_in,t_img * img_out);
CPU_CHAR luminance(t_img * img_in,t_img * img_out);
