#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>


#define PORT 9832
#define SERVER_IP "192.168.80.130"

int main()
{
	int server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
	server_addr.sin_port = htons(PORT);
	
	bind(server_sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	listen(server_sockfd, 5);

	char ch;
	int client_sockfd;
	struct sockaddr_in client_addr;
	socklen_t len = sizeof(client_addr);
	while(1)
	{
		printf("server waiting:\n");
		client_sockfd = accept(server_sockfg, (struct sockaddr *)&client_addr, &len);
		read(client_sockfd, &ch, 1);
		printf("get char from client: %c\n", ch);
		++ch;
		write(client_sockfd, &ch, 1);
		close(client_sockfd);
	}
	
	return 0;
}









