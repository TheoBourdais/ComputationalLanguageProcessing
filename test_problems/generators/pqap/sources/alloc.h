#ifndef ALLOC_H
#define ALLOC_H

#include <stdlib.h>

// Replace DOS memory allocation macros
#define ALS(p,t,n) if ((p=(t*)malloc((n)*sizeof(t)))==NULL) \
    {printf("  allocation failure\n");exit(1);}
#define ALI(p,n) if ((p=(int*)malloc((n)*sizeof(int)))==NULL) \
    {printf("  allocation failure\n");exit(1);}

#endif