#include<stdio.h>
#include<stdlib.h>
#include <complex.h>
#include <math.h>
#include <time.h>
#define N 200
 
typedef struct {
    int n_s;
    int n_c;
    int s_s;
    int s_c;
    int o_s;
    int o_c;
    int e_s;
    int e_c;
    int ord;
} element;
 
 
int takedata (FILE *myFile, element M[N][N]) {
    int i,j;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++){
            fscanf(myFile, "%d %d %d %d %d %d %d %d", &M[i][j].n_s, &M[i][j].n_c, &M[i][j].s_s, &M[i][j].s_c, &M[i][j].o_s, &M[i][j].o_c, &M[i][j].e_s, &M[i][j].e_c);
            M[i][j].ord = 0;
        }
    }
    return 1;
}
 
int printdata (element M[N][N]) {
    int i,j, ordd;
    ordd=1;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++){
            printf("%d %d %d %d %d %d %d %d %d %d \n", ordd, M[i][j].n_s, M[i][j].n_c, M[i][j].s_s, M[i][j].s_c, M[i][j].o_s, M[i][j].o_c, M[i][j].e_s, M[i][j].e_c, M[i][j].ord);
            ordd++;
        }
    }
    return 1;
}
 
void swap (int *a, int *b) {
    int tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}
 
void swapel (element *M1, element *M2) {
    swap(&(*M1).n_s, &(*M2).n_s);
    swap(&(*M1).n_c, &(*M2).n_c);
    swap(&(*M1).s_s, &(*M2).s_s);
    swap(&(*M1).s_c, &(*M2).s_c);
    swap(&(*M1).o_s, &(*M2).o_s);
    swap(&(*M1).o_c, &(*M2).o_c);
    swap(&(*M1).e_s, &(*M2).e_s);
    swap(&(*M1).e_c, &(*M2).e_c);
    (*M2).ord = 1;
}
 
void swapel2 (element *M1, element *M2) {
    swap(&(*M1).n_s, &(*M2).n_s);
    swap(&(*M1).n_c, &(*M2).n_c);
    swap(&(*M1).s_s, &(*M2).s_s);
    swap(&(*M1).s_c, &(*M2).s_c);
    swap(&(*M1).o_s, &(*M2).o_s);
    swap(&(*M1).o_c, &(*M2).o_c);
    swap(&(*M1).e_s, &(*M2).e_s);
    swap(&(*M1).e_c, &(*M2).e_c);
}
 
int opposhape(int *c){
  int z;
  z=(*c % 2)+1;
  return z;
}
 
int main ( int argc, char *argv[]) {
clock_t begin = clock();
 
    element M[N][N];
    int i, j, k, l;
    FILE *myFile;
 
    myFile = fopen(argv[1], "r");
    if (myFile == NULL) {
        printf("Error Reading File\n");
        exit (0);
    }
    takedata(myFile, M);
    fclose(myFile);
 
 
    for (k=0; k<4; k++){
        for (i=0; i<N; i++){
            for (j=0; j<N; j++){
 
                if (k==0 && M[i][j].n_s == 0 && M[i][j].o_s == 0) {
                    swapel(&M[i][j], &M[0][0]);
                }
 
                if (k==1 && M[i][j].n_s == 0 && M[i][j].e_s == 0){
                    swapel(&M[i][j], &M[0][N-1]);
                }
 
                if (k==2 && M[i][j].s_s == 0 && M[i][j].o_s == 0){
                    swapel(&M[i][j], &M[N-1][0]);
                }
 
                if (k==3 && M[i][j].s_s == 0 && M[i][j].e_s == 0){
                    swapel(&M[i][j], &M[N-1][N-1]);
                }
 
            }
        }
    }
 
for (k=0; k<(N-2); k++){
      for (i=0; i<N; i++){
            for (j=0; j<N; j++){
              if (M[i][j].n_s==0 && M[i][j].o_s == opposhape(&M[0][k].e_s) && M[i][j].o_c == M[0][k].e_c){
                swapel(&M[i][j], &M[0][k+1]);

              }
 
            }
      }
}
for (k=0; k<(N-2); k++){
      for (i=0; i<N; i++){
            for (j=0; j<N; j++){
              if (M[i][j].e_s==0 && M[i][j].n_s == opposhape(&M[k][N-1].s_s) && M[i][j].n_c == M[k][N-1].s_c){
                swapel(&M[i][j], &M[k+1][N-1]);

              }
 
            }
      }
}
 
for (k=0; k<(N-2); k++){
      for (i=0; i<N; i++){
            for (j=0; j<N; j++){
              if (M[i][j].o_s==0 && M[i][j].n_s == opposhape(&M[k][0].s_s) && M[i][j].n_c == M[k][0].s_c){
                swapel(&M[i][j], &M[k+1][0]);

              }
 
            }
      }
}
for (k=0; k<(N-2); k++){
      for (i=0; i<N; i++){
            for (j=0; j<N; j++){
             
                            if (M[i][j].s_s==0 && M[i][j].o_s == opposhape(&M[N-1][k].e_s) && M[i][j].o_c == M[N-1][k].e_c){
                                            swapel(&M[i][j], &M[N-1][k+1]);
                                                         
                                                                         }
                                                                          
                                                                                      }
                                                                                            }
                                                                                            }
                                                                                             
                                                                                              
                                                                                               
                                                                                                   for (k=1; k<N-1; k++){
                                                                                                           for (l=1; l<N-1; l++){
                                                                                                                       for (i=k; i<N-1; i++){
                                                                                                                                       for (j=1; j<N-1; j++){
                                                                                                                                                           if (M[i][j].o_s != M[k][l-1].e_s && M[i][j].o_c == M[k][l-1].e_c) {
                                                                                                                                                                                   swapel(&M[i][j], &M[k][l]);
                                                                                                                                                                                                       }
                                                                                                                                                                                                                       }
                                                                                                                                                                                                                                }
                                                                                                                                                                                                                                        }
                                                                                                                                                                                                                                            }
                                                                                                                                                                                                                                               
clock_t end = clock();
double time_spent = (double) (end - begin)/ CLOCKS_PER_SEC;
printf ("%lf", time_spent);                                                                                                                                                                                                                                                                                                                                                                                                                                                        
                                                                                                                                                                                                                                                         return 0;
                                                                                                                                                                                                                                                         }
