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

#define BLUE    0x0001
#define GREEN   0x0010
#define RED     0x0100
#define HEADER  0x1000

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

#define GetBlue(c)  0xFF&(c)
#define GetGreen(c) 0xFF&(c>>8)
#define GetRed(c)   0xFF&(c>>16)

#define SetRGB(r,g,b)   ((0xFF&r)<<16) | ((0xFF&g)<<8 ) | (0xFF&b)


void display_img_value(t_img * img,short int colors);
void init_area(t_area * area,unsigned short maxwidth,unsigned short maxheight);
void highlight_area(t_img * img,t_area * area,unsigned long RGB);
void printf_area(t_area * area);
t_vect highlight_line(t_img * img,t_pixel pix1,t_pixel pix2,unsigned long RGB);
t_area pixel_to_area(t_pixel pix);
t_vect pixels_to_vector(t_pixel pix1,t_pixel pix2);
unsigned char color_filter(t_img * img_in,t_img * img_out, unsigned long color);
unsigned char histogram(t_img * img_in,t_img * img_out, unsigned long color);
unsigned char luminance(t_img * img_in,t_img * img_out);
