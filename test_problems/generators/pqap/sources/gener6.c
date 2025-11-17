// #include <alloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
// #include <process.h>
#include "GENER6.H"

int graph_selection(int,int,int,int,int,int,int,int *,Matrices *,Objects *,
      Graph *,Path *);

 int graph_size_selection(int size_bound,int size_lower_bound)
 {int r_numb;
  r_numb=((double)rand()/(double)(RAND_MAX))*(size_bound-size_lower_bound+2);
  if (r_numb%2>0) r_numb--;
  return r_numb+size_lower_bound;
 }
 void generator6(FILE *out,int n_objects,
                 int x_dimension,int y_dimension,long n_graphs,
                 int size_bound,int size_lower_bound,int graph_weight_bound,
                 int bound_on_trials,int *failure,
                 double *minimal_value,double *seed,
                 Matrices *pmatrices,Objects *pobjects,
                 Graph *pgraph,Path *ppath,
                 Grid_structure *pgrid_structure)
 {int i,j,k;
  int object;
  int n_points;
  int weigh;
  int ind;
  int res;
  int min_weight,dist;
  int x1,x2,y1,y2;
  long lo,lon;
  double total_distance;
  long sum_weight;

  *failure=0;
  srand(*seed);
         /* Randomly selecting grid points for objects  */
  n_points=x_dimension*y_dimension;
  ALS(pgrid_structure,Grid_structure,n_points+1)
  for (i=1;i<=x_dimension;i++)
      ALI((pmatrices+i)->grid,y_dimension+1)
  for (i=1,ind=0;i<=x_dimension;i++) for (j=1;j<=y_dimension;j++)
     {ind++;
      (pgrid_structure+ind)->x=i;(pgrid_structure+ind)->y=j;
      grid(i,j)=0;
     }
  for (i=1;i<=n_objects;i++)
     {k=((double)rand()/(double)(RAND_MAX))*(n_points-i+1);
      k+=i;
      (pobjects+i)->x=(pgrid_structure+k)->x;
      (pobjects+i)->y=(pgrid_structure+k)->y;
      grid((pobjects+i)->x,(pobjects+i)->y)=i;
      (pgrid_structure+k)->x=(pgrid_structure+i)->x;
      (pgrid_structure+k)->y=(pgrid_structure+i)->y;
     }
  free(pgrid_structure);
         /* Initialization of matrices  */
  for (i=1;i<=n_objects;i++)
     {distances(i,i)=0;
      (pobjects+i)->n=0;
      (pobjects+i)->fail=0;
      for (j=1;j<=n_objects;j++)
           weights(i,j)=0;
     }
  pobjects->fail=0;
         /* Constructing matrix of weights  */
  sum_weight=0;
  for (lo=1;lo<=n_graphs;lo++)
     {res=graph_selection(n_objects,x_dimension,y_dimension,
            size_bound,size_lower_bound,graph_weight_bound,bound_on_trials,
            &weigh,pmatrices,pobjects,pgraph,ppath);
      if (res < 1) {*failure=1;goto lab;}
      sum_weight+=weigh;
     }
  for (i=1,min_weight=1;i<n_objects;i++) for (j=i+1;j<=n_objects;j++)
      if (weights(i,j)<min_weight) min_weight=weights(i,j);
  if (min_weight<0) 
     {min_weight=-min_weight;
      for (i=1;i<n_objects;i++) for (j=i+1;j<=n_objects;j++)
         {lon=weights(i,j)+(long)min_weight;
          if (lon>INTLIMIT) {printf("  'int' overflow\n");exit(1);}
          weights(i,j)=(int)lon;
          weights(j,i)=weights(i,j);
         }
     }
   else min_weight=0;
         /* Constructing matrix of distances  */
  for (j=1,ind=0;j<=y_dimension;j++) for (i=1;i<=x_dimension;i++)
     {object=grid(i,j);if (object<=0) continue;
      ind++;
      (pobjects+ind)->optimal=object;
      (pobjects+ind)->x=i;(pobjects+ind)->y=j;
     }
  for (i=1,total_distance=0;i<n_objects;i++)
     {x1=(pobjects+i)->x;y1=(pobjects+i)->y;
      for (j=i+1;j<=n_objects;j++)
         {x2=(pobjects+j)->x;y2=(pobjects+j)->y;
          if (x1<=x2) dist=x2-x1; else dist=x1-x2;
          if (y1<=y2) dist+=(y2-y1); else dist+=(y1-y2);
          distances(i,j)=dist;distances(j,i)=dist;
          total_distance+=(double)dist;
         }
     }
  *minimal_value=total_distance*(double)min_weight*2;
  lo=(long)n_objects*(long)(n_objects-1)/2;lo*=(long)min_weight;
  sum_weight+=lo;
//  fprintf(out,"  sum_weight=%8ld\n",sum_weight);
 lab:;
 }
 void set_orientation_for_left_part(int x,int y,int in,Graph *pgraph)
 {int i;
  for (i=(pgraph+x)->x;i<(pgraph+x+1)->x;i++)
      if ((pgraph+i)->ex==y)
         {if (in>0) (pgraph+i)->x_orient=1;
            else (pgraph+i)->x_orient=0;
          return;
         }
 }
 void set_orientation_for_right_part(int y,int x,int in,Graph *pgraph)
 {int i;
  for (i=(pgraph+y)->y;i<(pgraph+y+1)->y;i++)
      if ((pgraph+i)->ey==x)
         {if (in>0) (pgraph+i)->y_orient=1;
            else (pgraph+i)->y_orient=0;
          return;
         }
 }
 int balance(int lnx,int lny,Graph *pgraph,Path *ppath)
 {int i,j,k;
  int index,length,pl,examined;
  int source,target,vertex,reached;
  int odd;

  while (1)
     {
      /* Taking the first vertex for a path */
      for (i=1,source=-1;i<=lnx;i++)
          if ((pgraph+i)->deviation<0) {source=i;break;}
      if (source<0) return 1;
      /* Initialization of the breadth-first search */
      for (i=1;i<=lnx;i++) (pgraph+i)->x_mark=0;
      for (i=1;i<=lny;i++) (pgraph+i)->y_mark=0;
      (ppath+1)->reached=source;
      (ppath+1)->mark=0;(ppath+1)->track=0;
      (pgraph+source)->x_mark=1;
      length=examined=1;
      /* Starting the search */
      while (examined<=length)
         {vertex=(ppath+examined)->reached;
          target=-1;
          switch ((ppath+examined)->mark)
             {case 0:
      /* Current vertex is from the left part */
                     for (j=(pgraph+vertex)->x;j<(pgraph+vertex+1)->x;j++)
                         {if ((pgraph+j)->x_orient>0) continue;
                          reached=(pgraph+j)->ex;
                          if ((pgraph+reached)->y_mark>0) continue;
                          (pgraph+reached)->y_mark=1;
                          length++;
                          (ppath+length)->reached=reached;
                          (ppath+length)->mark=1;
                          (ppath+length)->track=examined;
                         }
                     break;
              case 1:
      /* Current vertex is from the right part */
                     for (j=(pgraph+vertex)->y;j<(pgraph+vertex+1)->y;j++)
                         {if ((pgraph+j)->y_orient>0) continue;
                          reached=(pgraph+j)->ey;
                          if ((pgraph+reached)->x_mark>0) continue;
                          length++;
                          (ppath+length)->reached=reached;
                          (ppath+length)->mark=0;
                          (ppath+length)->track=examined;
                          if ((pgraph+reached)->deviation>0)
                             {target=reached;break;}
                          (pgraph+reached)->x_mark=1;
                         }
                     break;
             } /* switch */
          if (target>0) break;
          examined++;
         } /* inner while */
      if (target<0) return 0;
      /* Constructing the path (initially, reverse) from the search results */
      (ppath+1)->p=target;pl=1;
      index=length;
      while ((ppath+index)->track>0)
         {index=(ppath+index)->track;
          pl++;
          (ppath+pl)->p=(ppath+index)->reached;
         }
      /* Reversing the path */
      for (i=1;i<=pl/2;i++)
         {k=(ppath+i)->p;(ppath+i)->p=(ppath+pl-i+1)->p;
          (ppath+pl-i+1)->p=k;
         }
      /* Changing orientation of the arcs on the path to the opposite */
      for (i=1,odd=1;i<pl;i++)
         {if (odd>0)
             {odd=0;
              set_orientation_for_left_part((ppath+i)->p,(ppath+i+1)->p,
                                            1,pgraph);
              set_orientation_for_right_part((ppath+i+1)->p,(ppath+i)->p,
                                            0,pgraph);
             }
            else
             {odd=1;
              set_orientation_for_right_part((ppath+i)->p,(ppath+i+1)->p,
                                            1,pgraph);
              set_orientation_for_left_part((ppath+i+1)->p,(ppath+i)->p,
                                            0,pgraph);
             }
         }
      (pgraph+source)->deviation+=2;
      (pgraph+target)->deviation-=2;
     } /* outer while */
 }
 int odd_coloring(int n_selected,int x_dimension,int y_dimension,
                      Objects *pobjects,Graph *pgraph,Path *ppath)
 {int i,j,k,q;
  int lnx,lny,indx,indy,ln1,ln2,ind1,ind2;
  int x,y;
  int obj;
  int dif,count;
  int res,delta_positive;
  int r_numb;
      /* Identifying grid lines containing points selected */
  for (i=1;i<=x_dimension;i++) (pobjects+i)->gridx=0;
  for (i=1;i<=y_dimension;i++) (pobjects+i)->gridy=0;
  for (i=1;i<=n_selected;i++)
     {obj=(pobjects+i)->selected;
      ((pobjects+(pobjects+obj)->x)->gridx)++;
      ((pobjects+(pobjects+obj)->y)->gridy)++;
     }
      /* Constructing array of pointers to arcs for the left part */
  (pgraph+1)->x=(pgraph+1)->y=1;
  lnx=lny=0;
  for (i=1;i<=x_dimension;i++)
     {if ((pobjects+i)->gridx<=0) continue;
      lnx++;(pgraph+lnx)->x_mark=-1;
      (pgraph+lnx+1)->x=(pgraph+lnx)->x+(pobjects+i)->gridx;
      (pobjects+i)->gridx=lnx;
     }
      /* Constructing array of pointers to arcs for the right part */
  for (i=1;i<=y_dimension;i++)
     {if ((pobjects+i)->gridy<=0) continue;
      lny++;(pgraph+lny)->y_mark=-1;
      (pgraph+lny+1)->y=(pgraph+lny)->y+(pobjects+i)->gridy;
      (pobjects+i)->gridy=lny;
     }
      /* Constructing arrays of arcs */
  for (i=1;i<=n_selected;i++)
     {obj=(pobjects+i)->selected;
      x=(pobjects+obj)->x;y=(pobjects+obj)->y;
      indx=(pobjects+x)->gridx;((pgraph+indx)->x_mark)++;
      indy=(pobjects+y)->gridy;((pgraph+indy)->y_mark)++;
      k=(pgraph+indx)->x+(pgraph+indx)->x_mark;
      (pgraph+k)->ex=indy;(pgraph+k)->object=obj;
      k=(pgraph+indy)->y+(pgraph+indy)->y_mark;
      (pgraph+k)->ey=indx;
     }
      /* Setting orientation for arcs */
  delta_positive=0;
  for (i=1;i<=lny;i++)
     {dif=(pgraph+i+1)->y-(pgraph+i)->y;
      k=dif/2;
      if (dif%2>0)
         {if (delta_positive>0) {k++;delta_positive=0;}
            else delta_positive=1;
         }
      for (j=0;j<k;j++)
         {r_numb=((double)rand()/(double)(RAND_MAX))*(dif-j);
          ind1=(pgraph+i)->y+j;ind2=ind1+r_numb;
          q=(pgraph+ind1)->ey;
          (pgraph+ind1)->ey=(pgraph+ind2)->ey;
          (pgraph+ind2)->ey=q;
          (pgraph+ind1)->y_orient=1;
          set_orientation_for_left_part((pgraph+ind1)->ey,i,0,pgraph);
         }
      for (j=(pgraph+i)->y+k;j<(pgraph+i+1)->y;j++)
         {(pgraph+j)->y_orient=0;
          set_orientation_for_left_part((pgraph+j)->ey,i,1,pgraph);
         }
     }
      /* Calculating deviation for vertices of the left part */
  delta_positive=1;
  for (i=1;i<=lnx;i++)
     {dif=(pgraph+i+1)->x-(pgraph+i)->x;
      k=dif/2;
      if (dif%2>0)
         {if (delta_positive>0) {k++;delta_positive=0;}
            else delta_positive=1;
         }
      for (j=(pgraph+i)->x,count=0;j<(pgraph+i+1)->x;j++)
          if ((pgraph+j)->x_orient>0) count++;
      (pgraph+i)->deviation=2*(count-k);
     }
      /* Balancing */
  res=balance(lnx,lny,pgraph,ppath);
  if (res<1) return 0;
      /* Dividing the set of selected objects into two subsets */
  ln1=ln2=0;
  for (i=1;i<(pgraph+lnx+1)->x;i++)
      if ((pgraph+i)->x_orient==0)
         {ln1++;
          (pobjects+ln1)->set1=(pgraph+i)->object;
         }
        else
         {ln2++;
          (pobjects+ln2)->set2=(pgraph+i)->object;
         }
  pobjects->set1=ln1;pobjects->set2=ln2;
  return 1;
 }
 int graph_selection(int n_objects,int x_dimension,int y_dimension,
                     int size_bound,int size_lower_bound,int graph_weight_bound,
                     int bound_on_trials,int *weigh,Matrices *pmatrices,
                     Objects *pobjects,Graph *pgraph,Path *ppath)
 {int i,i1,j,j1,k;
  int size;
  int wei;
  int res;
  int n_failures=0;
  long lo;

  while (n_failures<bound_on_trials)
     {if (n_failures<=0)
         {size=graph_size_selection(size_bound,size_lower_bound);
          if (size<0) return 0;
         }
      for (i=1;i<=n_objects;i++) (pobjects+i)->candidates=i;
      for (i=1;i<=size;i++)
         {k=((double)rand()/(double)(RAND_MAX))*(n_objects-i+1);
          k+=i;
          (pobjects+i)->selected=(pobjects+k)->candidates;
          (pobjects+k)->candidates=(pobjects+i)->candidates;
         }
      res=odd_coloring(size,x_dimension,y_dimension,pobjects,
          pgraph,ppath);
      if (res<1)
         {n_failures++;
          ((pobjects+size)->fail)++;(pobjects->fail)++;
          continue;
         }
      ((pobjects+size)->n)++;
      i=((double)rand()/(double)(RAND_MAX))*graph_weight_bound;wei=i+1;
      *weigh=wei*(size/2);
      for (i=1;i<pobjects->set1;i++)
         {i1=(pobjects+i)->set1;
          for (j=i+1;j<=pobjects->set1;j++)
             {j1=(pobjects+j)->set1;
              lo=weights(i1,j1)-(long)wei;
              if (lo<-INTLIMIT) {printf("  'int' overflow\n");exit(1);}
              weights(i1,j1)=(int)lo;weights(j1,i1)=weights(i1,j1);
             }
         }
      for (i=1;i<pobjects->set2;i++)
         {i1=(pobjects+i)->set2;
          for (j=i+1;j<=pobjects->set2;j++)
             {j1=(pobjects+j)->set2;
              lo=weights(i1,j1)-(long)wei;
              if (lo<-INTLIMIT) {printf("  'int' overflow\n");exit(1);}
              weights(i1,j1)=(int)lo;weights(j1,i1)=weights(i1,j1);
             }
         }
      for (i=1;i<=pobjects->set1;i++)
         {i1=(pobjects+i)->set1;
          for (j=1;j<=pobjects->set2;j++)
             {j1=(pobjects+j)->set2;
              lo=weights(i1,j1)+(long)wei;
              if (lo>INTLIMIT) {printf("  'int' overflow\n");exit(1);}
              weights(i1,j1)=(int)lo;weights(j1,i1)=weights(i1,j1);
             }
         }
      return 1;
     } /* while */
  return 0;
 }
