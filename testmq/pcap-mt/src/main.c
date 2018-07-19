#include <stdio.h>
#include <pcap/pcap.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <signal.h>
#include "mq.h"


pcap_t * device = NULL;
#define thread_num 2


void getPacket(u_char *arg, const struct pcap_pkthdr *pkthdr, const u_char *packet){
        int thread_id = *((int *)arg);
        printf("thread id: %d\n", thread_id);
        printf("Packet length: %d\n", pkthdr->len);
        printf("Number of bytes: %d\n", pkthdr->caplen);
        printf("Recieved time: %s\n", ctime((const time_t *)&pkthdr->ts.tv_sec));

//	sendMessage(pkthdr->ts.tv_sec, pkthdr->ts.tv_usec, pkthdr->caplen, pkthdr->len, packet);
}

void getPacketThreadFunc(void * _thread_id){
        int thread_id = (int)_thread_id;
        pcap_loop(device, -1, getPacket, (u_char*)&thread_id);

}

void processPacketThreadFunc(void *_thread_id){
	int thread_id = (int)_thread_id;
        uint64_t tv_sec;
        uint64_t tv_usec;
        uint32_t caplen;
        uint32_t len;
	while(1){
  //      	char *msg = receiveMessage(&tv_sec, &tv_usec, &caplen, &len);
        	printf("thread id: %d, beign process packet: tv_sec:%lu, tv_usec:%lu, caplen:%ld, len:%ld\n", 
			thread_id, tv_sec, tv_usec, caplen, len);
		usleep(5000);
	}
}


void process_terminate(){
 
	if(device != NULL){ 
          pcap_close(device);
	}
        #if 1
        shutdownClient();
        #endif
	exit(0);
}

int main(int argc, char *argv[]){
	int ret = 0;
	int threadId[thread_num] = {0, 1};
	pthread_t tid[thread_num];
	void *thd_res[thread_num];

	signal(SIGTERM, process_terminate);
	signal(SIGINT, process_terminate);
        
	#if 1
	if(initializeClient("fanghua") == -1){
		return -1;
	}
	#endif

 	char errBuf[PCAP_ERRBUF_SIZE], *devStr;
        devStr = pcap_lookupdev(errBuf);
        if(devStr){
                printf("success: device: %s\n", devStr);
        }else{
                printf("error: %s\n", errBuf);
                return -1;
        }

        device = pcap_open_live(devStr, 65535, 1, 0, errBuf);  
        if(!device){
                printf("error: pcap_open_live(): %s\n", errBuf);
                return -1;
        }
        
        struct bpf_program filter;
        // dst port 80
        #if 1
	if(pcap_compile(device, &filter, "", 1, 0) != 0){
	   printf("compile filter failed, %s\n", pcap_geterr(device));
	}
        
	if(pcap_setfilter(device, &filter) != 0){
           printf("install filter failed, %s\n", pcap_geterr(device));
	}
	#else
        pcap_compile(device, &filter, "http", 1, 0);
        pcap_setfilter(device, &filter);
	#endif

       
	// create first thread to capture packet
	ret = pthread_create(&tid[0], NULL, getPacketThreadFunc, (void *)threadId[0]);
	if(ret != 0){
		printf("create thread capture packet thread fail\n");
		return -1;
	}
	
 	if(thread_num  > 1){
	// create second thread to process packet
	ret = pthread_create(&tid[1], NULL, processPacketThreadFunc, (void *)threadId[1]);
                if(ret != 0){
                printf("create thread process packet thread fail\n");
                return -1;
        }
	}

	for(int i=0; i<thread_num; i++){
		ret = pthread_join(tid[i], &thd_res[i]);
		if(ret != 0){
			printf("error on join thread %d\n", i);	
			return -1;
		}
	}	
#if 1
        pcap_close(device);
	#if 1
        shutdownClient();
	#endif
#endif
	return 0;
}
