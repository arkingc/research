#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>

#define TIMES 20000

const char* path[2] = {"/root/file1","/root/file2"};

int main(int argc,char* argv[])
{
    struct timeval start,end;

    gettimeofday(&start,NULL);

    int idx = 0;
    int i = 0;
    while(++i <= TIMES){
        rename(path[idx],path[1 - idx]);
        idx = 1 - idx;
    }

    gettimeofday(&end,NULL);

    long int time = 1000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000;

    printf("time(ms): %ld\n",time);

    return 0;
}
