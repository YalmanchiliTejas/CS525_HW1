#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>

double get_time(){
  struct timeval t;
  gettimeofday(&t, NULL);
  return t.tv_sec + t.tv_usec * 1e-6;

}

void mult_row_1d(double *A, double*x, double* y, int n, int rank, int p){
    int num_local_rows = n/p;
    int local_x = num_local_rows;

    double *x_res = (double *) malloc(n * sizeof(double));

    MPI_Allgather(x, local_x, MPI_DOUBLE, x_res, local_x, MPI_DOUBLE, MPI_COMM_WORLD);

    for(int i =  0; i < num_local_rows; i++){
        y[i] = 0.0;
        for(int j = 0; j < n; j++){
            y[i] += A[i * n +  j] * x_res[j];
        }
    }
    free(x_res);
}


void mult_row_2d(double *A, double*x, double* y, int n, int rank, int p){


    int dimensions[2] = {0,0};

    MPI_Comm grid_communication, row_communication, col_communication;

    int time_period[2] = {0,0};
    int coordinates[2];

    MPI_Dims_create(p, 2, dimensions);
    MPI_Cart_create(MPI_COMM_WORLD, 2, dimensions, time_period, 0, &grid_communication);
    MPI_Cart_coords(grid_communication, rank, 2 , coordinates);

    int row = coordinates[0];
    int col = coordinates[1];

    int block_rows = n/dimensions[0];
    int block_cols = n / dimensions[1];

    MPI_Comm_split(grid_communication, row, col, &row_communication);
    MPI_Comm_split(grid_communication, col, row, &col_communication);

    double *x_col = (double *)malloc(block_cols * sizeof(double));

    double *partial_rows = (double *) malloc(block_rows * sizeof(double));

    for(int i = 0; i < block_cols; i++){
        x_col[i] = x[i];
    }
    MPI_Bcast(x_col, block_cols, MPI_DOUBLE, 0, col_communication);

    for(int i = 0; i < block_rows; i++){
        partial_rows[i] = 0.0;
        for(int j = 0; j < block_cols; j++){
            partial_rows[i] += A[i * block_cols + j ] * x_col[j];
        }
    }
    MPI_Reduce(partial_rows, y, block_rows, MPI_DOUBLE, MPI_SUM, 0, row_communication);

    free(x_col);
    free(partial_rows);
    MPI_Comm_free(&row_communication);
    MPI_Comm_free(&col_communication);
    MPI_Comm_free(&grid_communication);

}

int main(int argc, char *argv[]){

    int rank, p;
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);


    double W = atoi(argv[1]);

 
    int n = p * ceil(512.0 * sqrt(W));
    


    int local_rows = n/p;
    int local_x = local_rows;

    double *A = (double*)malloc(local_rows * n * sizeof(double));
    double *x = (double*) malloc(local_x * sizeof(double));
    double *y = (double*) malloc(local_rows * sizeof(double));


    for(int i = 0; i < local_rows; i++){
        for(int j = 0; j < n; j++){
            A[i * n  + j] = (rank * local_rows + i + j + 1) * 0.001;

        }
    }

    
    for(int i = 0; i < local_x; i++){
        x[i] = (rank * local_x + i + 1) * 0.001;
    }
    MPI_Barrier(MPI_COMM_WORLD);
    double start_1d = get_time();
    mult_row_1d(A, x, y, n, rank, p);
    MPI_Barrier(MPI_COMM_WORLD);
    double end_1d = get_time();

    if(!rank){
        printf("ROW_1D p=%d n=%d time=%lf seconds\n", p, n, end_1d - start_1d);
    }

    free(A);
    free(x);
    free(y);

    int dims[2] = {0,0};
    MPI_Dims_create(p, 2, dims);

    int block_rows = n / dims[0];
    int block_cols = n / dims[1];

    A = (double *) malloc(block_rows * block_cols * sizeof(double));
    x = (double*) malloc(block_cols * sizeof(double));
    y = (double*) malloc(block_rows * sizeof(double));


    int row = rank/ dims[1];
    int col = rank % dims[1];

    int row_off = row * block_rows;
    int col_off = col * block_cols;

    for(int i = 0; i < block_rows; i++){
        for(int j = 0; j < block_cols; j++){
            A[i * block_cols + j] = (row_off + i + col_off + j + 1) * 0.001;

        }
    }

    for(int i = 0; i < block_cols; i++){
        x[i] = (col_off + i + 1) * 0.001;
    }
    for(int i = 0 ; i < block_rows; i++){
        y[i] = 0.0;
    }
    MPI_Barrier(MPI_COMM_WORLD);
    double start_2d = get_time();
    mult_row_2d(A, x, y, n , rank, p);
    MPI_Barrier(MPI_COMM_WORLD);
    double end_2d = get_time();

    if(!rank){
        printf("ROW_2D p=%d n=%d time=%lf seconds\n", p, n, end_2d - start_2d);
    }
    free(A);
    free(x);
    free(y);
   

 MPI_Finalize();
    return 0;
}