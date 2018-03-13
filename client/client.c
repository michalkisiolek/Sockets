/*CLIENT*/

/*  Make the necessary includes and set up the variables.  */

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "../endianess.h"

int endianess = 0;

int PORT_NUMBER = 3433;

int main()
{
	socklen_t len;
	struct sockaddr_in address;
	int sockfd, length, choice = 0, requestId = 0, answerId = 0;
	float result = 0.0, number = 0.0;

	//  Create a socket for the client.
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	//  Name the socket, as agreed with the server.
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port = htons(PORT_NUMBER);
	len = sizeof(address);

	//  Now connect our socket to the server's socket.
	result = connect(sockfd, (struct sockaddr *) &address, len);

	checkEndianess(&endianess);

	if (result == -1)
	{
		perror("oops: netclient");
		exit(1);
	}

	printf("1. Get square of number\n");
	printf("2. Get date from server\nWrite your choice: ");
	scanf("%d", &choice);
	requestId = choice;

	if (endianess == 0) SWAP(requestId);
	write(sockfd, &requestId, sizeof(int));

	if (choice == 1)
	{
		printf("Write a number:\n");
		scanf("%f", &number);

		if (endianess == 0)	SWAP(number);
		write(sockfd, &number, sizeof(int));
	}

	
	read(sockfd, &answerId, sizeof(int));
	if (endianess == 0) SWAP(answerId);

	if (answerId == 1)
	{
		read(sockfd, &result, sizeof(float));
		if (endianess == 0) SWAP(result);

		if (result == -1)
		{
			printf("Wrong number!");
		}
		else
		{
			printf("The result is: %f \n", result);
		}
	}
	if (answerId == 2)
	{
		printf("Received date from server.\n");
		read(sockfd, &length, sizeof(int));
		if (endianess == 0)	SWAP(length);

		char *date;
		date = malloc(length*sizeof(char));
		read(sockfd, date, length);
		printf("%s\n", date);
		free(date);
	}

	close(sockfd);
	exit(0);
}

