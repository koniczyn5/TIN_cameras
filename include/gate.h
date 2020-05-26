// gate.h
// Projekt: Camera-Link
// Autor: Konrad Meysztowicz-Wiśniewski
// Data utworzenia: 22.05.2020

#ifndef GATE_H
#define GATE_H

#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h> //inet_ntop
#include <netdb.h>
#include <string>
#include <string.h>
#include <fstream>
#include <ctime>
#include <algorithm> //std::remove
#include <vector>

#include "message.h"
#include "fileMessage.h"

void saveLog(std::string logText, const struct sockaddr *ai_addr, int port);
std::string ipToString(const struct sockaddr *ipAddress);
void error(const char *msg);
void getValue(int *dest);
void getValue(float *dest);
bool installCamera(int socketFd, sockaddr *ai_addr, socklen_t ai_addrlen, int port);
void photoReceiver(int socketFd, sockaddr *ai_addr, socklen_t ai_addrlen, int port, float interval);
void configureCamera(int socketFd, sockaddr *ai_addr, socklen_t ai_addrlen, int port);
void testConnection(int socketFd, sockaddr *ai_addr, socklen_t ai_addrlen, int port);

#endif // GATE_H
