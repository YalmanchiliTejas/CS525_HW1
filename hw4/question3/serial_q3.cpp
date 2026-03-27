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

int main(int argc, char * argv[]){
    vector<int> ns_list = {100,1000,10000};
    for(int ns : ns_list){
        int N = 1000*ns;
        vector<int> global;
        global.resize(N);
        srand(5+ns);
        for(int i = 0;i<N;i++){
            global[i]=rand();
        }
        double s_time = 0.0,start_time,end_time;
        start_time = get_time();
        sort(global.begin(),global.end());
        end_time = get_time();
        s_time = end_time - start_time;
        printf("%d,%.6f\n", N,s_time);
    }
    return 0;
}
