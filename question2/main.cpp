#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int main(int argc, char ** argv) {
  size_t length = 10000000,skip=1;
  long sum = 0;
  long* a = (long *)malloc(skip*length * sizeof(long));
  //long a[skip * length];
  for (size_t i = 0 ; i < skip*length ; i++) {
    a[i] = 2;
  }
  struct timeval start,end;
  /* YOUR CODE BEGIN */
  gettimeofday(&start,NULL);
  for (int i = 0 ; i < skip*length ; i++) {
    sum += a[i];
  }
  gettimeofday(&end,NULL);
  double elapsed = (end.tv_sec - start.tv_sec)*1e6 + end.tv_usec-start.tv_usec;
  printf("%f",elapsed);
  /* YOUR CODE END */

  //free(a);
  return 0;
}
