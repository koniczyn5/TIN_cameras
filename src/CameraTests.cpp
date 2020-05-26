//-- CameraTests.cpp
//-- Projekt : Camera-Link
//-- Autor: Maciej Puchalski
//-- Data utworzenia : 16.05.2020
#include <boost/lambda/lambda.hpp>
#include <iostream>
#include "camera.h"
#define BOOST_TEST_MODULE CameraTest
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_CASE(CameraConfigure_test)
{
    Camera camera;
    char buffer[BUFFER_LEN];
	int *rHor =(int*)(&buffer[1]);
    *rHor = 1920;
    int *rVer = (int*)(&buffer[5]);
    *rVer = 1080;
    float *focalL = (float*)(&buffer[9]);
    *focalL = 10.2;
    float *inter = (float*)(&buffer[13]);
    *inter = 15.1;
    float *jitt = (float*)(&buffer[17]);
    *jitt = 2.23;

    camera.configure(buffer);

    BOOST_CHECK_EQUAL(*rHor,camera.getResHor());
    BOOST_CHECK_EQUAL(*rVer,camera.getResVer());
    BOOST_CHECK_EQUAL(*focalL,camera.getFocalLength());
    BOOST_CHECK_EQUAL(*inter,camera.getInterval());
    BOOST_CHECK_EQUAL(*jitt,camera.getJitter());
}

BOOST_AUTO_TEST_CASE(CameraTest_test)
{
    Camera camera;
    char buffer[BUFFER_LEN];
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
    BOOST_CHECK(buffer[0]==TEST_ACK);
    BOOST_CHECK(buffer[1]==31);
}


// g++ CameraTests.cpp -o test -DBOOST_TEST_DYN_LINK -lboost_unit_test_framework