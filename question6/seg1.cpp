#include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
using namespace std;
int main(int argc, char ** argv) {
  float* a = (float *)malloc(1e6 * sizeof(float));
  for (size_t i = 0 ; i < 1e6 ; i++) {
    a[i]=2;
  }
  
  struct timeval start,end;
  /* YOUR CODE BEGIN */
  gettimeofday(&start,NULL);
  
  for (int i = 0 ; i < 1e6 ; i++) {
    a[i]=a[i] / 2.0;
  }
  gettimeofday(&end,NULL);
  double elapsed = (end.tv_sec - start.tv_sec)*1e6 + end.tv_usec-start.tv_usec;
  printf("%f",elapsed);
  printf("n=%f, time=%f us (%.6f ms)\n",1e6, elapsed, elapsed / 1000.0);
  printf("n=%f, time=%f microseconds\n", 1e6, elapsed);

  /* YOUR CODE END */

  free(a);
  
  //free(result);
  return 0;
}
