#define _GNU_SOURCE             /* See feature_test_macros(7) */
#include <sched.h>  
#include <unistd.h>  
#include <sys/types.h>  
#include<string.h>  
#include <stdio.h>  
#include <errno.h>  
  
  
int set_cpu(int i)  
{  
    cpu_set_t mask;  
    CPU_ZERO(&mask);  
  
    CPU_SET(i,&mask);  
  
    if(-1 == sched_setaffinity(getpid() ,sizeof(mask),&mask))  
    {  
        printf("set affinity failed, process %u, i = %d\n", getpid(), i);
        return -1;  
    }
    printf("process %u, i = %d\n", getpid(), i);
  
    return 0;  
}
  
void *fun(void *i)  
{  
    if(set_cpu(*(int *)i))  
    {  
        printf("set cpu erro\n");  
    }  
    long long a = 0;  
    while(a <10)  
    {  
        sleep(1);
        a++;  
    }  
    return NULL;  
}  
  
int main (int argc, char * argv[]) {  
    int i;  
    int cpu_nums = sysconf(_SC_NPROCESSORS_CONF);  
  
    printf("cpu_numbs = %d\n", cpu_nums);  
     
    int tmp[10];  
  
    for(i = 0; i < 10; ++i)  
    {  
        tmp[i] = i%cpu_nums;  
        pid_t pid = fork();
        if(pid < 0){
          printf("fork error %d\n", i);
        }else if(pid == 0){
          fun(&tmp[i]);
          return 0;
        }

                
    }  
}  
