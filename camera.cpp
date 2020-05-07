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
#define BUFFER_LEN 4096
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
    void configure(char buffer[]);
    void test(char buffer[]);
};

Camera::Camera()
{
    resHorizontal = 0;
    resVertical = 0;
    focalLength = 0;
    interval = 0;
    jitter = 0;
}
Camera::~Camera()
{
}
void Camera::configure(char buffer[])
{
    memcpy(&resHorizontal,buffer+1,4);
    memcpy(&resVertical,buffer+5,4);
    memcpy(&focalLength,buffer+9,4);
    memcpy(&interval,buffer+13,4);
    memcpy(&jitter,buffer+17,4); 
}
void Camera::test(char buffer[])
{
    buffer[0] = TEST_ACK;
    buffer[1]=0;
    if(resVertical != 0 ) 
    {
    ++buffer[1];
    }
    buffer[1] = buffer[1]<<1;
    if(resHorizontal != 0 ) 
    {
    ++buffer[1];
    }
    buffer[1] = buffer[1]<<1;
    if(focalLength != 0 ) 
    {
    ++buffer[1];
    }
    buffer[1] = buffer[1]<<1;
    if( interval != 0 ) 
    {
    ++buffer[1];
    }
    buffer[1] = buffer[1]<<1;
    if(jitter != 0 ) 
    {
    ++buffer[1];
    }
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

    char buffer[BUFFER_LEN];
     //testowe
    /*
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


    camera.configure(buffer);
    memset(buffer, 0, sizeof(buffer));
    camera.test(buffer);
    */

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
        memset(buffer, 0, sizeof(buffer));
        Message msg(0, "okon",4);
        strcpy(buffer,msg.get_code());
        //  strncpy(buffer, "nawiazano polaczenie", sizeof(buffer));
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
            char bufferRead[sizeof(sockaddr_in)];
            file.read(bufferRead,sizeof(sockaddr_in));
            gate = *reinterpret_cast<sockaddr_in*>(bufferRead);
    }
    
    //konfiguracja
    //problem z serializacja
    while(1)
    {
    if (recvfrom(socket_, buffer, sizeof(buffer), 0, (struct sockaddr *)&gate, &len) < 0)
    {
        perror("recvfrom() ERROR");
        exit(1);
    }
    if(buffer[0] == CONF_REQ)
    {
        camera.configure(buffer);
        memset(buffer, 0, sizeof(buffer));
        buffer[0] = CONF_ACK;
        if (sendto(socket_, buffer, strlen(buffer), 0, (struct sockaddr *)&gate, len) < 0)
        {
            perror("sendto() ERROR");
            exit(5);
        }        
        memset(buffer, 0, sizeof(buffer));
    }else if(buffer[0] = TEST_REQ)
    {
        memset(buffer, 0, sizeof(buffer));
        camera.test(buffer);
        if (sendto(socket_, buffer, strlen(buffer), 0, (struct sockaddr *)&gate, len) < 0)
        {
            perror("sendto() ERROR");
            exit(5);
        }     
        break;
    }
    }
    
    shutdown(socket_, SHUT_RDWR);
}
// gcc camera.cpp -g -Wall -o camera && ./camera 6666
