//-- camera.cpp
//-- Projekt : Camera-Link
//-- Autor : Maciej Puchalski
//-- Data utworzenia : 17.04.2020
#include "camera.h"
#include <pthread.h>
#include <ctime>
#include <algorithm>
#include <string>
#include <stdio.h>
#include <chrono>
#include <unistd.h>
#define NUM_THREADS 5
#define SEND_PHOTO_INTERVAL 120
#define ADDRESS_SIZE 46
 Camera camera;
int gatePort4;
int gatePort6;
int gatePhotoPort;
bool isIpv6connected = false;
bool isIpv4connected = false;
pthread_cond_t ipv4_cond;
pthread_cond_t ipv6_cond;
pthread_mutex_t mutexIpv4;
pthread_mutex_t mutexIpv6;

pthread_t threads[NUM_THREADS];
struct sockaddr_in gate4
{
    .sin_family = AF_INET,
    .sin_port = htons(gatePort4)
};
struct sockaddr_in gatePhoto4
{
    .sin_family = AF_INET,
    .sin_port = htons(gatePhotoPort)
};
struct sockaddr_in6 gate6
{
    .sin6_family = AF_INET6,
    .sin6_port = htons(gatePort6)
};
struct sockaddr_in6 gatePhoto6
{
    .sin6_family = AF_INET6,
    .sin6_port = htons(gatePhotoPort)
};

void *photoSender(void *data)

{
    bool *ipv6 = (bool *)data;
    char buffer[BUFFER_LEN] = {};
    char filename[10] = "photo.jpg";
    pthread_exit(NULL);
    if (*ipv6)
    {
        const int socket_ = socket(AF_INET6, SOCK_DGRAM, 0);
        socklen_t len6 = sizeof(gate6);

        while (true)
        {
            //send photo

            std::fstream file;
            file.open(filename, std::ios::in | std::ios::out | std::ios::binary | std::ios::ate);
            file.seekg(0, file.end);

            file.seekg(0, std::ios::beg);
            FileMessage fileMessage(DATA_MSG, file, filename, sizeof(filename));
            pthread_mutex_lock(&mutexIpv6);
            strcpy(buffer, fileMessage.sendFileInfo());
            if (sendto(socket_, buffer, sizeof(buffer), 0, (struct sockaddr *)(&gatePhoto6), len6) < 0)
            {
                perror("sendto() ERROR");
                exit(5);
            }
            memset(buffer, 0, sizeof(buffer));
            int size = fileMessage.get_package_amount();
            for (int i = 0; i < size; ++i)
            {
                strcpy(buffer, fileMessage.sendPackage(i));
                int bufferSize;
                if (i == size - 1)
                    bufferSize = fileMessage.get_last_package_size();
                else
                    bufferSize = fileMessage.get_package_size();
                if (sendto(socket_, buffer, bufferSize, 0, (struct sockaddr *)(&gatePhoto6), len6) < 0)
                {
                    perror("sendto() ERROR");
                    exit(5);
                }
                memset(buffer, 0, sizeof(buffer));
            }
            while (1)
            {
                if (recvfrom(socket_, buffer, sizeof(buffer), 0, (struct sockaddr *)&(gatePhoto6), &len6) < 0)
                {
                    perror("recvfrom() ERROR");
                    exit(1);
                }
                if (buffer[0] == DATA_ACK)
                {
                    memset(buffer, 0, sizeof(buffer));
                    break;
                }
                else
                {
                    int packageNr = buffer[1];
                    int bufferSize;
                    if (packageNr == size - 1)
                        bufferSize = fileMessage.get_last_package_size();
                    else
                        bufferSize = fileMessage.get_package_size();
                    strcpy(buffer, fileMessage.sendPackage(packageNr));
                    if (sendto(socket_, buffer, bufferSize, 0, (struct sockaddr *)(&gatePhoto6), len6) < 0)
                    {
                        perror("sendto() ERROR");
                        exit(5);
                    }
                    memset(buffer, 0, sizeof(buffer));
                }
            }

            pthread_mutex_unlock(&mutexIpv6);
            sleep(SEND_PHOTO_INTERVAL);
        }
    }
    else
    {
        const int socket_ = socket(AF_INET, SOCK_DGRAM, 0);
        socklen_t len4 = sizeof(gate4);
        //while (true)
        {
            //send photo
            std::fstream file;
            file.open(filename, std::ios::in | std::ios::out | std::ios::binary | std::ios::ate);
            file.seekg(0, file.end);
            file.seekg(0, std::ios::beg);
            FileMessage fileMessage(DATA_MSG, file, filename, sizeof(filename));
            if (file.is_open())
            {
                pthread_mutex_lock(&mutexIpv4);
                if (file.is_open())
                {
                    strcpy(buffer, fileMessage.sendFileInfo());

                    if (sendto(socket_, buffer, sizeof(buffer), 0, (struct sockaddr *)(&gatePhoto4), len4) < 0)
                    {
                        perror("sendto() ERROR");
                        exit(5);
                    }
                    memset(buffer, 0, sizeof(buffer));
                    int size = fileMessage.get_package_amount();

                    for (int i = 0; i < size - 1; ++i)
                    {
                        strcpy(buffer, fileMessage.sendPackage(i));
                        int bufferSize;
                        if (i == size - 1)
                            bufferSize = fileMessage.get_last_package_size();
                        else
                            bufferSize = fileMessage.get_package_size();
                        if (sendto(socket_, buffer, bufferSize, 0, (struct sockaddr *)(&gatePhoto4), len4) < 0)
                        {
                            perror("sendto() ERROR");
                            exit(5);
                        }

                        memset(buffer, 0, sizeof(buffer));
                    }
                    while (1)
                    {
                        if (recvfrom(socket_, buffer, sizeof(buffer), 0, (struct sockaddr *)&(gatePhoto4), &len4) < 0)
                        {
                            perror("recvfrom() ERROR");
                            exit(1);
                        }
                        if (buffer[0] == DATA_ACK)
                        {
                            memset(buffer, 0, sizeof(buffer));
                            break;
                        }
                        else
                        {
                            int packageNr = buffer[1];
                            int bufferSize;
                            if (packageNr == size - 1)
                                bufferSize = fileMessage.get_last_package_size();
                            else
                                bufferSize = fileMessage.get_package_size();
                            strcpy(buffer, fileMessage.sendPackage(packageNr));
                            if (sendto(socket_, buffer, bufferSize, 0, (struct sockaddr *)(&gatePhoto4), len4) < 0)
                            {
                                perror("sendto() ERROR");
                                exit(5);
                            }
                            memset(buffer, 0, sizeof(buffer));
                        }
                    }

                    pthread_mutex_unlock(&mutexIpv4);
                }
            }
            sleep(SEND_PHOTO_INTERVAL);
        }
    }

    pthread_exit(NULL);
}

void saveLog(string logText, bool ipv6, string ipAdress)
{
    fstream file;
    time_t now = time(0);
    string log = std::ctime(&now);
    log.erase(std::remove(log.begin(), log.end(), '\n'), log.end());
    log = log + " : adress ip=" + ipAdress + " : ";
    log = log + logText + "\n";
    cout << log;
    string filename = "cameraLog" + std::to_string(gatePort4) + ".log";
    file.open(filename, fstream::app);
    file << log;
    file.close();
}
void loadGateAdressFromFile(string filename, char *gateAdress)
{
    ifstream file;
    file.open(filename);
    file >> gateAdress;
    file.close();
}
const int getSocket(bool ipv6)
{
    if (ipv6 == true)
    {
        return socket(AF_INET6, SOCK_DGRAM, 0);
    }
    else
    {
        return socket(AF_INET, SOCK_DGRAM, 0);
    }
}

void *listener(void *data)
{
   
    //sockaddr_in *gate = (sockaddr_in *)data;
    bool *ipv6 = (bool *)data;

    const int socket_ = getSocket(*ipv6);

    if (socket_ < 0)
    {
        perror("socket() ERROR");
        exit(1);
    }
    socklen_t len4 = sizeof(gate4);
    socklen_t len6 = sizeof(gate6);
    // char zeroAdress[128] = {"0.0.0.0"};
    char gateAdress[ADDRESS_SIZE] = {};

    if (*ipv6)
    {
        loadGateAdressFromFile("gateAdress6.config", gateAdress);
    }
    else
    {
        loadGateAdressFromFile("gateAdress4.config", gateAdress);
    }
    if (*ipv6 && strcmp(gateAdress, ""))
    {
        inet_pton(AF_INET6, gateAdress, &gate6.sin6_addr);
        isIpv6connected = true;
        inet_pton(AF_INET6, gateAdress, &gatePhoto6.sin6_addr);
        pthread_create(&threads[2], NULL, photoSender, &*ipv6);
    }
    else if (strcmp(gateAdress, ""))
    {
        inet_pton(AF_INET, gateAdress, &gate4.sin_addr);
        inet_pton(AF_INET, gateAdress, &gatePhoto4.sin_addr);
        isIpv4connected = true;
        pthread_create(&threads[2], NULL, photoSender, &*ipv6);
    }

    if (*ipv6)
    {
        if (bind(socket_, (struct sockaddr *)&(gate6), len6) < 0)
        {
            perror("bind() ERROR");
            exit(3);
        }
    }
    else
    {
        if (bind(socket_, (struct sockaddr *)&(gate4), len4) < 0)
        {
            perror("bind() ERROR");
            exit(3);
        }
    }

    char buffer[BUFFER_LEN];

    while (1)
    {
        cout << "waiting for gate msg\n";
        char from[ADDRESS_SIZE] = {};
        //struct pollfd pfd = {.fd = socket_, .events = POLLIN};
        //poll(&pfd, 1, 1000);
        if (*ipv6)
        {

            if (recvfrom(socket_, buffer, sizeof(buffer), 0, (struct sockaddr *)&(gate6), &len6) < 0)
            {
                perror("recvfrom() ERROR");
                exit(1);
            }
            pthread_mutex_lock(&mutexIpv6);
            while (isIpv4connected)
            {
                pthread_cond_wait(&ipv6_cond, &mutexIpv6);
            }
            pthread_mutex_unlock(&mutexIpv6);
            inet_ntop(AF_INET6, &gate6.sin6_addr, from, sizeof(from));
        }
        else
        {

            if (recvfrom(socket_, buffer, sizeof(buffer), 0, (struct sockaddr *)&(gate4), &len4) < 0)
            {
                perror("recvfrom() ERROR");
                exit(1);
            }
            pthread_mutex_lock(&mutexIpv4);
            while (isIpv6connected)
            {
                pthread_cond_wait(&ipv4_cond, &mutexIpv4);
            }
            pthread_mutex_unlock(&mutexIpv4);

            inet_ntop(AF_INET, &gate4.sin_addr, from, sizeof(from));
        }

        if (buffer[0] == INST_REQ)
        {
            saveLog("inst req", *ipv6, from);
            if (strcmp(gateAdress, ""))
            {
                buffer[0] = IS_PAIR;

                if (*ipv6)
                {
                    if (sendto(socket_, buffer, 1, 0, (struct sockaddr *)(&gate6), len6) < 0)
                    {
                        perror("sendto() ERROR");
                        exit(5);
                    }
                }
                else
                {
                    if (sendto(socket_, buffer, 1, 0, (struct sockaddr *)(&gate4), len4) < 0)
                    {
                        perror("sendto() ERROR");
                        exit(5);
                    }
                }
            }
            else
            {
                if (strcmp(camera.getPassword(), buffer + 1))
                {
                    memset(buffer, 0, sizeof(buffer));
                    strcpy(gateAdress, from);
                    ofstream file;
                    buffer[0] = INST_ACK;
                    if (*ipv6)
                    {
                        file.open("gateAdress6.config");
                        isIpv6connected = true;
                        inet_pton(AF_INET6, gateAdress, &gate6.sin6_addr);
                        inet_pton(AF_INET6, gateAdress, &gatePhoto6.sin6_addr);

                        if (sendto(socket_, buffer, 1, 0, (struct sockaddr *)(&gate6), len4) < 0)
                        {
                            perror("sendto() ERROR");
                            exit(5);
                        }
                    }
                    else
                    {
                        file.open("gateAdress4.config");
                        isIpv4connected = true;
                        inet_pton(AF_INET, gateAdress, &gate4.sin_addr);
                        inet_pton(AF_INET, gateAdress, &gatePhoto4.sin_addr);

                        if (sendto(socket_, buffer, 1, 0, (struct sockaddr *)(&gate4), len4) < 0)
                        {
                            perror("sendto() ERROR");
                            exit(5);
                        }
                    }
                    saveLog("succesful pairing (inst ack)", *ipv6, gateAdress);
                    cout << "udane parowanie\n";
                    file.write(gateAdress, sizeof(gateAdress));
                    file.close();
                }
                else
                {
                    buffer[0] = NO_PAIR;
                    if (sendto(socket_, buffer, 1, 0, (struct sockaddr *)(&gate4), len4) < 0)
                    {
                        perror("sendto() ERROR");
                        exit(5);
                    }
                    saveLog("failed pairing", *ipv6, gateAdress);
                    cout << "nieudane parowanie\n";
                    memset(gateAdress, 0, sizeof(gateAdress));
                }
            }
        }
        else if (strcmp(gateAdress, "") && strcmp(gateAdress, from) == 0)
        {

            if (buffer[0] == CONF_REQ)
            {

                saveLog("conf req", *ipv6, gateAdress);
                camera.configure(buffer);
                memset(buffer, 0, sizeof(buffer));
                buffer[0] = CONF_ACK;

                if (*ipv6)
                {
                    if (sendto(socket_, buffer, strlen(buffer), 0, (struct sockaddr *)(&gate6), len6) < 0)
                    {
                        perror("sendto() ERROR");
                        exit(5);
                    }
                }
                else
                {
                    if (sendto(socket_, buffer, strlen(buffer), 0, (struct sockaddr *)(&gate4), len4) < 0)
                    {
                        perror("sendto() ERROR");
                        exit(5);
                    }
                }

                saveLog("conf ack", *ipv6, gateAdress);
                memset(buffer, 0, sizeof(buffer));
            }
            else if (buffer[0] == TEST_REQ)
            {
                memset(buffer, 0, sizeof(buffer));
                camera.test(buffer);
                saveLog("test req", *ipv6, gateAdress);
                if (*ipv6)
                {
                    if (sendto(socket_, buffer, strlen(buffer), 0, (struct sockaddr *)(&gate6), len6) < 0)
                    {
                        perror("sendto() ERROR");
                        exit(5);
                    }
                }
                else
                {
                    if (sendto(socket_, buffer, strlen(buffer), 0, (struct sockaddr *)(&gate4), len4) < 0)
                    {
                        perror("sendto() ERROR");
                        exit(5);
                    }
                }

                saveLog("test ack", *ipv6, gateAdress);
                memset(buffer, 0, sizeof(buffer));
            }
            else if (buffer[0] == DISC_REQ)
            {
                saveLog("disc req", *ipv6, gateAdress);
                buffer[0] = DISC_ACK;
                memcpy(gateAdress, "", sizeof(gateAdress));
                ifstream file;
                pthread_cancel(threads[2]);
                if (*ipv6)
                {
                    remove("gateAdress6.config");

                    if (sendto(socket_, buffer, strlen(buffer), 0, (struct sockaddr *)(&gate6), len6) < 0)
                    {
                        perror("sendto() ERROR");
                        exit(5);
                    }
                    isIpv6connected = false;
                    pthread_cond_signal(&ipv4_cond);
                }
                else
                {
                    remove("gateAdress4.config");
                    if (sendto(socket_, buffer, strlen(buffer), 0, (struct sockaddr *)(&gate4), len4) < 0)
                    {
                        perror("sendto() ERROR");
                        exit(5);
                    }
                    isIpv4connected = false;
                    pthread_cond_signal(&ipv6_cond);
                }

                saveLog("disc ack", true, gateAdress);

                memset(buffer, 0, sizeof(buffer));
            }
        }
        else
        {
            if (*ipv6)
            {
                inet_ntop(AF_INET6, &gate6.sin6_addr, gateAdress, sizeof(gateAdress));
            }
            else
            {
                inet_ntop(AF_INET, &gate4.sin_addr, gateAdress, sizeof(gateAdress));
            }

            saveLog("no pair", *ipv6, gateAdress);
            memcpy(gateAdress, "", sizeof(gateAdress));
            buffer[0] = NO_PAIR;

            if (*ipv6)
            {
                if (sendto(socket_, buffer, strlen(buffer), 0, (struct sockaddr *)(&gate6), len6) < 0)
                {
                    perror("sendto() ERROR");
                    exit(5);
                }
            }
            else
            {
                if (sendto(socket_, buffer, strlen(buffer), 0, (struct sockaddr *)(&gate4), len4) < 0)
                {
                    perror("sendto() ERROR");
                    exit(5);
                }
            }

            memset(buffer, 0, sizeof(buffer));
        }
    }

    shutdown(socket_, SHUT_RDWR);

    pthread_exit(NULL);
}

void *conectionResponser()
{

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{

    if (argc < 4)
    {
        fprintf(stderr, "Use: \"%s portIPv4 portIPv6 portPhoto\"", argv[0]);
        exit(1);
    }
    gatePort4 = atoi(argv[1]);
    gatePort6 = atoi(argv[2]);
    gatePhotoPort = atoi(argv[3]);
    gate4.sin_port = htons(gatePort4);
    gate6.sin6_port = htons(gatePort6);
    gatePhoto4.sin_port = htons(gatePhotoPort);
    gatePhoto6.sin6_port = htons(gatePhotoPort);
    bool ipv4 = false;
    bool ipv6 = true;
    camera.loadCameraConfig("camera.config");
    if(!camera.loadPassword("password"))return 0;
    pthread_create(&threads[0], NULL, listener, &ipv4);
    pthread_create(&threads[1], NULL, listener, &ipv6);
    while (1)
        ;
    return 0;
}
// gcc camera.cpp -g -Wall -pthread -o camera -lstdc++ && ./camera 6666 6667
