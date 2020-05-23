#include "fileMessage.cpp"
#include <iostream>
#include <fstream>  

#define BOOST_TEST_MODULE fileMessageTests
#include <boost/test/included/unit_test.hpp>
#include <boost/test/test_tools.hpp>
#include <boost/lambda/lambda.hpp>

BOOST_AUTO_TEST_CASE(fileMessage_constructor1_test)
{
	char id = 5;
	char arr[10] = "test1.txt";
	std::ofstream outfile("C:/test1.txt");
	outfile << arr << std::endl;
	outfile.close();

	fileMessage *filemessage1 = new fileMessage( id, "C:/test1.txt", arr, sizeof(arr));

	BOOST_CHECK_EQUAL(filemessage1->get_id(), id);
	BOOST_CHECK_EQUAL(filemessage1->get_file_name(), arr);
	BOOST_CHECK_EQUAL(filemessage1->get_file_name_size(), sizeof(arr));
	delete filemessage1;
}

BOOST_AUTO_TEST_CASE(fileMessage_constructor2_test)
{
	char id = 5;
	char arr[10] = "test2.txt";
	std::ofstream outfile("C:/test2.txt");
	outfile << arr << std::endl;
	outfile.close();

	std::fstream file_buffer;
	file_buffer.open("C:/test2.txt", std::ios::in | std::ios::out | std::ios::binary | std::ios::ate);
	if (file_buffer.is_open())
	{
		fileMessage *filemessage2 = new fileMessage(id, file_buffer, arr, sizeof(arr));
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
	char id = 5;
	char arr[10] = "test3.txt";
	std::ofstream outfile("C:/test3.txt");
	outfile << arr << std::endl;
	outfile.close();

	std::fstream file_buffer;
	file_buffer.open("C:/test3.txt", std::ios::in | std::ios::out | std::ios::binary | std::ios::ate);
	file_buffer.seekg(0, file_buffer.end);
	int size = file_buffer.tellg();
	file_buffer.seekg(0, std::ios::beg);
	if (file_buffer.is_open())
	{
		fileMessage *filemessage3 = new fileMessage(id, file_buffer, arr, sizeof(arr));

		char * temp_arr = new char[size + 9];
		char * tmp = new char[size];

		temp_arr[0] = id;
		temp_arr[1] = (1 >> 24) & 0xFF;
		temp_arr[2] = (1 >> 16) & 0xFF;
		temp_arr[3] = (1 >> 8) & 0xFF;
		temp_arr[4] = 1 & 0xFF;
		temp_arr[5] = (1 >> 24) & 0xFF;
		temp_arr[6] = (1 >> 16) & 0xFF;
		temp_arr[7] = (1 >> 8) & 0xFF;
		temp_arr[8] = 1 & 0xFF;
		file_buffer.read(tmp, size);
		memcpy(temp_arr + 9, tmp, size);
		BOOST_CHECK_EQUAL(filemessage3->sendPackage(1), temp_arr);
		delete temp_arr;
		delete tmp;
		delete filemessage3;
	}
	else
		BOOST_ERROR("file does not load");


}

BOOST_AUTO_TEST_CASE(sendFileName_test)
{
	char id = 5;
	char arr[10] = "test4.txt";
	std::ofstream outfile("C:/test4.txt");
	outfile << arr << std::endl;
	outfile.close();

	fileMessage *filemessage4 = new fileMessage(id, "C:/test4.txt", arr, sizeof(arr));

	char *arr2 = new char[9 + sizeof(arr)];
	arr2[0] = 12;
	arr2[1] = (sizeof(arr) >> 24) & 0xFF;
	arr2[2] = (sizeof(arr) >> 16) & 0xFF;
	arr2[3] = (sizeof(arr) >> 8) & 0xFF;
	arr2[4] = sizeof(arr) & 0xFF;
	arr2[5] = (1 >> 24) & 0xFF;
	arr2[6] = (1 >> 16) & 0xFF;
	arr2[7] = (1 >> 8) & 0xFF;
	arr2[8] = 1 & 0xFF;
	memcpy(arr2 + 9, arr, sizeof(arr));
	BOOST_CHECK_EQUAL(filemessage4->sendFileInfo(), arr2);
}
