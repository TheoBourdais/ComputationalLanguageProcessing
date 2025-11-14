#ifndef GENER6_H
#define GENER6_H

#include "common.h"

#define weights(X,Y) *((pmatrices+X)->weights+Y)
#define distances(X,Y) *((pmatrices+X)->distances+Y)
#define grid(X,Y) *((pmatrices+X)->grid+Y)

typedef struct
     {int x,y;       /* coordinates of an object on the grid */
      int selected;  /* vertices of the graph */
      int candidates;/* all possible vertices for graph selection */
      int set1,set2; /* vertices of the left and, respectively, 
                        right part of the graph */
      float distribution; /* probabilities for graph size selection */
      int gridx,
          gridy;     /* grid lines containing points corresponding
                        to graph vertices selected */
      int optimal;   /* an optimal permutation */
      int n;         /* number of generated graphs of each size */
      int fail;      /* number of failures for each graph size */
     }Objects;
typedef struct
     {int x,y;       /* pointers to arcs from the vertices of the 
                        left and, respectively, right part */
      int ex,ey;     /* graph arcs listed with respect to the left 
                        and, correspondingly, right part */
      int x_orient,
          y_orient;  /* orientation of the arcs */
      int deviation; /* a special characteristic used for balancing
                        of the graph */
      int object;    /* a correspondence between vertices of the 
                        left part of the graph and objects */
      int x_mark,
          y_mark;    /* a marker for reached vertices during
                        breadth-first search of the path */
     }Graph;
typedef struct
     {int reached;   /* a set of reached vertices during the
                        breadth-first search */
      int mark;      /* a marker showing the part the reached
                        vertex belongs to */
      int track;     /* for each vertex reached, its predecessor
                        on a path from the start vertex */
      int p;         /* vertices of the path */
     }Path;
typedef struct
     {int *weights;  /* a pointer to a row of weight matrix   */
      int *distances;/* a pointer to a row of distance matrix */
      int *grid;     /* a pointer to a row of grid            */
     }Matrices;
typedef struct
     {int x,y;       /* coordinates of a grid point */
     }Grid_structure;

#endif