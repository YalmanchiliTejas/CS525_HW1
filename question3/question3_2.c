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

    // Loop Unrolling:
    for (long r = 0; r < 1000000; r++) {
    sum = 0;
    int i = 0;
    int limit = length - (length % 8);
    for (int i = 0; i < limit; i += 8){
        sum += a[i];
        sum += a[i+1];
        sum += a[i+2];
        sum += a[i+3];
        sum += a[i+4];
        sum += a[i+5];
        sum += a[i+6];
        sum += a[i+7];
    }
    sum += a[limit] + a[limit+ 1] + a[limit+ 2] + a[limit+3];

}
    gettimeofday(&end, NULL);
    seconds  = end.tv_sec  - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;
    total_time = seconds + useconds/1000000.0;

    printf("Time taken unrolled : %f seconds\n", total_time);
    printf("Sum: %ld\n", sum);

    free(a);
    return 0;
}