//-- Camera
//-- Autor: Maciej Puchalski
#include "camera.h"
#include <pthread.h>
#include <ctime>
#include <algorithm>
#include <string>
#include <stdio.h>
#define NUM_THREADS 5

int gatePort4;
int gatePort6;

void saveLog(string logText, bool ipv6, string ipAdress)
{
    fstream file;
    time_t now = time(0);
    string log = std::ctime(&now);
    log.erase(std::remove(log.begin(), log.end(), '\n'), log.end());
    log = log + " : port = " + std::to_string(gatePort4) + " : adress ip=" + ipAdress + " : ";
    log = log + logText + "\n";
    cout << log;
    file.open("cameraLog.txt", fstream::app);
    file << log;
    file.close();
}
void loadFromFile(string filename, char *gateAdress)
{
    ifstream file;
    file.open(filename);
    file >> gateAdress;
    file.close();
}
void *listenerIpv4(void *data)
{
    Camera camera;
    sockaddr_in *gate = (sockaddr_in *)data;
    const int socket_ = socket(AF_INET, SOCK_DGRAM, 0);

    if (socket_ < 0)
    {
        perror("socket() ERROR");
        exit(1);
    }
    socklen_t len4 = sizeof(*gate);
    char test[128];
    inet_ntop(AF_INET, &gate->sin_addr, test, sizeof(test));
    char zeroAdress[128] = {"0.0.0.0"};
    char gateAdress[128] = {};
    char from[128] = {};
    loadFromFile("gateAdress4.config", gateAdress);

    if (strcmp(gateAdress, "") == 0)
    {
        strcpy(gateAdress, zeroAdress);
    }
    else
    {
        inet_pton(AF_INET, gateAdress, &gate->sin_addr);
    }

    if (bind(socket_, (struct sockaddr *)&(*gate), len4) < 0)
    {
        perror("bind() ERROR");
        exit(3);
    }
    char buffer[BUFFER_LEN];

    while (1)
    {
        cout << "waiting for gate msg\n";
        if (recvfrom(socket_, buffer, sizeof(buffer), 0, (struct sockaddr *)(&*gate), &len4) < 0)
        {
            perror("recvfrom() ERROR");
            exit(1);
        }
        inet_ntop(AF_INET, &gate->sin_addr, from, sizeof(gateAdress));
        if (buffer[0] == INST_REQ)
        {
            if (strcmp(gateAdress, zeroAdress))
            {
                buffer[0] = IS_PAIR;
                if (sendto(socket_, buffer, 1, 0, (struct sockaddr *)(&*gate), len4) < 0)
                {
                    perror("sendto() ERROR");
                    exit(5);
                }
            }
            else
            {
                inet_ntop(AF_INET, &gate->sin_addr, gateAdress, sizeof(gateAdress));
                saveLog("inst req", false, gateAdress);
                memset(buffer, 0, sizeof(buffer));
                Message msg(INST_HASH, "okon", 4);
                strcpy(buffer, msg.get_code());

                if (sendto(socket_, buffer, 5, 0, (struct sockaddr *)(&*gate), len4) < 0)
                {
                    perror("sendto() ERROR");
                    exit(5);
                }

                saveLog("install code sent", false, gateAdress);
                memset(buffer, 0, sizeof(buffer));

                if (recvfrom(socket_, buffer, sizeof(buffer), 0, (struct sockaddr *)(&*gate), &len4) < 0)
                {
                    perror("recvfrom() ERROR");
                    exit(1);
                }

                if (buffer[0] == INST_ACK)
                {

                    saveLog("succesful pairing (inst ack)", false, gateAdress);
                    cout << "udane parowanie\n";
                    ofstream file;
                    file.open("gateAdress4.config");
                    file.write(gateAdress, sizeof(gateAdress));
                }
                else
                {
                    saveLog("failed pairing", false, gateAdress);
                    cout << "nieudane parowanie\n";
                    memset(gateAdress, 0, sizeof(gateAdress));
                }
            }
        }
        else if (strcmp(gateAdress, zeroAdress) && strcmp(gateAdress, from) == 0)
        {

            if (buffer[0] == CONF_REQ)
            {

                saveLog("conf req", false, gateAdress);
                camera.configure(buffer);
                memset(buffer, 0, sizeof(buffer));
                buffer[0] = CONF_ACK;
                if (sendto(socket_, buffer, strlen(buffer), 0, (struct sockaddr *)(&*gate), len4) < 0)
                {
                    perror("sendto() ERROR");
                    exit(5);
                }
                saveLog("conf ack", false, gateAdress);
                memset(buffer, 0, sizeof(buffer));
            }
            else if (buffer[0] == TEST_REQ)
            {
                memset(buffer, 0, sizeof(buffer));
                camera.test(buffer);
                saveLog("test req", false, gateAdress);
                if (sendto(socket_, buffer, strlen(buffer), 0, (struct sockaddr *)(&*gate), len4) < 0)
                {
                    perror("sendto() ERROR");
                    exit(5);
                }
                saveLog("test ack", false, gateAdress);
                memset(buffer, 0, sizeof(buffer));
            }
            else if (buffer[0] == DISC_REQ)
            {
                saveLog("disc req", false, gateAdress);
                buffer[0] = DISC_ACK;
                memcpy(gateAdress, zeroAdress, sizeof(gateAdress));
                ifstream file;
                remove("gateAdress4.config");
                if (sendto(socket_, buffer, strlen(buffer), 0, (struct sockaddr *)(&*gate), len4) < 0)
                {
                    perror("sendto() ERROR");
                    exit(5);
                }
                saveLog("disc ack", false, gateAdress);
                memset(buffer, 0, sizeof(buffer));
            }
        }
        else
        {
            inet_ntop(AF_INET, &gate->sin_addr, gateAdress, sizeof(gateAdress));
            saveLog("no pair", false, gateAdress);
            memcpy(gateAdress, zeroAdress, sizeof(gateAdress));
            buffer[0] = NO_PAIR;
            if (sendto(socket_, buffer, strlen(buffer), 0, (struct sockaddr *)(&*gate), len4) < 0)
            {
                perror("sendto() ERROR");
                exit(5);
            }
            memset(buffer, 0, sizeof(buffer));
        }
    }

    shutdown(socket_, SHUT_RDWR);

    pthread_exit(NULL);
}

void *photoSender(void *data)
{

    pthread_exit(NULL);
}

void *conectionResponser()
{
    pthread_exit(NULL);
}

void *listenerIpv6(void *data)
{
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
    gatePort4 = atoi(argv[1]);
    gatePort6 = atoi(argv[2]);

    struct sockaddr_in gate4
    {
        .sin_family = AF_INET,
        .sin_port = htons(gatePort4)
    };
    struct sockaddr_in6 gate6
    {
        .sin6_family = AF_INET6,
        .sin6_port = htons(gatePort6)
    };

    pthread_create(&threads[0], NULL, listenerIpv4, &gate4);
    //   pthread_create(&threads[1], NULL,listenerIpv6, &gate6);
    while (1)
        ;
    return 0;
}
// gcc camera.cpp -g -Wall -pthread -o camera -lstdc++ && ./camera 6666 6667
