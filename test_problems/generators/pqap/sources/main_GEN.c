// #include <alloc.h>
// #include <process.h>
#ifdef _WIN32
    #include <direct.h>
#else
    #include <sys/stat.h>
    #include <sys/types.h>
#endif
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "GENER6.H"

void generator6(FILE *,int,int,int,long,int,int,int,int,int *,
                double *,double *,Matrices *,Objects *,Graph *,
                Path *,Grid_structure *);

void main(int argc,char **argv)
 {FILE *out,*out_sln,*in;
  Matrices *pmatrices;
  Objects *pobjects;
  Graph *pgraph;
  Path *ppath;
  Grid_structure *pgrid_structure;
  int i,j;
  int inst;
  int ind1,ind2,numb;
  int n_objects,graph_weight_bound;
  int size_bound,size_lower_bound;
  int n_instances;
  int bound_on_trials,failure;
  int x_dimension,y_dimension;
  long n_graphs;
  long lo,term;
  double seed;
  double minimal_value,optimal_value;
  char file_name[PATH_MAX],file_name_out[PATH_MAX],file_name_sln[PATH_MAX], string[PATH_MAX];

  if (argc>1)
     {strcpy(file_name,argv[1]);
      if ((in=fopen(file_name,"r"))==NULL)
         {printf("  fopen failed for input");exit(1);}
      fscanf(in,"%d %d %d %ld %d %d %d %lf %d",
             &n_objects,&x_dimension,&y_dimension,&n_graphs,
             &size_bound,&size_lower_bound,&graph_weight_bound,&seed,
             &n_instances);
      if (size_lower_bound<3) size_lower_bound=3;
      if (size_bound%2==0) size_bound--;
      if (size_lower_bound%2==0) size_lower_bound--;
     }
   else
     {printf("     Give the following values:\n");
      printf("  number n of objects=");
      failure=1;
      while(failure)
         {gets(string);
          if (atoi(string)<=0) printf("  try again");
            else
                 {numb=atoi(string);
                  if (numb<6) printf("  try again");
                    else {n_objects=numb;failure=0;}
                 }
         }
      printf("  grid x-dimension=");
      failure=1;
      while(failure)
         {gets(string);
          if (atoi(string)<=0) printf("  try again");
            else
                 {numb=atoi(string);
                  if (numb<1) printf("  try again");
                    else {x_dimension=numb;failure=0;}
                 }
         }
      printf("  grid y-dimension=");
      failure=1;
      while(failure)
         {gets(string);
          if (atoi(string)<=0) printf("  try again");
            else
                 {numb=atoi(string);
                  if (numb<1) printf("  try again");
                    else {y_dimension=numb;failure=0;}
                 }
         }
      if (x_dimension*y_dimension<n_objects)
         {printf(
          "  number of objects is invalid\n");
          printf("  - program is aborted\n");
          exit(1);
         }
      printf("  number of graphs=");
      failure=1;
      while(failure)
         {gets(string);
          if (atoi(string)<=0) printf("  try again");
            else
                 {lo=atoi(string);
                  if (lo<1) printf("  try again");
                    else {n_graphs=lo;failure=0;}
                 }
         }
      printf("  upper bound on the size of graphs=");
      failure=1;
      while(failure)
         {gets(string);
          numb = atoi(string);
          if (numb%2==0) numb--;
          if (numb<3 || numb>n_objects) printf("  try again");
            else {size_bound=numb;failure=0;}
         }
      printf("  lower bound on the size of graphs=");
      failure=1;
      while(failure)
         {gets(string);
          numb = atoi(string);
          if (numb%2==0) numb--;
          if (numb>size_bound) printf("  try again");
            else 
                 {if (numb<3) size_lower_bound=3;
                    else size_lower_bound=numb;
                  failure=0;
                 }
         }
      printf(
 "  upper bound on the positive weight of a graph (recommended<=100) =");
      failure=1;
      while(failure)
         {gets(string);
          if (atoi(string)<=0) printf("  try again");
            else
                 {numb=atoi(string);
                  if (numb<1) printf("  try again");
                    else {graph_weight_bound=numb;failure=0;}
                 }
         }
      printf("  seed for random number generator=");
      failure=1;
      while(failure)
         {gets(string);
          if (atoi(string)<=0) printf("  try again");
            else
                 {lo=atoi(string);
                  if (lo<1) printf("  try again");
                    else {seed=lo;failure=0;}
                 }
         }
      printf("  number of instances=");
      failure=1;
      while(failure)
         {gets(string);
          if (atoi(string)<=0) printf("  try again");
            else {n_instances=atoi(string);failure=0;}
         }
     }

  for (inst=1;inst<=n_instances;inst++) {
    // Create directory name and create it if it doesn't exist
    char dir_name[PATH_MAX];
    snprintf(dir_name, PATH_MAX, "data/pqap_%d", n_objects);
    #ifdef _WIN32
        _mkdir(dir_name);
    #else
        char mkdir_cmd[PATH_MAX + 10];
        snprintf(mkdir_cmd, PATH_MAX + 10, "mkdir -p %s", dir_name);
        system(mkdir_cmd);
    #endif

    // Create full file paths including directory
    snprintf(file_name_out, PATH_MAX, "%s/pqap_%d_%d.dat", dir_name, n_objects, inst);
    snprintf(file_name_sln, PATH_MAX, "%s/pqap_%d_%d.sln", dir_name, n_objects, inst);
    
    // update seed
    seed = seed + inst * 2024;

    if ((out=fopen(file_name_out,"w"))==NULL)
       {printf("  fopen failed for output");exit(1);}
    if ((out_sln=fopen(file_name_sln,"w"))==NULL)
       {printf("  fopen failed for solution output");exit(1);}

    ALS(pobjects,Objects,n_objects+1)
    ALS(pgraph,Graph,n_objects+2)
    ALS(ppath,Path,2*n_objects+1)
    ALS(pmatrices,Matrices,n_objects+1)
    for (i=1;i<=n_objects;i++) ALI((pmatrices+i)->weights,n_objects+1)
    for (i=1;i<=n_objects;i++) ALI((pmatrices+i)->distances,n_objects+1)
   
    bound_on_trials=BT;

    generator6(out,n_objects,x_dimension,y_dimension,n_graphs,
                size_bound,size_lower_bound,graph_weight_bound,
                bound_on_trials,&failure,&minimal_value,&seed,
                pmatrices,pobjects,pgraph,ppath,pgrid_structure);
    if (failure)
         {fprintf(out,"  failure in test problem construction\n");
          fprintf(out,"  (advice: apply with smaller bound on graph size)\n");
          break;
         }
         // Output problem size
         fprintf(out, "%d\n", n_objects);
         /* Output of flow and distance matrices  */
      for (i=1;i<=n_objects;i++)
         {for (j=1;j<=n_objects;j++)
              fprintf(out,"%4d",weights(i,j));
          fprintf(out,"\n");
         }
      fprintf(out,"\n");

      for (i=1;i<=n_objects;i++)
         {for (j=1;j<=n_objects;j++)
              fprintf(out,"%4d",distances(i,j));
          fprintf(out,"\n");
         }
      fprintf(out,"\n");

      for (i=1;i<=x_dimension;i++)
          if ((pmatrices+i)->grid!=NULL) free((pmatrices+i)->grid);
      for (i=1;i<=n_objects;i++)
          (pobjects+(pobjects+i)->optimal)->candidates=i;
      for (i=1;i<=n_objects;i++)
          (pobjects+i)->optimal=(pobjects+i)->candidates;
      for (i=1,optimal_value=0;i<=n_objects;i++)
         {ind1=(pobjects+i)->optimal;
          for (j=1;j<=n_objects;j++)
             {ind2=(pobjects+j)->optimal;
              term=(long)weights(i,j)*distances(ind1,ind2);
              optimal_value+=term;
             }
            }
      fprintf(out_sln,"%4d %12lf\n",n_objects,optimal_value);
      for (i=1; i<=n_objects; i++) {
          ind1=(pobjects+i)->optimal;
          fprintf(out_sln, "%4d ", ind1-1); // 0-indexed
      }
      fprintf(out_sln, "\n");

      fclose(out);
      fclose(out_sln);
  }
  fclose(in);

  for (i=1;i<=n_objects;i++)
      if ((pmatrices+i)->weights!=NULL) free((pmatrices+i)->weights);
  for (i=1;i<=n_objects;i++)
      if ((pmatrices+i)->distances!=NULL) free((pmatrices+i)->distances);
  if (pmatrices!=NULL) free(pmatrices);
  if (pobjects!=NULL) free(pobjects);
  if (pgraph!=NULL) free(pgraph);
  if (ppath!=NULL) free(ppath);
 }
