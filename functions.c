#include "includes.h"

unsigned short min(unsigned short a,unsigned short b)
{
    unsigned short ret=0;
    if(a >= b)
    {
        ret = b;
    }else
    {
        ret = a;
    }
    return ret;
}

unsigned short max(unsigned short a,unsigned short b)
{
    unsigned short ret=0;
    if(a >= b)
    {
        ret = a;
    }else
    {
        ret = b;
    }
    return ret;
}


unsigned char **createTable(int nbLin, int nbCol)
{
	int i;
	unsigned char **tableau = (unsigned char **)malloc(sizeof(unsigned char*)*nbLin);
	unsigned char *tableau2 = (unsigned char *)malloc(sizeof(unsigned char)*nbCol*nbLin);
	for(i = 0 ; i < nbLin ; i++){
		tableau[i] = &tableau2[i*nbCol];
	}
	return tableau;
}

void freeTable(int **tableau)
{
	free(tableau[0]);
	free(tableau);
}


unsigned long Max_CPU_INT32U(unsigned long a,unsigned long b)
{
    unsigned long int c;
	if(a > b)
	{
		c = a;
	}
	else
	{
		c = b;
	}

	return c;
}
