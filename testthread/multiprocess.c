#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <sys/types.h>
#include <wait.h>
#include <unistd.h>
#include <stdlib.h>


int detect_process(char *process_name)
{
  FILE *fp = NULL;
  char buf[512] = {0};
  char command[128] = {0};
  sprintf(command, "ps -ef | grep -w %s | grep -v grep | wc -l", process_name);
  
  if((fp = popen(command, "r")) != NULL){
    if(fgets(buf, 512, fp) != NULL){
      int count = atoi(buf);
      printf("count: %d\n", count); 
      if(count >= 2){
        pclose(fp);
        printf("the process '%s' is already running, will not start new one\n", process_name);
        return -1;
      }
    }
  
  pclose(fp);
  }
  return 0;
}


void clear_pthread(int signal){
  int status = 0;
  while(waitpid(-1, &status, WNOHANG)){
    printf("process:%ld, sig:%d, code:%d\n", (long)getpid(), status&0xff, (status>>8)&0xff);
    exit(0); 
  }	 	  
}

void process_main(){
  int i = 3000;
  while(i-->0){
    sleep(1);
    printf("%ld is do ..\n", (long)getpid());
  }
}

int shutdown_app = 0;
void sigproc(int sid){
printf("process %ld recive the TERM signal, now exit ...\n", (long)getpid());
exit(0);
}

int main(){
 
 if(detect_process("multiprocess") != 0){
    exit(0);
  }

  signal(SIGTERM, sigproc);

//  signal(SIGCHLD, clear_pthread);
  for(int n=0; n<5; n++){
  pid_t id = fork();
  if(id < 0){
    perror("failure");
    exit(1);
  }else if(id == 0){
     process_main();
     exit(0); 
 }else if(id > 0){      
#if 0
     while(1){
       signal(SIGCHLD, clear_pthread);
     }
#endif
     //exit(0);
   
   }
  }

  return 0;
}
