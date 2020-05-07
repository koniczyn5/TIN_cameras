// Gate
// Autor: Konrad Meysztowicz-Wiśniewski
#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string>
#include <string.h>
#include "message.cpp"

#define BUFFER_LEN 4096

void error(const char *msg)
{
	perror(msg);
	exit(1);
}

void getValue(int *dest)
{
    std::string inputString;

    getline(std::cin, inputString);

    *dest = stoi(inputString);
}

void getValue(float *dest)
{
    std::string inputString;

    getline(std::cin, inputString);

    *dest = stof(inputString);
}

int main (int argc, char *argv[])
{
	int socketFd;
	char buffer[BUFFER_LEN];
	struct sockaddr_in serverAddr;
	struct hostent *serverHost;
	socklen_t serverAddrLen;
	std::string msgText;
	Message msg(0);

	if(argc < 3)
	{
		fprintf(stderr, "Use: \"%s hostname port\"", argv[0]);
		exit(1);
	}
	socketFd = socket(AF_INET, SOCK_DGRAM, 0);

	if(socketFd < 0)
		error("Failed to open socket");

	serverHost = gethostbyname(argv[1]);
	if(serverHost == NULL)
		error("No such host");

	bzero((char *) &serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	bcopy((char *)serverHost->h_addr, (char *)&serverAddr.sin_addr.s_addr, serverHost->h_length);
	serverAddr.sin_port = htons(atoi(argv[2]));
	serverAddrLen = sizeof(serverAddr);

	if(argc >= 3 && argv[2] == "-i")
	{
		bzero(buffer, sizeof(buffer));
		if(sendto(socketFd, buffer, sizeof(buffer), 0, (struct sockaddr *) &serverAddr, serverAddrLen) < 0)
			error("Failed sendto");

		bzero(buffer, sizeof(buffer));
		msgText = "polaczono\n";
		strcpy(buffer, msgText.c_str());

		if(sendto(socketFd, buffer, sizeof(buffer), 0, (struct sockaddr *) &serverAddr, serverAddrLen) < 0)
				error("Failed sendto");
	}

	bzero(buffer, sizeof(buffer));

	int *resHorizontal = (int*)(&buffer[1]);
	int *resVertical = (int*)(&buffer[5]);
    float *focalLength = (float*)(&buffer[9]);
    float *interval = (float*)(&buffer[13]);
    float *jitter = (float*)(&buffer[17]);

    int resInt;

	printf("Enter camera parameters:\n");

	printf(" horizontal resolution (integer): ");
	getValue(resHorizontal);
	printf(" vertical resolution (integer): ");
	getValue(resVertical);
	printf(" focal length (float): ");
	getValue(focalLength);
	printf(" interval (float): ");
	getValue(interval);
	printf(" jitter (float): ");
	getValue(jitter);

	msg.set_id(CONF_REQ);
	msg.set_msg(buffer);
	strcpy(buffer, msg.get_code());

	if(sendto(socketFd, buffer, sizeof(buffer), 0, (struct sockaddr *) &serverAddr, serverAddrLen) < 0)
				error("Failed sendto");

	bzero(buffer, sizeof(buffer));

	if(recvfrom(socketFd, buffer, sizeof(buffer), 0, (struct sockaddr *) &serverAddr, &serverAddrLen) < 0)
		error("Failed recvfrom");

	if(buffer[0] == CONF_ACK)
		printf("Configuration successful\n");
	else
		printf("Configuration failed\n");

	bzero(buffer, sizeof(buffer));
	msg.set_id(TEST_REQ);
	msg.set_msg(buffer);
	strcpy(buffer, msg.get_code());

	if(sendto(socketFd, buffer, sizeof(buffer), 0, (struct sockaddr *) &serverAddr, serverAddrLen) < 0)
        error("Failed sendto");

    bzero(buffer, sizeof(buffer));

    if(recvfrom(socketFd, buffer, sizeof(buffer), 0, (struct sockaddr *) &serverAddr, &serverAddrLen) < 0)
		error("Failed recvfrom");

    if(buffer[0] == TEST_ACK)
        printf("Test successful\n");
    else
        printf("Test failed\n");

	shutdown(socketFd, SHUT_RDWR );
	return 0;
}


