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
    // int num_local_rows = n/p;
    

    int quotient = n / p;
    int remainder = n % p;
    
    int num_local_rows = quotient + (remainder > rank ? 1 : 0);
    int local_x = num_local_rows;
    
    // to tell how many elements of the vector x each row contains(to make sure that it's split as evenly as possible)

    int * element_count = (int *) malloc(p * sizeof(int));


    // this will tell ALl teh indexes of teh starting indexes of the 
    int * chunk_index = (int *) malloc(p * sizeof(int));



    chunk_index[0] = 0;
    for(int i = 0; i < p;i++){
        if(i != 0){
            chunk_index[i] = chunk_index[i - 1] + element_count[i - 1];
        }
        element_count[i] = quotient +  (remainder > i ? 1 : 0);

    }





    double *x_res = (double *) malloc(n * sizeof(double)); 

    // All to all broadcast so that each row has the entire vector
    // MPI_Allgather(x, local_x, MPI_DOUBLE, x_res, local_x, MPI_DOUBLE, MPI_COMM_WORLD);
    MPI_Allgatherv(x, local_x,  MPI_DOUBLE, x_res, element_count, chunk_index,MPI_DOUBLE, MPI_COMM_WORLD);

    // Summing it up per row
    for(int i =  0; i < num_local_rows; i++){
        y[i] = 0.0;
        for(int j = 0; j < n; j++){
            y[i] += A[i * n +  j] * x_res[j];
        }
    }
    free(x_res);
    free(element_count);
    free(chunk_index);
}


void mult_row_2d(double *A, double*x, double* y, int n, int rank, int p){


    // stores the dimenstions of the the processor grid, in which the processors have been arranged into
    int grid_dimensions[2] = {0,0};

    // coordinates of the process in the grid

    int coords[2];

    // wraparounds
    int wraparounds[2] = {0,0};

    // MpI communicators across the grid, rows and columns
    MPI_Comm across_grid, row_comms, col_comms;

    MPI_Dims_create(p, 2, grid_dimensions);
    MPI_Cart_create(MPI_COMM_WORLD, 2, grid_dimensions, wraparounds, 0, &across_grid);

    int grid_rank;
    MPI_Comm_rank(across_grid, &grid_rank);


    MPI_Cart_coords(across_grid, grid_rank, 2, coords);

    int columns  = coords[1];
    int rows = coords[0];

    MPI_Comm_split(across_grid, rows, columns, &row_comms);
    MPI_Comm_split(across_grid, columns, rows , &col_comms);

    int process_rows = grid_dimensions[0];
    int process_cols = grid_dimensions[1];


    int row_quotient = n / process_rows;
    int row_remainder = n % process_rows;

    int col_quotient = n / process_cols;
    int col_remainder = n % process_cols;


    int * row_count = (int *) malloc(process_rows * sizeof(int));

    // this will tell teh All
    int * row_chunk_index = (int *) malloc(process_rows* sizeof(int));

    int * col_count =  (int *) malloc(process_cols * sizeof(int));

    int * col_chunk_index = (int *) malloc(process_cols* sizeof(int));

    row_chunk_index[0] = 0;
    col_chunk_index[0] = 0;

    
    for(int i = 0; i < process_rows;i++){
        if(i != 0){
            row_chunk_index[i] = row_chunk_index[i - 1] + row_count[i - 1];
        }
        row_count[i] = row_quotient +  (row_remainder > i ? 1 : 0);

    }

    for(int i = 0; i < process_cols;i++){
        if(i != 0){
            col_chunk_index[i] = col_chunk_index[i - 1] + col_count[i - 1];
        }
        col_count[i] = col_quotient +  (col_remainder > i ? 1 : 0);

    }
     
    int local_rows = row_count[rows];
    int local_cols = col_count[columns];

    double *x_column = (double *) malloc(local_cols * sizeof(double));
    double *row_vals = (double * ) malloc(local_rows * sizeof(double));

    for(int i = 0; i < local_cols; i++){
        x_column[i] = 0.0;
    }
    // Main deviation from the slides method, I am doing this because I couldn't figure out a way to handle the slide's algorithm is p is not a proper square root and we end up with a rectangular grid
    // instead of a square grid ( From this piazza post:https://piazza.com/class/mkf0yb9nf9u6o6/post/89 ). so In order to account for that case, what I did was to let first row process of each process column own that columns vector block and then broadcast it downward
    // I found that it would keep the overall communication structure where you broadcast it column wise and then do a row-wise rediction while generalizing to rectangular grids
    if (rows == 0){
        for(int i = 0; i < local_cols; i++){
            x_column[i] = x[i];
        }
    }

    MPI_Bcast(x_column, local_cols, MPI_DOUBLE, 0, col_comms);

    for(int i = 0; i < local_rows; i++){
        row_vals[i] = 0.0;

        for(int j = 0; j < local_cols; j++){
            int index = i * local_cols + j;
            row_vals[i] += A[index] * x_column[j];
        }
    }

    MPI_Reduce(row_vals, y, local_rows, MPI_DOUBLE, MPI_SUM, 0, row_comms);

    free(x_column);
    free(row_vals);
    free(col_count);
    free(col_chunk_index);
    free(row_count);
    free(row_chunk_index);

    MPI_Comm_free(&across_grid);
    MPI_Comm_free(&row_comms);
    MPI_Comm_free(&col_comms);

   

    
}
int main(int argc, char *argv[]){

    int rank, p;
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    for(int exp = 9; exp <= 12; exp++){
        int n = 1 << exp;

    


    int quotient = n / p;
    int remainder = n % p;

    int local_rows = quotient + (remainder  > rank ? 1 : 0);
    int local_x = local_rows;

    double *A = (double*)malloc(local_rows * n * sizeof(double));
    double *x = (double*) malloc(local_x * sizeof(double));
    double *y = (double*) malloc(local_rows * sizeof(double));

    srand(100 + rank);//seed for random matrix and vector generation
    for(int i = 0; i < local_rows; i++){
        for(int j = 0; j < n; j++){
            A[i * n  + j] = (double)(rand() % 10 + 1);

        }
    }

    
    for(int i = 0; i < local_x; i++){
        x[i] = (double)(rand() % 10 + 1);
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
    int wraparound[2] = {0,0};
    int coords[2];
    MPI_Comm grid_comm;

    MPI_Dims_create(p, 2, dims);
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, wraparound,0, &grid_comm);

    int grid_rank;
    MPI_Comm_rank(grid_comm, &grid_rank);
    MPI_Cart_coords(grid_comm, grid_rank, 2, coords);

    int col = coords[1];
    int row = coords[0];

    int process_rows = dims[0];
    int process_cols = dims[1];

    int row_quotient = n / process_rows;
    int row_remainder = n % process_rows;

    int col_quotient = n / process_cols;
    int col_remainder = n % process_cols;


    int * row_count = (int *) malloc(process_rows * sizeof(int));

    // this will tell teh All
    int * row_chunk_index = (int *) malloc(process_rows* sizeof(int));

    int * col_count =  (int *) malloc(process_cols * sizeof(int));

    int * col_chunk_index = (int *) malloc(process_cols* sizeof(int));

    row_chunk_index[0] = 0;
    col_chunk_index[0] = 0;

    
    for(int i = 0; i < process_rows;i++){
        if(i != 0){
            row_chunk_index[i] = row_chunk_index[i - 1] + row_count[i - 1];
        }
        row_count[i] = row_quotient +  (row_remainder > i ? 1 : 0);

    }

    for(int i = 0; i < process_cols;i++){
        if(i != 0){
            col_chunk_index[i] = col_chunk_index[i - 1] + col_count[i - 1];
        }
        col_count[i] = col_quotient +  (col_remainder > i ? 1 : 0);

    }
     
    local_rows = row_count[row];
    int local_cols = col_count[col];


    

    A = (double *) malloc(local_rows * local_cols * sizeof(double));
    x = (double*) malloc(local_cols* sizeof(double));
    y = (double*) malloc(local_rows * sizeof(double));


    srand(100 + rank);

    for(int i = 0; i < local_rows; i++){
        for(int j = 0; j < local_cols; j++){
            A[i * local_cols + j] = (double)(rand() % 10 + 1);

        }
    }

    if(row == 0){
        for(int i = 0; i < local_cols; i++){
            x[i] = (double)(rand() % 10 + 1);
        }
    }
    for(int i = 0 ; i < local_rows; i++){
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
    free(row_count);
    free(row_chunk_index);
    free(col_count);
    free(col_chunk_index);
    MPI_Comm_free(&grid_comm);
   
}
 MPI_Finalize();
    return 0;
}