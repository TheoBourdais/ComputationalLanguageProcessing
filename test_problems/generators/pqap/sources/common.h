#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>
#include <stdio.h>

#define BT 50
#define INTLIMIT 30000

// Use consistent memory allocation macros
#define ALS(p,t,n) if ((p=(t*)calloc((n),sizeof(t)))==NULL) \
    {printf("  allocation failure\n");exit(1);}
#define ALI(p,n) if ((p=(int*)calloc((n),sizeof(int)))==NULL) \
    {printf("  allocation failure\n");exit(1);}

#endif