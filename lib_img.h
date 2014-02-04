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
