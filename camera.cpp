//-- Camera
//-- Autor: Maciej Puchalski
#include "camera.h"
#include <pthread.h>
#define NUM_THREADS 5
void *listenerIp4(void *data)
{
    Camera camera;
    sockaddr_in *gate = (sockaddr_in *)data;
    const int socket_4 = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_4 < 0)
    {
        perror("socket() ERROR");
        exit(1);
    }
    socklen_t len4 = sizeof(*gate);
    char gateAdress[128] = {};

    if (bind(socket_4, (struct sockaddr *)&(*gate), len4) < 0)
    {
        perror("bind() ERROR");
        exit(3);
    }
    char buffer[BUFFER_LEN];

    while (1)
    {
        if (recvfrom(socket_4, buffer, sizeof(buffer), 0, (struct sockaddr *)&gate, &len4) < 0)
        {
            perror("recvfrom() ERROR");
            exit(1);
        }
        if (buffer[0] == INST_REQ)
        {

            memset(buffer, 0, sizeof(buffer));
            Message msg(INST_REQ, "okon", 4);
            strcpy(buffer, msg.get_code());

            if (sendto(socket_4, buffer, 5, 0, (struct sockaddr *)&gate, len4) < 0)
            {
                perror("sendto() ERROR");
                exit(5);
            }
            memset(buffer, 0, sizeof(buffer));

            if (recvfrom(socket_4, buffer, sizeof(buffer), 0, (struct sockaddr *)&gate, &len4) < 0)
            {
                perror("recvfrom() ERROR");
                exit(1);
            }
            char adress[128];
            inet_ntop(AF_INET, &gate->sin_addr, adress, sizeof(gateAdress));
            char code[4096] = {"polaczono\n"};
            if (buffer[0] = INST_ACK)
            {
                printf("udane parowanie\n");
                ofstream file;
                file.open("gateAdress.config");

                char bufferWrite[sizeof(sockaddr_in)];
                memcpy(bufferWrite, &gate, sizeof(sockaddr_in));
                file.write(bufferWrite, sizeof(sockaddr_in));
                break;
            }
            else
            {
                printf("nieudane parowanie\n");
                memset(gateAdress, 0, sizeof(gateAdress));
            }
        }
        else if (buffer[0] == CONF_REQ)
        {
            camera.configure(buffer);
            memset(buffer, 0, sizeof(buffer));
            buffer[0] = CONF_ACK;
            if (sendto(socket_4, buffer, strlen(buffer), 0, (struct sockaddr *)&gate, len4) < 0)
            {
                perror("sendto() ERROR");
                exit(5);
            }
            memset(buffer, 0, sizeof(buffer));
        }
        else if (buffer[0] == TEST_REQ)
        {
            memset(buffer, 0, sizeof(buffer));
            camera.test(buffer);
            if (sendto(socket_4, buffer, strlen(buffer), 0, (struct sockaddr *)&gate, len4) < 0)
            {
                perror("sendto() ERROR");
                exit(5);
            }
            break;
        }
        else if (buffer[0] == DISC_REQ)
        {
            buffer[0] = DISC_ACK;
            memset(&gate->sin_addr, 0, sizeof(gate->sin_addr));
            if (sendto(socket_4, buffer, strlen(buffer), 0, (struct sockaddr *)&gate, len4) < 0)
            {
                perror("sendto() ERROR");
                exit(5);
            }
        }
    }

    shutdown(socket_4, SHUT_RDWR);

    pthread_exit(NULL);
}
void *listenerIp6(void *threadid)
{
    //if (bind(socket_6, (struct sockaddr *)&gate6, len6) < 0)
    {
        //  perror("bind() ERROR");
        // exit(3);
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    pthread_t threads[NUM_THREADS];

    if (argc < 2)
    {
        fprintf(stderr, "Use: \"%s port\"", argv[0]);
        exit(1);
    }
    int gatePort4 = atoi(argv[1]);
    int gatePort6 = atoi(argv[2]);
    // TO DO podział na wątki w zależności od tego czy bramka jest na IPv6 czy IPv4
    // lub jedno wspólne nasłuchiwanie naprzemiennie dla IPv4 i IPv6

    struct sockaddr_in gate =
        {
            .sin_family = AF_INET,
            .sin_port = htons(gatePort4)};
    struct sockaddr_in gate6 =
        {
            .sin_family = AF_INET6,
            .sin_port = htons(gatePort6)};

    const int socket_6 = socket(AF_INET6, SOCK_DGRAM, 0);

    if (socket_6 < 0)
    {
        perror("socket() ERROR");
        exit(1);
    }
    cout << (struct sockaddr *)&gate << endl;
    socklen_t len6 = sizeof(gate6);
    pthread_create(&threads[0], NULL, listenerIp4, &gate);
    //   pthread_create(&threads[1], NULL,listenerIp6, &gate6);
    while (1)
        ;
    return 0;

    struct sockaddr_in from = {};
    //instalacja
    char gateAdress[128] = {};
    /*
    if(argc>3 && argv[3]==std::string("-i"))
    {
    while (1)
    {

        memset(buffer, 0, sizeof(buffer));
        printf("Waiting for connection...\n");

        if (recvfrom(socket_4, buffer, sizeof(buffer), 0, (struct sockaddr *)&from, &len4) < 0)
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
        if (sendto(socket_4, buffer, 5, 0, (struct sockaddr *)&from, len4) < 0)
        {
            perror("sendto() ERROR");
            exit(5);
        }
        memset(buffer, 0, sizeof(buffer));
        
        if (recvfrom(socket_4, buffer, sizeof(buffer), 0, (struct sockaddr *)&gate, &len4) < 0)
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
            gate.sin_port =htons(gatePort4);
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
    */
}
// gcc camera.cpp -g -Wall -pthread -o camera -lstdc++ && ./camera 6666 6667
