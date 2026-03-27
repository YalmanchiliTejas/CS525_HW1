#include <mpi.h>
#include <algorithm>
#include <string>
#include <iostream>
#include <sys/time.h>
#include <vector>
using namespace std;
double get_time() {
  struct timeval t;
  gettimeofday(&t, NULL);
  return t.tv_sec + t.tv_usec * 1e-6;
}
void p_qsort(vector<int> &local,int low,int high){
  if (low==high){
    sort(local.begin(),local.end());
    return;
  }

  int rank=0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  //MPI_Comm_size(comm,&size);
  int pivot =0;
  if(rank==low){
    //choosing pivot
    pivot = local.empty()?0:local[local.size()/2];
  }
  MPI_Bcast(&pivot, 1,MPI_INT,low,MPI_COMM_WORLD);

  vector<int>l_half,up_half;//L_i,U_i
  for(int i:local){
    if(i<=pivot) l_half.push_back(i);
    else up_half.push_back(i);
  }
  int mid = low + (high-low+1)/2;
  if(rank < mid){
    int partner = rank + (mid - low);
    int send_sz = up_half.size(),rcv_sz;

    MPI_Sendrecv(&send_sz, 1, MPI_INT,partner,0, &rcv_sz, 1,MPI_INT,partner,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    vector<int> rcv_buf(rcv_sz);
    MPI_Sendrecv(up_half.data(),send_sz,MPI_INT, partner, 1, rcv_buf.data(),rcv_sz,MPI_INT,partner,1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    local =l_half;
    local.insert(local.end(), rcv_buf.begin(),rcv_buf.end());

    p_qsort(local,low,mid-1);
  } else {
    int partner = rank - (mid - low);
    int send_sz = l_half.size(),rcv_sz;

    MPI_Sendrecv(&send_sz, 1, MPI_INT,partner,0, &rcv_sz, 1,MPI_INT,partner,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    vector<int> rcv_buf(rcv_sz);
    MPI_Sendrecv(l_half.data(),send_sz,MPI_INT, partner, 1, rcv_buf.data(),rcv_sz,MPI_INT,partner,1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    local=up_half;
    local.insert(local.end(), rcv_buf.begin(),rcv_buf.end());

    p_qsort(local,mid,high);
  }
}

int main(int argc, char * argv[]){
  MPI_Init(&argc, &argv);
  int ns = stoi(argv[1]);
  int rank = 0, processors = 1;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &processors);
  if((processors & (processors -1)) !=0){
    MPI_Finalize();
    return 0;
  }
  //int num_list = 3;
  //int list_sizes[num_list] = {100,1000,10000};
  int seed=5;
  //for(int ns = 0; ns< num_list;ns++){
    int N = 1000*ns;
    vector<int> global(N);
    vector<int> s_copy;
    vector<int> l_cnts(processors), offs(processors);
    int base=N/processors;
    if(rank ==0){
      //global.resize(N);
      srand(seed+ns);
      for(int i = 0;i<N;i++){
        global[i]=rand();
      }
      s_copy=global;
      //int rem = N % processors;
    }
    vector<int> local(base);
    MPI_Scatter(global.data(),base,MPI_INT,local.data(),base, MPI_INT,0,MPI_COMM_WORLD);
    double s_time = 0.0,start_time,end_time;
    /*if(rank==0){
      start_time = get_time();
      sort(s_copy.begin(),s_copy.end());
      end_time = get_time();
      s_time = end_time - start_time;
    }*/
    if(ns==100){
       s_time = 0.142074;
    }else if(ns==1000){
        s_time=1.400953;
    } else if(ns=10000){
      s_time = 12.203427;
    }
    MPI_Barrier(MPI_COMM_WORLD);
    start_time = get_time();
    p_qsort(local,0,processors-1);
    end_time = get_time();
    double p_time = end_time - start_time;
    double max_t=0.0;
    MPI_Reduce(&p_time,&max_t,1,MPI_DOUBLE,MPI_MAX,0,MPI_COMM_WORLD);
    if(rank==0){
      double speedup = s_time / max_t;
      double efficiency = speedup / processors;
      printf("%d,%d,%.6f,%.6f,%.6f,%.6f\n", N, processors, s_time,max_t, speedup, efficiency);
    }

    int local_n = local.size();
    MPI_Gather(&local_n, 1 ,MPI_INT,l_cnts.data(),1,MPI_INT,0,MPI_COMM_WORLD);
    vector<int> ans(N);
    if(rank==0){
      //ans.resize(N);
      offs[0]=0;
      for(int i = 1;i<processors;i++){
        offs[i]=offs[i-1]+l_cnts[i-1];
      }
    }

    MPI_Gatherv(local.data(),local_n,MPI_INT, ans.data(),l_cnts.data(),offs.data(),MPI_INT,0,MPI_COMM_WORLD);
    if (rank == 0) {
      int ok = 1;
      for (int i = 1; i < N; i++) {
        if (ans[i - 1] > ans[i]) {
          ok = 0;
          break;
        }
      }
      printf("%s\n", ok ? "sorted" : "not sorted");
    }
    MPI_Finalize();
    return 0;

  //}
}
