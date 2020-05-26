// gate.cpp
// Projekt: Camera-Link
// Autor: Konrad Meysztowicz-Wiśniewski
// Data utworzenia: 16.04.2020
// Edytowane do testu przez: Adam Bieniek

#include "gate.h"

#define BUFFER_LEN 4096
#define PASSWORD "okon"
#define IPSTRLEN 40
#define RECVPORT 6668
#define PACKETSIZE 512
#define TIMEOUT 1
#define FAILNUM 3
#define PHOTOINTERVAL 5.0

std::string ipToString(const struct sockaddr *ipAddress)
{
    char buffer[IPSTRLEN] = {};
    switch(ipAddress->sa_family)
    {
    case AF_INET:
        inet_ntop(AF_INET, ipAddress, buffer, IPSTRLEN);
        break;
    case AF_INET6:
        inet_ntop(AF_INET6, ipAddress, buffer, IPSTRLEN);
        break;
    default:
        return "unknown AF";
    }

    std::string result = buffer;
    return result;
}

void saveLog(std::string logText, const struct sockaddr *ai_addr, int port)
{
    std::fstream file;
    time_t now = time(nullptr);
    std::string log = std::ctime(&now);
    log.erase(std::remove(log.begin(), log.end(), '\n'), log.end());
    std::string ipString = ipToString(ai_addr);
    log = log + " : adress ip=" + ipString + " : ";
    log = log + logText + "\n";
    std::cout << log;
    std::string filename = "gateLog" + std::to_string(port) + ".log";
    file.open(filename, std::fstream::app);
    if(!file.is_open())
        std::cout << "Failed to open log file\n";
    file << log;
    file.close();
}

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

bool installCamera(int socketFd, sockaddr *ai_addr, socklen_t ai_addrlen, int port)
{
    char buffer[BUFFER_LEN];
    memset(buffer, 0, BUFFER_LEN);
    std::string msgText = PASSWORD;
    buffer[0] = INST_REQ;
    strcpy((char*)&buffer[1], msgText.c_str());

    if(sendto(socketFd, buffer, 5, 0, ai_addr, ai_addrlen) < 0)
        error("Sendto()");

    saveLog("Camera installation request sent", ai_addr, port);

    memset(buffer, 0, BUFFER_LEN);
    if(recvfrom(socketFd, buffer, BUFFER_LEN, 0, ai_addr, &ai_addrlen) < 0)
    {
        if(errno == EAGAIN || errno == EWOULDBLOCK)
        {
            saveLog("Camera not responding", ai_addr, port);
            return false;
        }
        error("Recvfrom()");
    }

    if(buffer[0] == INST_ACK)
    {
        saveLog("Camera installation succeeded", ai_addr, port);
        return true;
    }
    else if(buffer[0] == IS_PAIR)
    {
        saveLog("Target camera is already paired with a device", ai_addr, port);
        return false;
    }
    else if(buffer[0] == NO_PAIR)
    {
        saveLog("Wrong password for target camera", ai_addr, port);
        return false;
    }

    saveLog("Camera installation failed for unknown reason", ai_addr, port);
    return false;
}

bool disconnectCamera(int socketFd, sockaddr *ai_addr, socklen_t ai_addrlen, int port)
{
    char buffer[BUFFER_LEN];
    memset(buffer, 0, BUFFER_LEN);
    buffer[0] = DISC_REQ;

    saveLog("Sending disconnect request to camera", ai_addr, port);

    if(sendto(socketFd, buffer, 1, 0, ai_addr, ai_addrlen) < 0)
        error("Sendto()");
    memset(buffer, 0, BUFFER_LEN);
    if(recvfrom(socketFd, buffer, BUFFER_LEN, 0, ai_addr, &ai_addrlen) < 0)
    {
        if(errno == EAGAIN || errno == EWOULDBLOCK)
        {
            saveLog("Camera not responding", ai_addr, port);
            return false;
        }
        error("Recvfrom()");
    }

    if(buffer[0] == DISC_ACK)
    {
        saveLog("Camera disconnect succeeded", ai_addr, port);
        return true;
    }
    if(buffer[0] == NO_PAIR)
    {
        saveLog("Camera was not connected", ai_addr, port);
        return false;
    }
    saveLog("Camera disconnect failed for unknown reason", ai_addr, port);
    return false;

}
void photoReceiver(int socketFd, sockaddr *ai_addr, socklen_t ai_addrlen, int port, float interval)
{
    char buffer[BUFFER_LEN];
    memset(buffer, 0, BUFFER_LEN);
    saveLog("Waiting for photo from camera", ai_addr, port);

    for (float f = 0.0;; f += (float)TIMEOUT)
    {
        memset(buffer, 0, BUFFER_LEN);
        if(recvfrom(socketFd, buffer, BUFFER_LEN, 0, ai_addr, &ai_addrlen) < 0)
        {
            if(f >= interval)
            {
                saveLog("Camera not responding", ai_addr, port);
                return;
            }
            else if(errno == EAGAIN || errno == EWOULDBLOCK)
                continue;
            else
                error("Recvfrom()");
        }
        if(buffer[0] == DATA_HDR || buffer[0] == DATA_MSG)
            break;
    }
    saveLog("Receiving data transfer from camera", ai_addr, port);
    int packetNum;
    int nOfPackets;
    nOfPackets = buffer[5]<<24 | buffer[6]<<16 | buffer[7]<<8 | buffer[8];
    bool received[nOfPackets+1];
    for(int i = 0; i < nOfPackets+1; ++i)
        received[i] = false;
    char dataBuffer[nOfPackets*PACKETSIZE];
    memset(dataBuffer, 0, nOfPackets*PACKETSIZE);

    int receivedCount = 0;

    if(buffer[0] == DATA_HDR)
    {
        received[0] = true;
        ++receivedCount;
    }
    else
    {
        packetNum = buffer[1]<<24 | buffer[2]<<16 | buffer[3]<<8 | buffer[4];
        received[packetNum] = true;
        ++receivedCount;
    }

    while(receivedCount < nOfPackets+1)
    {
        memset(buffer, 0, BUFFER_LEN);
        if(recvfrom(socketFd, buffer, BUFFER_LEN, 0, ai_addr, &ai_addrlen) < 0)
        {
            if(errno == EAGAIN || errno == EWOULDBLOCK)
                break;
            else
                error("Recvfrom()");
        }
        if(buffer[0] != DATA_MSG && buffer[0] != DATA_HDR)
            continue;
        if(buffer[0] == DATA_HDR)
        {
            received[0] = true;
            ++receivedCount;
        }
        else
        {
            packetNum = buffer[1]<<24 | buffer[2]<<16 | buffer[3]<<8 | buffer[4];
            received[packetNum] = true;
            ++receivedCount;
            memcpy(dataBuffer + (packetNum-1)*PACKETSIZE, buffer + 9, PACKETSIZE);
        }
    }
    if(receivedCount < nOfPackets+1)
    {
        saveLog("Data incomplete, requesting resend", ai_addr, port);
        for(int fails=0; fails<FAILNUM; ++fails)
        for(int currentPacket = 0; currentPacket < nOfPackets+1; ++currentPacket)
        {
            if(!received[currentPacket])
            {
                memset(buffer, 0, BUFFER_LEN);
                buffer[0] = DATA_RQT;
                int *rqtNum = (int*)&buffer[1];
                *rqtNum = currentPacket;
                if(sendto(socketFd, buffer, BUFFER_LEN, 0, ai_addr, ai_addrlen) < 0)
                    error("Sendto()");
                memset(buffer, 0, BUFFER_LEN);
                if(recvfrom(socketFd, buffer, BUFFER_LEN, 0, ai_addr, &ai_addrlen) < 0)
                {
                    if(errno == EAGAIN || errno == EWOULDBLOCK)
                        continue; //failed to get the package, try next
                    else
                        error("Recvfrom()");
                }
                if(buffer[0] != DATA_MSG && buffer[0] != DATA_HDR)
                    continue;
                if(buffer[0] == DATA_HDR)
                {
                    received[0] = true;
                    ++receivedCount;
                }
                else
                {
                    packetNum = buffer[1]<<24 | buffer[2]<<16 | buffer[3]<<8 | buffer[4];
                    if(packetNum <= 0)
                        continue;
                    received[packetNum] = true;
                    ++receivedCount;
                    memcpy(dataBuffer + (packetNum-1)*PACKETSIZE, buffer + 9, PACKETSIZE);
                }
                fails=0;
            }
        }
    }
    if(receivedCount < nOfPackets+1)
    {
        saveLog("Failed to get complete data from camera", ai_addr, port);
        return;
    }

    saveLog("Photo received, sending DATA_ACK", ai_addr, port);
    memset(buffer, 0, BUFFER_LEN);
    buffer[0] = DATA_ACK;
    if(sendto(socketFd, buffer, BUFFER_LEN, 0, ai_addr, ai_addrlen) < 0)
        error("Sendto()");


    std::fstream photoFile;
    time_t now = time(nullptr);
    std::string fileName = std::ctime(&now);
    fileName.erase(std::remove(fileName.begin(), fileName.end(), '\n'), fileName.end());
    std::string ipString = ipToString(ai_addr);
    fileName = fileName + "_" + ipString;
    std::replace(fileName.begin(), fileName.end(), ' ', '_');
    std::replace(fileName.begin(), fileName.end(), ':', '-');
    std::replace(fileName.begin(), fileName.end(), '.', '-');
    fileName = fileName + ".txt";
    photoFile.open(fileName, std::fstream::in | std::fstream::out | std::fstream::trunc);
    if(!photoFile.is_open())
    {
        saveLog("Failed to open photo file", ai_addr, port);
        return;
    }
    saveLog("Photo saved", ai_addr, port);
    photoFile << dataBuffer;
    photoFile.close();
    shutdown(socketFd, SHUT_RDWR);
}

void configureCamera(int socketFd, sockaddr *ai_addr, socklen_t ai_addrlen, int port)
{
    char buffer[BUFFER_LEN];
    memset(buffer, 0, BUFFER_LEN);

    buffer[0] = CONF_REQ;
    int *resHorizontal = (int*)(&buffer[1]);
    *resHorizontal = 1;
    int *resVertical = (int*)(&buffer[5]);
    *resVertical = 1;
    float *focalLength = (float*)(&buffer[9]);
    *focalLength = 1.0;
    float *interval = (float*)(&buffer[13]);
    *interval = 1.0;
    float *jitter = (float*)(&buffer[17]);
    *jitter = 1.0;

    //printf("Enter camera parameters:\n");

    //printf(" horizontal resolution (integer): ");
    //getValue(resHorizontal);
    //printf(" vertical resolution (integer): ");
    //getValue(resVertical);
    //printf(" focal length (float): ");
    //getValue(focalLength);
    //printf(" interval (float): ");
    //getValue(interval);
    //printf(" jitter (float): ");
    //getValue(jitter);

    saveLog("Sending configuration data to camera", ai_addr, port);

    if(sendto(socketFd, buffer, 21, 0, ai_addr, ai_addrlen) < 0)
        error("Sendto()");

    saveLog("Waiting for confirmation", ai_addr, port);

    if(recvfrom(socketFd, buffer, BUFFER_LEN, 0, ai_addr, &ai_addrlen) < 0)
    {
        if(errno == EAGAIN || errno == EWOULDBLOCK)
        {
            saveLog("Camera not responding", ai_addr, port);
            return;
        }

        error("Recvfrom()");
    }

    if(buffer[0] == CONF_ACK)
        saveLog("Configuration succeeded", ai_addr, port);
    else
        saveLog("Configuration failed", ai_addr, port);
}

void testConnection(int socketFd, sockaddr *ai_addr, socklen_t ai_addrlen, int port)
{
    char buffer[BUFFER_LEN];
    memset(buffer, 0, BUFFER_LEN);
    buffer[0] = TEST_REQ;

    saveLog("Testing connection with camera", ai_addr, port);

    if(sendto(socketFd, buffer, 1, 0, ai_addr, ai_addrlen) < 0)
        error("Sendto()");

    memset(buffer, 0, BUFFER_LEN);

    saveLog("Awaiting response to connection test", ai_addr, port);

    if(recvfrom(socketFd, buffer, 1, 0, ai_addr, &ai_addrlen) < 0)
    {
        if(errno == EAGAIN || errno == EWOULDBLOCK)
        {
            saveLog("Camera not responding", ai_addr, port);
            return;
        }

        error("Recvfrom()");
    }

    if(buffer[0] == TEST_ACK)
        saveLog("Connection test succeeded", ai_addr, port);
    else
        saveLog("Connection test failed", ai_addr, port);
}

int main (int argc, char *argv[])
{
    int connectSocketFd, recvSocketFd;
    addrinfo hints;
    addrinfo *cameraInfo;

	if(argc < 3)
	{
		fprintf(stderr, "Use: \"%s hostname port\"", argv[0]);
		exit(1);
    }

    struct timeval timeout;
    timeout.tv_sec = TIMEOUT;
    timeout.tv_usec = 0;

    connectSocketFd = socket(AF_INET, SOCK_DGRAM, 0);
    if(connectSocketFd < 0)
        error("Open connect socket");
    if(setsockopt(connectSocketFd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(struct timeval)) < 0)
        error("Setsockopt()");

    recvSocketFd = socket(AF_INET, SOCK_DGRAM, 0);
    if(recvSocketFd < 0)
        error("Open receive socket");

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(RECVPORT);
    addr.sin_addr.s_addr = INADDR_ANY;
    if(setsockopt(recvSocketFd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(struct timeval)) < 0)
        error("Setsockopt()");
    if(bind(recvSocketFd, (struct sockaddr *) &addr, sizeof(struct sockaddr_in)) < 0)
        error("Bind()");

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_NUMERICSERV;
    memset(&cameraInfo, 0, sizeof (cameraInfo));

    int errornum = getaddrinfo(argv[1], argv[2], &hints, &cameraInfo);
    if(errornum != 0)
    {
        fprintf(stderr, "Getaddrinfo(): %s\n", gai_strerror(errornum));
        exit(1);
    }

    installCamera(connectSocketFd, cameraInfo->ai_addr, cameraInfo->ai_addrlen, atoi(argv[2]));

    configureCamera(connectSocketFd, cameraInfo->ai_addr, cameraInfo->ai_addrlen, atoi(argv[2]));

    while(true)
    {
        testConnection(connectSocketFd, cameraInfo->ai_addr, cameraInfo->ai_addrlen, atoi(argv[2]));
    }

    freeaddrinfo(cameraInfo);
    shutdown(connectSocketFd, SHUT_RDWR);
    shutdown(recvSocketFd, SHUT_RDWR);

	return 0;
}
