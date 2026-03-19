#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>


double get_time(){
  struct timeval t;
  gettimeofday(&t, NULL);
  return t.tv_sec + t.tv_usec * 1e-6;

}

int main(int argc, char *argv[]){

  int rank, processors, half, partner, i , j;

  int message_sizes[8] = {1024, 2048, 4096, 8192, 16384, 32768, 65536, 102400};

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &processors);

  if(processors % 2 != 0){
    MPI_Finalize();
    return 0;
  }
  half = processors / 2;
  if (rank < half){
    partner = rank + half;
  } else {
    partner = rank - half;
  }

  for(int i = 0; i < 8; i++){
    int message_size = message_sizes[i];
    char *message = (char *)malloc(message_size * sizeof(char));
    double start_time, end_time, local_time, max_time;

    for(int j = 0; j < message_size; j++){
      message[j] = 'c';
    }

    MPI_Barrier(MPI_COMM_WORLD);
    start_time = get_time();
    // NUM Pingpongs = 10000
    for (int j = 0; j < 10000; j++){
      if (rank < half){
        MPI_Send(message, message_size, MPI_CHAR, partner, 0, MPI_COMM_WORLD);
        MPI_Recv(message, message_size, MPI_CHAR, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      } else {
        MPI_Recv(message, message_size, MPI_CHAR, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Send(message, message_size, MPI_CHAR, partner, 0, MPI_COMM_WORLD);
      }
    }
    MPI_Barrier(MPI_COMM_WORLD);
    end_time = get_time();
    local_time = (end_time - start_time);
    MPI_Reduce(&local_time, &max_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0){
      double one_way_latency = max_time / (2.0  * 10000);
      printf("Message Size: %d bytes, num_procs: %d, One-way Latency: %f seconds\n", message_size, processors, one_way_latency * 1e6);
    }
    free(message);

    
  }
  MPI_Finalize();
  return 0;
}

