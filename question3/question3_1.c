#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int main(int arc, char ** argv){

    
    size_t length = 100;
    long sum = 0;

    long* a = (long*) malloc (length * sizeof(long));
    for (size_t i = 0; i < length; i++){
        a[i] = i + 1;

    }

    struct timeval start, end;
    long seconds, useconds;
    double total_time;

    gettimeofday(&start, NULL);
    for (long r = 0; r < 1000000; r++) {
    sum = 0;
    for (int i = 0; i < length; i++){
        sum += a[i];
    }
}
    gettimeofday(&end, NULL);
    seconds  = end.tv_sec  - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;
    total_time = seconds + useconds/1000000.0;

    printf("Time taken regular: %f seconds\n", total_time);
    printf("Sum: %ld\n", sum);

    free(a);
    return 0;
}