#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>


int main(int arc, char ** argv){



    int iterations[] = {10,100,500,1000,5000};

    int results[sizeof(iterations)/sizeof(iterations[0])];


    size_t results_len = sizeof(results)/sizeof(results[0]);

    for (int idx = 0; idx < results_len; idx++){
    size_t n = iterations[idx];

    // int A[n][n];
    // int B[n][n];
    int **A = malloc(n * sizeof(int*));
    int **B = malloc(n * sizeof(int*));
    int **C = malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++){
        A[i] = malloc(n * sizeof(int));
        B[i] = malloc(n * sizeof(int));
        C[i] = malloc(n * sizeof(int));
    }

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

 

    // int C[n][n];



    for(int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            C[i][j] = 0;
        }
    }

    printf("starting the matrix multiplicationb for n = %ld\n", n);

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

    printf("completed the matrix multiplication for n = %ld\n", n);

    long total_operations =  2 * n * n * n; // 2 * n^3 operations for matrix multiplication

    printf("Total operations for n value of %ld: %ld\n", n, total_operations);

    double computation_rate = total_operations / total_time;
    printf("Time taken for matrix multiplication: %f seconds\n", total_time);
    printf("Computation rate for n value of %ld: %f operations/second\n", n, computation_rate);

    results[idx] = computation_rate;
    
    for (int i = 0; i < n; i++){
        free(A[i]);
        free(B[i]);
        free(C[i]);
    }
    free(A);
    free(B);
    free(C);

}


for (int i = 0; i < results_len; i++){
    printf("Results[%d] = %d operations/second\n", iterations[i], results[i]);  


}
}