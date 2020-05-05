//-- Camera
//-- Autor: Maciej Puchalski
#include <stdio.h>
#include <stdlib.h>    // exit()
#include <string.h>    // memset()
#include <arpa/inet.h> // inet_pton()
#include <sys/socket.h>
#include "message.cpp"
#include <fstream>
#include <iostream>

class Camera
{
private:
    int resHorizontal; //resolution horizontal
    int resVertical;   //resolution vertical
    float focalLength;
    float interval;
    float jitter;

public:
    Camera();
    ~Camera();
    void configure(int resH, int resV, float fLength, float inter, float jitt);
};

Camera::Camera()
{
}
Camera::~Camera()
{
}
void Camera::configure(int resH, int resV, float fLength, float inter, float jitt)
{
    resHorizontal = resH;
    resVertical = resV;
    focalLength = fLength;
    interval = inter;
    jitter = jitt;
}



int main(int argc, char *argv[])
{
    Camera camera;
    if (argc < 2)
    {
        fprintf(stderr, "Use: \"%s port\"", argv[0]);
        exit(1);
    }
    int gatePort = atoi(argv[1]);
    
    // TO DO podział na wątki w zależności od tego czy bramka jest na IPv6 czy IPv4
    // lub jedno wspólne nasłuchiwanie naprzemiennie dla IPv4 i IPv6

    struct sockaddr_in gate =
        {
            .sin_family = AF_INET,
            .sin_port = htons(gatePort)};
    struct sockaddr_in gate6 =
        {
            .sin_family = AF_INET6,
            .sin_port = htons(gatePort)};

    const int socket_ = socket(AF_INET, SOCK_DGRAM, 0);

    if (socket_ < 0)
    {
        perror("socket() ERROR");
        exit(1);
    }

    char buffer[4096];

    socklen_t len = sizeof(gate);

    if (bind(socket_, (struct sockaddr *)&gate, len) < 0)
    {
        perror("bind() ERROR");
        exit(3);
    }
    struct sockaddr_in from = {};

    //instalacja
    char gateAdress[128] = {};
    if(argc>2 && argv[2]==std::string("-i"))
    {
    while (1)
    {

        memset(buffer, 0, sizeof(buffer));
        printf("Waiting for connection...\n");

        if (recvfrom(socket_, buffer, sizeof(buffer), 0, (struct sockaddr *)&from, &len) < 0)
        {
            perror("recvfrom() ERROR");
            exit(1);
        }
        printf("%s", buffer);
        //int port=ntohs(from.sin_port);        
        printf("|Gate ip: %s port: %d|\n", inet_ntop(AF_INET, &from.sin_addr, gateAdress, sizeof(gateAdress)), ntohs(from.sin_port));
        
        if (inet_pton(AF_INET, gateAdress, &gate.sin_addr) <= 0)
        {
            perror("inet_pton() ERROR");
            exit(1);
        }
        //std::cout << "test\n";
        //Message msg(0, "okon");
        //#char dataBlock[sizeof(msg)];
        strncpy(buffer, "nawiazano polaczenie", sizeof(buffer));
        //std::cout <<"wiadomosc to :"<< msg.get_msg();
        //wysyłać przez serializowaną classe messages
        if (sendto(socket_, buffer, strlen(buffer), 0, (struct sockaddr *)&from, len) < 0)
        {
            perror("sendto() ERROR");
            exit(5);
        }
        memset(buffer, 0, sizeof(buffer));
        
        if (recvfrom(socket_, buffer, sizeof(buffer), 0, (struct sockaddr *)&gate, &len) < 0)
        {
            perror("recvfrom() ERROR");
            exit(1);
        }
        char adress[128];
        inet_ntop(AF_INET, &from.sin_addr, adress, sizeof(gateAdress));
        char code[4096]={"polaczono\n"};
        if (strcmp(buffer,code)==0 && strcmp(gateAdress,adress)==0)
        {
            printf("udane parowanie\n");
            ofstream file;
            file.open("gateAdress.conf");

            char bufferWrite[sizeof(sockaddr_in)];
            memcpy(bufferWrite,&gate,sizeof(sockaddr_in));
            file.write(bufferWrite,sizeof(sockaddr_in));
            break;
        }
        else
        {
            printf("nieudane parowanie\n");
            memset( gateAdress, 0, sizeof( gateAdress ) );
            gate.sin_port =htons(gatePort);
        }
    }
    printf("%s",buffer);
    printf("%s", gateAdress);
    }else
    {
            ifstream file;
            file.open("gateAdress.conf");
            char bufferWrite[sizeof(sockaddr_in)];
            file.read(bufferWrite,sizeof(sockaddr_in));
            gate = *reinterpret_cast<sockaddr_in*>(bufferWrite);
    }
    
    //konfiguracja
    //problem z serializacja
    if (recvfrom(socket_, buffer, sizeof(buffer), 0, (struct sockaddr *)&gate, &len) < 0)
    {
        perror("recvfrom() ERROR");
        exit(1);
    }
    
    std::cout << buffer;
    shutdown(socket_, SHUT_RDWR);
}
// gcc camera.cpp -g -Wall -o camera && ./camera 6666
