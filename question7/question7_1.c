#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>


float a[10000000];
int main(int argc, char ** argv){



    float  sum;
    sum = 0.0;
    for(int i = 0; i < 10000000; i++){
        a[i] =  1.0;
    }


    struct timeval start, end;
    long seconds, useconds;
    double total_time;

    gettimeofday(&start, NULL);

    for(int i = 0; i < 10000000; i++){
        sum += a[i];
    }
    gettimeofday(&end, NULL);
    seconds  = end.tv_sec  - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;
    total_time = seconds + useconds/1000000.0;
    printf("Time taken for segment1: %f seconds\n", total_time);
    printf("the value of the summation is: %f\n", sum);
}
