#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>


int main(int arc, char ** argv){



    
    size_t n = 10;



    int A[n][n];
    int B[n][n];
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            if (i == j){
                A[i][j] = 1;
            } else if (i < j){
                B[i][j] = 10;
                A[i][j] = 2;
            }else{
                B[i][j] = 20;
                A[i][j] = 2;
            }
        }
    }

    int C[n][n];

    for(int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            C[i][j] = 0;
        }
    }

    struct timeval start, end;
    long seconds, useconds;
    double total_time;

    gettimeofday(&start, NULL);

    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            for (int k = 0; k < n; k++){
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    gettimeofday(&end, NULL);
    seconds  = end.tv_sec  - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;
    total_time = seconds + useconds/1000000.0;

    long total_operations =  2 * n * n * n; // 2 * n^3 operations for matrix multiplication

    double computation_rate = total_operations / total_time;
    printf("Time taken for matrix multiplication: %f seconds\n", total_time);
    printf("Computation rate: %f operations/second\n", computation_rate);



}