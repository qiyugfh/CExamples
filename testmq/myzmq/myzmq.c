#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern "C"{


#include <zmq.h>
#include <zmq_utils.h>
}
#pragma comment(lib,"libzmq_d.lib")


int main(int argc, char* argv[])
{
void * context = NULL;
void * socket = NULL;
int rc = 0;
uint8_t id[1024] = {0};
size_t id_size = 1024;


   // 初始化环境   
context = zmq_ctx_new();
if (!context)
{
	printf("初始化环境失败!\n");
	goto lbl_end;
}


// 创建套接字

socket = zmq_socket(context, ZMQ_STREAM);
if (!socket)
{
	printf("创建套接字失败!");
	goto lbl_end;
}
//  绑定地址
rc = zmq_bind(socket, "tcp://*:5050");
if (rc != 0)
{
	printf("绑定地址失败了");
	goto lbl_end;
}


while (1)
{
// 接受数据
id_size = zmq_recv(socket, id, 1024, 0);
if (id_size <= 0)
	goto lbl_end;
printf("%s\n", id);
char http_response[] =
"HTTP/1.0 200 OK\r\n"
"Content-Type: text/plain\r\n"
"\r\n"
"真是个开心的故事.";
zmq_send(socket, id, id_size, ZMQ_SNDMORE);
zmq_send(socket, http_response, strlen(http_response), ZMQ_SNDMORE);
zmq_send(socket, id, id_size, ZMQ_SNDMORE);
zmq_send(socket, 0, 0, ZMQ_SNDMORE);

}


lbl_end:
zmq_close(socket);
zmq_ctx_destroy(context);
system("pause");
return 0;

}

