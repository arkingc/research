#include <sys/time.h>
#include <stdio.h>

int main()
{
    struct timeval start,end;
    gettimeofday(&start,NULL);
    sleep(1);
    gettimeofday(&end,NULL);
    printf("current time: %ld ms\n",1000 * start.tv_sec + start.tv_usec / 1000);
    long int timeuse = 1000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000;
    printf("time: %ld ms\n",timeuse);
    return 0;
}
