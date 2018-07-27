#include<stdio.h>
#include<stdlib.h>
#include <complex.h>
/*#include <mpi.h>*/
#include <math.h>
#include <time.h>
#define N 12
 
/*
 * typedef struct {
 *     int n_s;
 *         int n_c;
 *             int s_s;
 *                 int s_c;
 *                     int o_s;
 *                         int o_c;
 *                             int e_s;
 *                                 int e_c;
 *                                     int ord;
 *                                     } element;
 *                                     */
 
int shape() {
  /* function assigning shape to each side of an element */
 
    double x;
    x = ((double) rand())/(RAND_MAX);
    if ( x < 0.5 ) {
        return 1;
    }
    else {
         return 2;
    }
}
 
int position() {
  /* function assigning a random position to an element on the grid. This functions gives in output an integer with
 *   value between 0 and N*N */
    double x;
    int y;
 
    x = ((double) rand())/(RAND_MAX)*N*N;
    y =(int)x;
    return y;
}
 
 
int main() {
    /* create matrix */
    int M[N][N][8];
    int i, j, k, v, p, tmp;
    v=1;
    FILE *fp;
    fp = fopen("puzzle12","w");
    if (fp == NULL){
        printf("Error file!\n");
        exit(EXIT_FAILURE);
    }
    srand(time(NULL));
 
    /* set shape = 0, colour = 0 on borders */
    for (i=0; i<N; i++){
        M[0][i][0]=0;
        M[i][0][4]=0;
        M[N-1][i][2]=0;
        M[i][N-1][6]=0;
        M[0][i][1]=0;
        M[i][0][5]=0;
        M[N-1][i][3]=0;
        M[i][N-1][7]=0;
    }
    /* random shape assignment, male or female */
    for (i=0; i<N; i++){
        for (j=0; j<N-1; j++){
            M[i][j][6] = shape();
            M[i][j+1][4] = (M[i][j][6] % 2)+1;
            M[j][i][2] = shape();
            M[j+1][i][0] = (M[j][i][2] % 2) +1;
        }
    }
 
    /*colour assignment */
 
 
    for (i=0; i<N; i++){
        for (j=0; j<(N-1); j++){
            M[i][j][7] = v;
            M[i][j+1][5] = M[i][j][7];
            v++;
            M[j][i][3] = v;
            M[j+1][i][1] = M[j][i][3];
            v++;
        }
    }
    /* fprintf(fp, "Matrice ordinata \n");
 *     for (i = 0; i < N; i++) {
 *             for (j = 0; j < N; j++){
 *                         for (k = 0; k < 8; k++){
 *                                         fprintf(fp, "%d ", M[i][j][k]);
 *                                                     }
 *                                                                 fprintf(fp, "\n");
 *                                                                         }
 *                                                                             }
 *                                                                                 */
 
    /* Random Switch of elements in the matrix */
    for (i=0; i<N; i++){
        for (j=0; j<N; j++){
            p=position();
            for (k=0; k<8; k++){
              tmp = M[i][j][k];
              M[i][j][k] = M[(int)p/N][p%N][k];
              M[(int)p/N][p%N][k] = tmp;
 
            }
        }
    }
 
    i=0;
    j=0;
    k=0;
    /* file creation */
 
    /*fprintf(fp, "Matrice disordinata \n");*/
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++){
            for (k = 0; k < 8; k++){
                fprintf(fp, "%d ", M[i][j][k]);
            }
            fprintf(fp, "\n");
        }
    }
    fclose(fp);
 
    return 0;
}
