//-- camera.h
//-- Projekt : Camera-Link
//-- Autor: Maciej Puchalski
//-- Data utworzenia : 16.05.2020
#include <stdio.h>
#include <stdlib.h>    // exit()
#include <string.h>    // memset()
#include <arpa/inet.h> // inet_pton()
#include <sys/socket.h>
#include "message.cpp"
#include "fileMessage.cpp"
#include <fstream>
#include <iostream>
#include <poll.h>
#define BUFFER_LEN 4096
using namespace std;
class Camera
{
private:
    int resHorizontal; //resolution horizontal
    int resVertical;   //resolution vertical
    float focalLength;
    float interval;
    float jitter;
    char password[4];
public:
    Camera()
    {
        resHorizontal = 0;
        resVertical = 0;
        focalLength = 0;
        interval = 0;
        jitter = 0;
        memset(password, 0, sizeof(password));
    }
    ~Camera()
    {
    }
    int getResHor()
    {
        return resHorizontal;
    }
    int getResVer()
    {
        return resVertical;
    }
    float getFocalLength()
    {
        return focalLength;
    }
    float getInterval()
    {
        return interval;
    }
    float getJitter()
    {
        return jitter;
    }
    char *getPassword()
    {
        return password;
    }
    void configure(char buffer[])
    {
        memcpy(&resHorizontal, buffer + 1, 4);
        memcpy(&resVertical, buffer + 5, 4);
        memcpy(&focalLength, buffer + 9, 4);
        memcpy(&interval, buffer + 13, 4);
        memcpy(&jitter, buffer + 17, 4);
        cout << "resHorizonta=" << resHorizontal << endl;
        cout << "resVertical=" << resVertical << endl;
        cout << "focalLength=" << focalLength << endl;
        cout << "interval=" << interval << endl;
        cout << "jitter=" << jitter << endl;

        string data = to_string(resHorizontal) + "\n" + to_string(resVertical) + "\n" + to_string(focalLength) + "\n" + to_string(interval) + "\n" + to_string(jitter);
        saveToFile("camera.config", data);
    }
    void test(char buffer[])
    {

        buffer[0] = TEST_ACK;
        buffer[1] = 0;
        if (resVertical != 0)
        {
            ++buffer[1];
        }
        buffer[1] = buffer[1] << 1;
        if (resHorizontal != 0)
        {
            ++buffer[1];
        }
        buffer[1] = buffer[1] << 1;
        if (focalLength != 0)
        {
            ++buffer[1];
        }
        buffer[1] = buffer[1] << 1;
        if (interval != 0)
        {
            ++buffer[1];
        }
        buffer[1] = buffer[1] << 1;
        if (jitter != 0)
        {
            ++buffer[1];
        }
    }

    bool isConfigured()
    {
        if (resHorizontal > 0 && resVertical > 0 && focalLength > 0 && interval > 0 && jitter > 0)
            return true;
        else
            return false;
    }
    void saveToFile(string filename, string data)
    {
        fstream file;
        file.open(filename, std::ofstream::trunc | std::ios::out);
        file << data;
        file.close();
    }
    void loadCameraConfig(string filename)
    {
        string data;
        fstream file;
        file.open(filename);
        if (file.is_open())
        {
            getline(file, data);
            resHorizontal = stoi(data);
            getline(file, data);
            resVertical = stoi(data);
            getline(file, data);
            focalLength = stoi(data);
            getline(file, data);
            interval = stoi(data);
            getline(file, data);
            jitter = stoi(data);
            file.close();

            cout << "resHorizonta=" << resHorizontal << endl;
            cout << "resVertical=" << resVertical << endl;
            cout << "focalLength=" << focalLength << endl;
            cout << "interval=" << interval << endl;
            cout << "jitter=" << jitter << endl;
        }
    }
    bool loadPassword(string filename)
    {
        //string data[4];
        fstream file;
        file.open(filename);
        if (!file.is_open())
            return 0;
        file >> password;

        //strcpy(password,data)
        file.close();
        cout << password << endl;
        return 1;
    }
};
