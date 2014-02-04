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
