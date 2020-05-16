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
    Camera()
    {
    resHorizontal = 0;
    resVertical = 0;
    focalLength = 0;
    interval = 0;
    jitter = 0;
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
    void configure(char buffer[])
    {
    memcpy(&resHorizontal,buffer+1,4);
    memcpy(&resVertical,buffer+5,4);
    memcpy(&focalLength,buffer+9,4);
    memcpy(&interval,buffer+13,4);
    memcpy(&jitter,buffer+17,4); 
    }
    void test(char buffer[])
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
};


