// fileMessage.cpp
// Projekt:	Camera-Link
// Autor: Lukasz Rombel
// Data utworzenia: 18.05.2020
#include <string.h>
#include "fileMessage.h"
#include <fstream>
#include <iostream>

//class that loads file, splits into smaller chunks/packages and sends file, one package at the time
//packages are labeled from 1 to n

const int package_size = 0;

FileMessage::FileMessage(char id, const char * file_path, char * file_name, int file_name_size)
{
	set_id(id);
	set_file_name_size(file_name_size);
	set_file_name(file_name);
	std::fstream file_buffer;
	file_buffer.open(file_path, std::ios::in | std::ios::out | std::ios::binary | std::ios::ate);
	try {
		if (file_buffer.is_open())
		{
			//copy buffer to buffer_code
			file_buffer.seekg(0, file_buffer.end);
			int size = file_buffer.tellg();
			if (size != 0)
			{
				buffer_code = new char[size];
				file_buffer.seekg(0, std::ios::beg);
				file_buffer.read(buffer_code, size);

				//initialize package_amount and last_package_size
				set_package_amount(((int)(size / package_size) + 1));
				set_last_package_size((int)(size - (package_amount - 1) * package_size));
			}
			else
				throw "Empty file";
		}
		else
			throw "File is closed or nonexsistent";
	}
	catch (const char * throwmsg)
	{
		std::cerr << throwmsg << std::endl;
	}
	file_buffer.close();
}
//constructor with buffer stream as argument
FileMessage::FileMessage(char id, std::fstream& file_buffer, char * file_name, int file_name_size)
{
	set_id(id);
	set_file_name_size(file_name_size);
	set_file_name(file_name);
	try {
		if (file_buffer.is_open())
		{
			//copy buffer to buffer_code
			file_buffer.seekg(0, file_buffer.end);
			int size = file_buffer.tellg();
			if (size != 0)
			{
				buffer_code = new char[size];
				file_buffer.seekg(0, std::ios::beg);
				file_buffer.read(buffer_code, size);

				//initialize package_amount and last_package_size
				set_package_amount(((int)(size / package_size) + 1));
				set_last_package_size((int)(size - (package_amount - 1) * package_size));
			}
			else
				throw "Empty file";
		}
		else
			throw "File is closed or nonexsistent";
	}
	catch(const char * throwmsg)
	{
		std::cerr << throwmsg << std::endl;
	}
	file_buffer.close();
}
//Destructor
FileMessage::~FileMessage()
{
	delete buffer_code;
	if (!file_name)
		delete file_name;
}
//getters and setters
void FileMessage::set_id(char new_id)
{
	id = new_id;
}

char FileMessage::get_id()
{
	return id;
}

void FileMessage::set_package_size(int new_package_size)
{
	package_size = new_package_size;
}

void FileMessage::set_file_name_size(int new_file_name_size)
{
	file_name_size = new_file_name_size;
}

int FileMessage::get_file_name_size()
{
	return file_name_size;
}

void FileMessage::set_file_name(char new_file_name[])
{
	if (file_name_size)
	{
		file_name = new char[file_name_size];
		memcpy(file_name, new_file_name, file_name_size);
	}
}

char * FileMessage::get_file_name()
{
	return file_name;
}

void FileMessage::set_buffer_code(char new_buffer_code[])
{
	memcpy(buffer_code, new_buffer_code, get_buffer_size());
}

int FileMessage::get_buffer_size()
{
	return (package_amount - 1) * package_size + last_package_size;
}

void FileMessage::set_package_amount(int new_package_amount)
{
	package_amount = new_package_amount;
}

void FileMessage::set_last_package_size(int new_last_package_size)
{
	last_package_size = new_last_package_size;
}

int FileMessage::get_package_amount()
{
	return package_amount;
}

int FileMessage::get_last_package_size()
{
	return last_package_size;
}

//send single data package (packages are labeled from 1 to n)
char * FileMessage::sendPackage(int packageId)
{
	int x;
	if (packageId < get_package_amount())
		x = get_package_size();
	else if (packageId == get_package_amount())
		x = get_last_package_size();
	else
	{
		x = 0;
		return nullptr;
	}
	//package attributes
	char * arr = new char[x + 9];
	arr[0] = id;
	*(int*)&arr[1] = packageId;
	*(int*)&arr[5] = package_amount;
	//package data
	memcpy(arr + 9, buffer_code + ((packageId-1) * package_size), x);
	return arr;
}
//send information about loaded file (name size/package amount/name)
char * FileMessage::sendFileInfo()
{
	//file name attributes
	char *arr = new char[9 + file_name_size];
	arr[0] = DATA_HDR;
	*(int*)&arr[1] = file_name_size;
	*(int*)&arr[5] = package_amount;
	*(int*)&arr[9] = last_package_size;
	//file name data
	memcpy(arr + 13, file_name , file_name_size);
	return arr;
}
