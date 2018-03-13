#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "../endianess.h"

int endianess = 0;
int PORT_NUMBER = 3433;

int main()
{	
	int server_sockfd, client_sockfd;
	int requestId, answerId;
	float number = 0.0;
	float result = 0.0;

	socklen_t server_len, client_len;
	struct sockaddr_in server_address;
	struct sockaddr_in client_address;

	/*Configuration*/
	server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server_address.sin_port = htons(PORT_NUMBER);
	server_len = sizeof(server_address);
	bind(server_sockfd, (struct sockaddr *) &server_address, server_len);

	/*  Create a connection queue and wait for clients.  */
	listen(server_sockfd, 1);

	checkEndianess(&endianess);

	while (1)
	{
		printf("Server is waiting for request\n");

		client_len = sizeof(client_address);
		client_sockfd = accept(server_sockfd,
			(struct sockaddr *) &client_address,
			&client_len);
	
		read(client_sockfd, &requestId, sizeof(int));
		if (endianess == 0)	SWAP(requestId);

		if (requestId == 1)
		{
			answerId = 1;
			read(client_sockfd, &number, sizeof(int));
			if (endianess == 0) SWAP(number);
	
			if (number >= 1)
			{
				result = sqrt(number);
			}
			else
			{
				result = -1;
			}
			
			if (endianess == 0) SWAP(answerId);
			write(client_sockfd, &answerId, sizeof(int));

			if (endianess == 0) SWAP(result);
			write(client_sockfd, &result, sizeof(float));
			printf("Sent result\n");
		}
		if (requestId == 2)
		{
			answerId = 2;
			
			time_t rawtime;
			struct tm * timeinfo;

			time(&rawtime);
			timeinfo = localtime(&rawtime);
			char* date = asctime(timeinfo);
			int dateLen = htons(strlen(date));
	
			if (endianess == 0)
			{
				SWAP(answerId);
				SWAP(dateLen);

			}
			write(client_sockfd, &answerId, sizeof(int));
			write(client_sockfd, &dateLen, sizeof(int));
			write(client_sockfd, asctime(timeinfo), dateLen);
			printf("Sent date\n");
			printf("%s\n", asctime(timeinfo));
		}
		close(client_sockfd);

	}
}

