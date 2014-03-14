#define DEF_ENABLED         1
#define DEF_DISABLED        0



typedef struct s_img
{
    unsigned short  signature ;
    unsigned short  depth ;
    unsigned long   wi ;
    unsigned long   he ;
    unsigned char ** Blue;
    unsigned char ** Green;
    unsigned char ** Red;

    unsigned char   FileHeader[MAX_HEADER_size];
    unsigned char   FileHeader_size;

}t_img;

typedef struct s_pixel
{
    unsigned short  x ;
    unsigned short  y ;
}t_pixel;

typedef struct s_vect
{
    short  x ;
    short  y ;
}t_vect;

typedef struct s_area
{
    t_pixel BotLeft;
    t_pixel BotRight;
    t_pixel TopLeft;
    t_pixel TopRight;
}t_area;
