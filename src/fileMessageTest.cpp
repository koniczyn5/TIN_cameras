// fileMessageTest.cpp
// Projekt:	Camera-Link
// Autor: Lukasz Rombel
// Data utworzenia: 23.05.2020
#include "fileMessage.h"

#define BOOST_TEST_MODULE FileMessageTests
#include <boost/test/included/unit_test.hpp>
#include <boost/test/test_tools.hpp>
#include <boost/lambda/lambda.hpp>

BOOST_AUTO_TEST_CASE(FileMessage_constructor1_test)
{
	char id = TEST_ACK;
	char arr[10] = "test1.txt";

	std::ofstream outfile("test1.txt");
	outfile << "test1.txt" << std::endl;
	outfile.close();

	FileMessage *filemessage1 = new FileMessage( id, "test1.txt", arr, sizeof(arr));
	BOOST_CHECK_EQUAL(filemessage1->get_id(), id);
	BOOST_CHECK_EQUAL(filemessage1->get_file_name(), arr);
	BOOST_CHECK_EQUAL(filemessage1->get_file_name_size(), sizeof(arr));
	delete filemessage1;
}

BOOST_AUTO_TEST_CASE(FileMessage_constructor2_test)
{
	char id = TEST_ACK;
	char arr[10] = "test2.txt";
	std::ofstream outfile("test2.txt");
	outfile << "test2.txt" << std::endl;
	outfile.close();

	std::fstream file_buffer;
	file_buffer.open("test2.txt", std::ios::in | std::ios::out | std::ios::binary | std::ios::ate);
	if (file_buffer.is_open())
	{
		FileMessage *filemessage2 = new FileMessage(id, file_buffer, arr, sizeof(arr));
		BOOST_CHECK_EQUAL(filemessage2->get_id(), id);
		BOOST_CHECK_EQUAL(filemessage2->get_file_name(), arr);
		BOOST_CHECK_EQUAL(filemessage2->get_file_name_size(), sizeof(arr));
		delete filemessage2;
	}
	else
		BOOST_ERROR("file does not load");
}

BOOST_AUTO_TEST_CASE(sendPackage_test)
{
	char id = TEST_ACK;
	char arr[10] = "test3.txt";
	std::ofstream outfile("test3.txt");
	outfile << "test3.txt" << std::endl;
	outfile.close();

	std::fstream file_buffer;
	file_buffer.open("test3.txt", std::ios::in | std::ios::out | std::ios::binary | std::ios::ate);
	file_buffer.seekg(0, file_buffer.end);
	int size = file_buffer.tellg();
	file_buffer.seekg(0, std::ios::beg);
	if (file_buffer.is_open())
	{
		FileMessage *filemessage3 = new FileMessage(id, file_buffer, arr, sizeof(arr));

		char * temp_arr = new char[size + 9];
		char * tmp = new char[size];

		int x = 1;
		temp_arr[0] = id;
		*(int*)&temp_arr[1] = x;
		*(int*)&temp_arr[5] = x;

		/*
		temp_arr[1] = (1 >> 24) & 0xFF;
		temp_arr[2] = (1 >> 16) & 0xFF;
		temp_arr[3] = (1 >> 8) & 0xFF;
		temp_arr[4] = 1 & 0xFF;
		temp_arr[5] = (1 >> 24) & 0xFF;
		temp_arr[6] = (1 >> 16) & 0xFF;
		temp_arr[7] = (1 >> 8) & 0xFF;
		temp_arr[8] = 1 & 0xFF;
		*/
		file_buffer.read(tmp, size);
		memcpy(temp_arr + 9, tmp, size);
		BOOST_CHECK_EQUAL(filemessage3->sendPackage(1), temp_arr);
		BOOST_CHECK_EQUAL(filemessage3->get_buffer_size(), size);
		BOOST_CHECK_EQUAL(filemessage3->get_package_amount(), (int)(size/512 + 1));
		BOOST_CHECK_EQUAL(filemessage3->get_last_package_size(), (int)(size - (512 * (filemessage3->get_package_amount() - 1))));
		delete temp_arr;
		delete tmp;
		delete filemessage3;
	}
	else
		BOOST_ERROR("file does not load");


}

BOOST_AUTO_TEST_CASE(sendFileName_test)
{
	char id = TEST_ACK;
	char arr[10] = "test4.txt";
	std::ofstream outfile("test4.txt");
	outfile << "test4.txt" << std::endl;
	outfile.close();

	FileMessage *filemessage4 = new FileMessage(id, "test4.txt", arr, sizeof(arr));

	char *arr2 = new char[9 + sizeof(arr)];
	int x = 1;
	arr2[0] = DATA_HDR;
	*(int*)&arr2[1] = sizeof(arr);
	*(int*)&arr2[5] = x;
	/*
	arr2[1] = (sizeof(arr) >> 24) & 0xFF;
	arr2[2] = (sizeof(arr) >> 16) & 0xFF;
	arr2[3] = (sizeof(arr) >> 8) & 0xFF;
	arr2[4] = sizeof(arr) & 0xFF;
	arr2[5] = (1 >> 24) & 0xFF;
	arr2[6] = (1 >> 16) & 0xFF;
	arr2[7] = (1 >> 8) & 0xFF;
	arr2[8] = 1 & 0xFF;
	*/
	memcpy(arr2 + 9, arr, sizeof(arr));
	BOOST_CHECK_EQUAL(filemessage4->sendFileInfo(), arr2);
	delete arr2;
	delete filemessage4;
}