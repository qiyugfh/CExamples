#include <stdio.h>
#include "mq.h"


int main(int argc, char *argv[]){
	if(initializeClient("fang") == -1){
		return -1;
	}

	
	sendMessage(22, 45, 14, 1234, "hello fanghua1");
	uint64_t tv_sec;
	uint64_t tv_usec;
	uint32_t caplen;
	uint32_t len;

	char *msg = receiveMessage(&tv_sec, &tv_usec, &caplen, &len);
	printf("tv_sec:%lu, tv_usec:%lu, caplen:%ld, len:%ld ,msg: %s\n", tv_sec, tv_usec, caplen, len,  msg);
	shutdownClient();
	return 0;
}
