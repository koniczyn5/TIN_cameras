// Bramka
// Autor: Konrad Meysztowicz-Wiśniewski
// Edytowane do prezentacji przez: Adam Bieniek
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <string.h>
#include "message.cpp"

#define BUFFER_LEN 4096

void error(const char *msg)
{
	perror(msg);
	exit(1);
}

int main (int argc, char *argv[])
{
	int socketFd;
	char buffer[BUFFER_LEN];
	struct sockaddr_in serverAddr;
	struct hostent *serverHost;
	socklen_t serverAddrLen;

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

	//Przygotowanie bufora
	buffer[0]=CONF_REQ;
	int *rHor =(int*)(&buffer[1]);
    *rHor = 1920;
    int *rVer = (int*)(&buffer[5]);
    *rVer = 1080;
    float *test1 = (float*)(&buffer[9]);
    *test1 = 10.2;
    float *test2 = (float*)(&buffer[13]);
    *test2 = 15.1;
    float *test3 = (float*)(&buffer[17]);
    *test3 = 2.23;

	//Przesylamy parametry konfiguracyjne kamery
	if(sendto(socketFd, buffer, 21, 0, (struct sockaddr *) &serverAddr, serverAddrLen) < 0)
		error("Failed sendto");
	//Czekamy na odpowiedz
	if(recvfrom(socketFd, buffer, sizeof(buffer), 0, (struct sockaddr *) &serverAddr, &serverAddrLen) < 0)
		error("Failed recvfrom");
	if(buffer[0]==CONF_ACK)
	{
		int respond=buffer[1];
		printf("%i\n", respond);
	}
	else printf("Bledna odpowiedz\n");

	//Sprawdzamy polaczenie z kamera i jej konfiguracje
	buffer[0]=TEST_REQ;
	if(sendto(socketFd, buffer, strlen(buffer), 0, (struct sockaddr *) &serverAddr, serverAddrLen) < 0)
		error("Failed sendto");
	//Czekamy na odpowiedz
	if(recvfrom(socketFd, buffer, sizeof(buffer), 0, (struct sockaddr *) &serverAddr, &serverAddrLen) < 0)
		error("Failed recvfrom");
	if(buffer[0]==TEST_ACK)
	{
		int respond=buffer[1];
		printf("%i\n", respond);
	}
	else printf("Bledna odpowiedz\n");
	
	shutdown(socketFd, SHUT_RDWR );
	return 0;
}


