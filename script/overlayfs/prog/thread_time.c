#include <pthread.h>
#include <stdio.h>
#include <stdlib.h> //atoi,malloc
#include <sys/time.h>

void* thr_fn(void *arg)
{
    struct timeval start;
    pthread_barrier_wait((pthread_barrier_t*)arg);
    gettimeofday(&start,NULL);
    printf("current time(ms): %ld\n",1000 * start.tv_sec + start.tv_usec / 1000);
    pthread_exit((void*)0);
}

int main(int argc,char* argv[])
{
    if(argc != 2){
        perror("usage: prog thread_num\n");
        return 0;
    }

    int thread_num = atoi(argv[1]);
    int i;
    pthread_t pid;
    pthread_t *pids = (pthread_t*)malloc(sizeof(pthread_t) * thread_num);
    pthread_barrier_t barrier;
   
    pthread_barrier_init(&barrier,NULL,thread_num);

    for(i = 0;i < thread_num;i++){
        if(pthread_create(&pid,NULL,thr_fn,&barrier) != 0){
            perror("thread create error!\n");
            free(pids);
            return 0;
        }
        pids[i] = pid;
    }

    for(i = 0;i < thread_num;i++)
        pthread_join(pids[i],NULL);

    free(pids);

    return 0;
}
