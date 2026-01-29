#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>



float  a[10000000];


int main(int argc, char ** argv){



   float sum,  sum1, sum2, sum3, sum4;

    int s = 0;
    int t = 1;
    int u = 2;
    int v = 3;

    sum = sum1 = sum2 = sum3 = sum4 = 0.0;
    for(int i = 0; i < 10000000; i++){
        a[i] = 1.0;
    }


    struct timeval start, end;
    long seconds, useconds;
    double total_time;



    gettimeofday(&start, NULL);


    for(int i = 0; i < 10000000; i+= 4){
        sum1 = a[s];
        sum2 = a[t];
        sum3 = a[u];
        sum4 = a[v];
        s+=4;
        t+=4;
        u+=4;
        v+=4;

        sum+= (sum1+ sum2 + sum3+sum4);
    }
    gettimeofday(&end, NULL);
    seconds  = end.tv_sec  - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;
    total_time = seconds + useconds/1000000.0;
    printf("Time taken for segment2: %f seconds\n", total_time);
    printf("The total summation value is: %f\n", sum);
    
}
