#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/syscall.h>



const int thread_num = 3;
int x = 0;

static void threadFunc(){
  int x = 0;
  while(x < 10000){
    sleep(1);
    x++;
    printf("thread %u : %d\n", (unsigned int) pthread_self(), x);
  }
}

#if 1
int main(){
  printf("start ...\n");
 

  pthread_t tid[thread_num];
  
  for(int i=0; i<thread_num; i++){
    int ret = pthread_create(&tid[i], NULL, (void*)threadFunc, NULL);
    if(ret != 0){
      printf("create thread %d fail\n", i);
    }else{
      printf("create thread %d success, %u\n", i, tid[i]);
    }
  }

  
  void *status[thread_num];
  for(int i=0; i<thread_num; i++){
    int ret = pthread_join(tid[i], &status[i]);
    if(ret != 0){
      printf("error on join thread %d\n", i);		  
    }
  }

printf("exit ...\n");
return 0;
}

#endif

#if 0
int main(){
  printf("start ...\n");
  pthread_t tid[thread_num];
  void *status[thread_num];

  for(int i=0; i<thread_num; i++){
    int ret = pthread_create(&tid[i], NULL, (void*)threadFunc, NULL);
    if(ret != 0){
      printf("create thread %d fail\n", i);
    }else{
      printf("create thread %d success, %u\n", i, tid[i]);
    }
    
   ret = pthread_join(tid[i], &status[i]);
   if(ret != 0){
     printf("error on join thread %d\n", i);
   }
  }


//  printf("exit ...");
  //return 0;
}
#endif



