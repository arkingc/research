#include <fcntl.h>
#include <stdio.h>
#include <sys/time.h>

const char* path = "/root/cowfile";

int main()
{
    struct timeval start,end;

    gettimeofday(&start,NULL);
    
    int fd = open(path,O_WRONLY | O_APPEND);
    if(fd < 0){
        perror("open error! ");
        return 0;
    }
    char c = '1';
    write(fd,&c,1);
    
    gettimeofday(&end,NULL);

    long int time = 1000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000;

    printf("time(ms): %ld\n",time);
    
    return 0;
}
