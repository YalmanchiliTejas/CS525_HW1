#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int main(int argc, char ** argv){



    float a[1000000], one_over_two;

    for(int i = 0; i < 1000000; i++){
        a[i] = 1.0 * i;
    }


    struct timeval start, end;
    long seconds, useconds;
    double total_time;



    gettimeofday(&start, NULL);

    one_over_two = 1.0 / 2.0;

    for(int i = 0; i < 1000000; i++){
        a[i] = a[i] / 2.0;
    }
    gettimeofday(&end, NULL);
    seconds  = end.tv_sec  - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;
    total_time = seconds + useconds/1000000.0;
    printf("Time taken for one_over_two: %f seconds\n", total_time);
    
}