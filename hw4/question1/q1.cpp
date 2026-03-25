#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

double get_time(){
  struct timeval t;
  gettimeofday(&t, NULL);
  return t.tv_sec +t.tv_usec*1e-6;
}

int main(int argc, char * argv[]){
  int num_pingpongs = 1000;
  //int num_sizes = 8;
  //int message_sizes[num_sizes] = {1024,2048,4096, 8192,16384, 32768, 65536, 102400};

  int rank, processors;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &processors);
  if(processors < 2){
    MPI_Finalize();
    return 0;
  }
  for(int i = 1; i<=100;i++){
    int message_size = 1000*i;
    char * message = (char *) malloc((size_t) message_size);
    for(int j = 0; j<message_size;j++){
      message[j]='a';
    }
    //message[j]='\0';
    MPI_Barrier(MPI_COMM_WORLD);
    double start_time =0.0, end_time =0.0;
    //for (int j = 0; j<num_pingpongs;j++){
      if(rank ==0){
        start_time = get_time();
        for (int j = 0; j<num_pingpongs;j++){
          MPI_Send(message,message_size, MPI_CHAR,1,0,MPI_COMM_WORLD);
          MPI_Recv(message,message_size,MPI_CHAR,1,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        }
        end_time = get_time();

      } else if(rank==1){
        for (int j = 0; j<num_pingpongs;j++){

          MPI_Recv(message,message_size, MPI_CHAR,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
          MPI_Send(message,message_size,MPI_CHAR,0,0,MPI_COMM_WORLD);
        }
      } 
    MPI_Barrier(MPI_COMM_WORLD);
    
    if(rank==0){
      double total_rtt = end_time - start_time;
      double avg_rtt = total_rtt / (double) num_pingpongs;
      double one_way = avg_rtt /2.0;
      double one_way_us = one_way * 1e6;
      int words = message_size / 8;
      printf("%d,%d,%.6f\n", message_size, words, one_way_us);
    }
    free(message);
  }
  MPI_Finalize();
  return 0;
}
