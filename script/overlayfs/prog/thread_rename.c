#include <pthread.h>
#include <stdio.h>
#include <stdlib.h> //atoi,malloc
#include <sys/time.h>
#include <unistd.h> //chdir

#define TIMES 20000

const char* pathTemplate[2] = {"merged%d/dir1/file1","merged%d/dir2/file2"};

struct thread_arg{
    int id;
    pthread_barrier_t *barrier;
};

void* thr_fn(void *arg)
{
    struct timeval start,end;
    int idx = 0;
    int i = 0;
    long int time;
    char path[2][50];
    char root[20];

    sprintf(path[0],pathTemplate[0],((struct thread_arg*)arg)->id);
    sprintf(path[1],pathTemplate[1],((struct thread_arg*)arg)->id);

    pthread_barrier_wait( ((struct thread_arg*)arg)->barrier );
    
    gettimeofday(&start,NULL);  //start time
    
    while(++i <= TIMES){
        rename(path[idx],path[1 - idx]);
        idx = 1 - idx;
    }
    
    gettimeofday(&end,NULL);    //end time

    time = 1000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000;
    
    printf("time(ms): %ld\n",time);
    
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
    struct thread_arg* args = (struct thread_arg*)malloc(sizeof(struct thread_arg) * thread_num);
  
    pthread_barrier_init(&barrier,NULL,thread_num);

    for(i = 0;i < thread_num;i++){
        args[i].id = i + 1;
        args[i].barrier = &barrier;
        if(pthread_create(&pid,NULL,thr_fn,args + i) != 0){
            perror("thread create error!\n");
            free(pids);
            free(args);
            return 0;
        }
        pids[i] = pid;
    }

    for(i = 0;i < thread_num;i++)
        pthread_join(pids[i],NULL);

    free(pids);
    free(args);

    return 0;
}
