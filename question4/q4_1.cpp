#include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
using namespace std;
int main(int argc, char ** argv) {
  size_t n = 10; //n= 10, 1000, 10000, and 100000
  int a[n][n];
  int b[n];
  int result[n];
  for (size_t i = 0 ; i < n ; i++) {
    b[i]=3;
  
    for (size_t j = 0 ; j < n ; j++) {
      a[i][j]=2;
    }
  }
  for (size_t i = 0 ; i < n ; i++) {
    result[i]=0;
  }

  
  struct timeval start,end;
  /* YOUR CODE BEGIN */
  gettimeofday(&start,NULL);
  
  for (int i = 0 ; i < n ; i++) {
    for(int j =0; j < n; j++){
      result[i]+=a[i][j] * b[i];
    }
  }
  gettimeofday(&end,NULL);
  double elapsed = (end.tv_sec - start.tv_sec)*1e6 + end.tv_usec-start.tv_usec;
  printf("%f",elapsed);
  printf("n=%zu, time=%f us (%.6f ms)\n",n, elapsed, elapsed / 1000.0);
  printf("n=%zu, time=%f microseconds\n", n, elapsed);

  /* YOUR CODE END */

  return 0;
}
