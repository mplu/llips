#define BLUE    0x0001
#define GREEN   0x0010
#define RED     0x0100
#define HEADER  0x1000

#define GetBlue(c)  0xFF&(c)
#define GetGreen(c) 0xFF&(c>>8)
#define GetRed(c)   0xFF&(c>>16)

#define SetRGB(r,g,b)   ((0xFF&r)<<16) | ((0xFF&g)<<8 ) | (0xFF&b)

typedef struct s_pixel
{
    unsigned short  x ;
    unsigned short  y ;
}t_pixel;

typedef struct s_area
{
    t_pixel BotLeft;
    t_pixel BotRight;
    t_pixel TopLeft;
    t_pixel TopRight;
}t_area;

void init_area(t_area * area,unsigned short maxwidth,unsigned short maxheight);
unsigned char search_diff(unsigned char tolerance,unsigned short quantity, t_img * img_in1,t_img * img_in2,t_img * img_out);
void highlight_area(t_img * img,t_area * area,unsigned long RGB);
void printf_area(t_area * area);
