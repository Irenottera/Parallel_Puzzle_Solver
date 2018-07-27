#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <complex.h>
#include <mpi.h>
#include <math.h>
#define N 100
 
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
 
int takedata(FILE *myFile, element **M) {
    int i,j;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++){
            fscanf(myFile, "%d %d %d %d %d %d %d %d", &M[i][j].n_s, &M[i][j].n_c,
            &M[i][j].s_s, &M[i][j].s_c, &M[i][j].o_s, &M[i][j].o_c, &M[i][j].e_s, &M[i][j].e_c);
            M[i][j].ord=0;
        }
    }
    return 1;
}
 
int printdata (element **M) {
    int i,j, ordd;
    ordd=1;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++){
            printf("%d %d %d %d %d %d %d %d %d %d \n", ordd, M[i][j].n_s, M[i][j].n_c,
            M[i][j].s_s, M[i][j].s_c, M[i][j].o_s, M[i][j].o_c, M[i][j].e_s, M[i][j].e_c, M[i][j].ord);
            ordd++;
        }
    }
    return 1;
}
 
void swap(int *a, int *b) {
    int tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}
 
void swapel(element *M1, element *M2) {
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
 
int opposhape(int *c) {
    int z;
    z=(*c % 2)+1;
    return z;
}
typedef struct {
    int r;
    int c;
} pos;
 
 
int main( int argc, char *argv[]) {
 
    int i, j, k, l, p, P;
    element **M;
    FILE* myFile = NULL;
    int tag1, tag2, tag3, tag4;
 
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &P);
    MPI_Comm_rank(MPI_COMM_WORLD, &p);
    MPI_Status status;
 
    const int nitems = 9;
    int blocklengths [9] = {1,1,1,1,1,1,1,1,1};
    MPI_Datatype types [9] = {MPI_INT, MPI_INT, MPI_INT, MPI_INT, MPI_INT, MPI_INT, MPI_INT, MPI_INT, MPI_INT};
    MPI_Datatype mpi_element;
    MPI_Aint offsets[9];
    offsets[0] = offsetof(element, n_s);
    offsets[1] = offsetof(element, n_c);
    offsets[2] = offsetof(element, s_s);
    offsets[3] = offsetof(element, s_c);
    offsets[4] = offsetof(element, o_s);
    offsets[5] = offsetof(element, o_c);
    offsets[6] = offsetof(element, e_s);
    offsets[7] = offsetof(element, e_c);
    offsets[8] = offsetof(element, ord);
    MPI_Type_create_struct(nitems, blocklengths, offsets, types, &mpi_element);
    MPI_Type_commit(&mpi_element);
 
    M=(element **) malloc (sizeof(element *)*N);
    for (i=0; i<N; i++){
        M[i] = (element *) malloc(sizeof(element)*N);
    }
 
    myFile = fopen(argv[1], "r");
    if (myFile == NULL){
        printf("Error Reading File\n");
        exit (0);
    }
    takedata(myFile, M);
    fclose(myFile);
 
    for (i=0; i<N; i++){
        for (j= 0; j < N ; j++){
 
            if (p==0) {
                if (M[i][j].n_s == 0 && M[i][j].o_s == 0) {
                    swapel(&M[i][j], &M[0][0]);
                }
            }
 
            if (p==(P/2)-1) {
                if (M[i][j].n_s == 0 && M[i][j].e_s == 0) {
                    swapel(&M[i][j], &M[0][N-1]);
                }
            }
 
            if (p==P-1) {
                if (M[i][j].s_s == 0 && M[i][j].o_s == 0) {
                    swapel(&M[i][j], &M[N-1][0]);
                }
            }
 
            if (p==P/2) {
                if (M[i][j].s_s == 0 && M[i][j].e_s == 0) {
                    swapel(&M[i][j], &M[N-1][N-1]);
                }
            }
        }
    }
 
    int m, n, q;
    int h = (N/(P/2));
    float t;
    t=((4*p)/P);
    q=(int)t;
 
    tag1 = 1;
    tag2 = 1;
    tag3 = 1;
    tag4 = 1;
    element Maux;
 
    if (p!=0 && p!=(P/2)-1 && p!=(P/2) && p!= P-1){
        if (q==0) {
            MPI_Recv(&Maux, 1, mpi_element, p-1, tag1, MPI_COMM_WORLD, &status);
        }
        else if (q==2) {
            MPI_Recv(&Maux, 1, mpi_element, p-1, tag3, MPI_COMM_WORLD, &status);
        }
        else if(q==1) {
            MPI_Recv(&Maux, 1, mpi_element, p+1, tag2, MPI_COMM_WORLD, &status);
        }
        else if (q==3) {
            MPI_Recv(&Maux, 1, mpi_element, p+1, tag4, MPI_COMM_WORLD, &status);
        }
    }
 
    for (k=0; k<N/2; k++){
        for (l=p*h; l<(p+1)*h; l++){
            for (i=0; i<N; i++){
                for (j=0; j<N; j++){
 
                    if (q==0) {
                        if (k==0 && l==p*h) {
                            if ((M[k][l].ord==0) && (M[i][j].o_s == opposhape(&Maux.e_s))
                            && (M[i][j].o_c == Maux.e_c) && (M[i][j].n_s==0)) {
                                swapel(&M[i][j], &M[k][l]);
                            }
                        }
                        if (k==0 && l!=p*h) {
                            if ((M[k][l].ord==0) && (M[i][j].o_s == opposhape(&M[k][l-1].e_s))
                            && (M[i][j].o_c == M[k][l-1].e_c) && (M[i][j].n_s==0)) {
                                swapel(&M[i][j], &M[k][l]);
                            }
                        }
                        if ((k!=0)  && (l!=p*h) ){
                            if ((M[k][l].ord==0) && (M[i][j].o_s == opposhape(&M[k][l-1].e_s))
                            && (M[i][j].o_c == M[k][l-1].e_c) && (M[i][j].n_s==opposhape(&M[k-1][l].s_s)) && (M[i][j].n_c==M[k-1][l].s_c) ) {
                                swapel(&M[i][j], &M[k][l]);
                            }
                        }
                        if (l==p*h && k!=0){
                            if ((M[k][l].ord==0) && (M[i][j].n_s == opposhape(&M[k-1][l].s_s))
                            && (M[i][j].n_c==M[k-1][l].s_c)) {
                                swapel(&M[i][j], &M[k][l]);
                            }
                        }
                        if (p!=(P/4)-1 && k==0 && l==(p+1)*h-1 && M[k][l].ord == 1 ){
                            MPI_Send(&M[k][l], 1, mpi_element, p+1, tag1, MPI_COMM_WORLD);
                        }
                    }
 
                    else if (q==1) {
                        m=(2*p+1)*h-l-1;
                        if (k==0 && l==p*h) {
                            if ((M[k][m].ord==0) && (M[i][j].e_s == opposhape(&Maux.o_s))
                            && (M[i][j].e_c == Maux.o_c) && (M[i][j].n_s==0)) {
                                swapel(&M[i][j], &M[k][m]);
                              }
                        }
                        if (k==0 && l!=p*h) {
                            if ((M[k][m].ord==0) && (M[i][j].e_s == opposhape(&M[k][m+1].o_s))
                            && (M[i][j].e_c == M[k][m+1].o_c) && (M[i][j].n_s==0)) {
                                swapel(&M[i][j], &M[k][m]);
                            }
                        }
                        if ((k!=0)  && (l!=p*h)) {
                            if ((M[k][m].ord==0) && (M[i][j].e_s == opposhape(&M[k][m+1].o_s))
                            && (M[i][j].e_c == M[k][m+1].o_c) && (M[i][j].n_s==opposhape(&M[k-1][m].s_s)) && (M[i][j].n_c==M[k-1][m].s_c) ){
                                swapel(&M[i][j], &M[k][m]);
                            }
                        }
                        if (l==p*h && k!=0) {
                            if ((M[k][m].ord==0) && (M[i][j].n_s == opposhape(&M[k-1][m].s_s))
                            && (M[i][j].n_c==M[k-1][m].s_c) ){
                                swapel(&M[i][j], &M[k][m]);
                            }
                        }
                        if (p!=(P/4) && k==0 && l==(p+1)*h-1 && M[k][m].ord == 1 ){
                          MPI_Send(&M[k][m], 1, mpi_element, p-1, tag2, MPI_COMM_WORLD);
                        }
                    }
                    else if (q==2) {
                        n=N-k-1;
                        m=(2*N-l-1);
                        if (k==0 && l==p*h) {
                            if ((M[n][m].ord==0) && (M[i][j].e_s == opposhape(&Maux.o_s))
                            && (M[i][j].e_c == Maux.o_c) && (M[i][j].s_s==0)) {
                                swapel(&M[i][j], &M[n][m]);
                            }
                        }
                        if (k==0 && l!=p*h) {
                            if ((M[n][m].ord==0) &&(M[n][m+1].ord==1) && (M[i][j].e_s == opposhape(&M[n][m+1].o_s))
                            && (M[i][j].e_c == M[n][m+1].o_c) && (M[i][j].s_s==0)){
                                swapel(&M[i][j], &M[n][m]);
                            }
                        }
                        if ((k!=0)  && (l!=p*h)) {
                            if ((M[n][m].ord==0) &&(M[n+1][m].ord==1)&&(M[n][m+1].ord==1) && (M[i][j].e_s == opposhape(&M[n][m+1].o_s))
                            && (M[i][j].e_c == M[n][m+1].o_c) && (M[i][j].s_s==opposhape(&M[n+1][m].n_s)) && (M[i][j].s_c==M[n+1][m].n_c)) {
                                swapel(&M[i][j], &M[n][m]);
                            }
                        }
                        if (l==p*h && k!=0) {
                            if( (M[n][m].ord==0) &&(M[n+1][m].ord==1)&& (M[i][j].s_s == opposhape(&M[n+1][m].n_s))
                            && (M[i][j].s_c==M[n+1][m].n_c) ) {
                                swapel(&M[i][j], &M[n][m]);
                            }
                        }
                        if (p!=(3/4)*P-1 && k==0 && l==(p+1)*h-1 && M[n][m].ord == 1 ){
                          MPI_Send(&M[n][m], 1, mpi_element, p+1, tag3, MPI_COMM_WORLD);
                        }
                    }
                    else if (q==3) {
                        n=N-k-1;
                        m=l-(2*p+1)*h+2*N;
                        if (k==0 && l==p*h) {
                            if ((M[n][m].ord==0) && (M[i][j].o_s == opposhape(&Maux.e_s))
                            && (M[i][j].o_c == Maux.e_c) && (M[i][j].s_s==0)) {
                                swapel(&M[i][j], &M[n][m]);
                              }
                        }
                        if (k==0 && l!=p*h) {
                            if ((M[n][m].ord==0) && (M[i][j].o_s == opposhape(&M[n][m-1].e_s))
                            && (M[i][j].o_c == M[n][m-1].e_c) && (M[i][j].s_s==0)) {
                                swapel(&M[i][j], &M[n][m]);
                            }
                        }
                        if ((k!=0)  && (l!=p*h)) {
                            if ((M[n][m].ord==0) && (M[i][j].o_s == opposhape(&M[n][m-1].e_s))
                            && (M[i][j].o_c == M[n][m-1].e_c) && (M[i][j].s_s==opposhape(&M[n+1][m].n_s)) && (M[i][j].s_c==M[n+1][m].n_c)) {
                                swapel(&M[i][j], &M[n][m]);
                            }
                        }
                        if (l==p*h && k!=0) {
                            if ((M[n][m].ord==0) && (M[i][j].s_s == opposhape(&M[n+1][m].n_s))
                            && (M[i][j].s_c==M[n+1][m].n_c) ) {
                                swapel(&M[i][j], &M[n][m]);
                            }
                        }
                        if (p!=(3/4)*P && k==0 && l==(p+1)*h-1 && M[n][m].ord == 1 ){
                          MPI_Send(&M[n][m], 1, mpi_element, p-1, tag4, MPI_COMM_WORLD);
                        }
                    }
                }
            }
        }
    }


printf ("OK from %d \n",p);
/* 
    int tag;
    tag = p;
    element Mp;
    for (k=0; k<N/2; k++){
        for (l=p*h; l<(p+1)*h; l++){
            if (q==0 && p!=0) {
                MPI_Send(&M[k][l], 1, mpi_element, 0, tag, MPI_COMM_WORLD);
                tag++;
            }
            if (q==1) {
                m=(2*p+1)*h-l-1;
                MPI_Send(&M[k][m], 1, mpi_element, 0, tag, MPI_COMM_WORLD);
                tag++;
            }
            if (q==2) {
                n=N-k-1;
                m=(2*N-l-1);
                MPI_Send(&M[n][m], 1, mpi_element, 0, tag, MPI_COMM_WORLD);
                tag++;
            }
            if (q==3) {
                n=N-k-1;
                m=l-(2*p+1)*h+2*N;
                MPI_Send(&M[n][m], 1, mpi_element, 0, tag, MPI_COMM_WORLD);
                tag++;
            }
            if (p==0) {
                int q0;
                float t0;
                for (i=1; i<P; i++){
                    t0=((4*i)/P);
                    q0=(int)t0;
                    if (q0==0) {
                        tag1 = i+k+l-p*h;
                        MPI_Recv(&Mp, 1, mpi_element, i, tag1, MPI_COMM_WORLD, &status);
                        swapel(&Mp, &M[k][l]);
                        printf ("swapped from %d \n",i);
                    }
                    if (q0==1) {
                        tag2 = i+k+l-p*h;
                        m=(2*p+1)*h-l-1;
                        MPI_Recv(&Mp, 1, mpi_element, i, tag2, MPI_COMM_WORLD, &status);
                        swapel(&Mp, &M[k][m]);
                        printf ("swapped from %d \n",i);
                    }
                    if (q0==2) {
                        tag3 = i+k+l-p*h;
                        n=N-k-1;
                        m=(2*N-l-1);
                        MPI_Recv(&Mp, 1, mpi_element, i, tag3, MPI_COMM_WORLD, &status);
                        swapel(&Mp, &M[n][m]);
                        printf ("swapped from %d \n",i);
                    }
                    if (q0==3) {
                        tag4 = i+k+l-p*h;
                        n=N-k-1;
                        m=l-(2*p+1)*h+2*N;
                        MPI_Recv(&Mp, 1, mpi_element, i, tag4, MPI_COMM_WORLD, &status);
                        swapel(&Mp, &M[n][m]);
                        printf ("swapped from %d \n",i);
                    }
                }
            }
        }
    }
 
*/
                 
                  
                      return 0;
                      }
