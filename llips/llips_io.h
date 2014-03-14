void init_img(t_img * img);
unsigned char decode_img(char * imgname, t_img * img);
unsigned char write_img(char * imgname, t_img * img);
unsigned char **createTable(int nbLin, int nbCol);
void freeTable(int **tableau);
