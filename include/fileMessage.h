#pragma once
// fileMessage.h
// Projekt:	Camera-Link
// Autor: Adam Bieniek
// Data utworzenia: 25.05.2020
#include <string.h>
#include "message.h"
#include <fstream>
#include <iostream>

//class that loads file, splits into smaller chunks/packages and sends file, one package at the time
//packages are labeled from 1 to n

class FileMessage {
private:
	
	int package_size = 512;
	int package_amount = 0;
	int last_package_size = 0;
	char id;
	char * buffer_code;
	char * file_name;
	int file_name_size;

public:
	//constructor with filepath as argument
	FileMessage(char id, const char * file_path, char * file_name, int file_name_size);
	//constructor with buffer stream as argument
	FileMessage(char id, std::fstream& file_buffer, char * file_name, int file_name_size);
	//Destructor
	~FileMessage();
	//getters and setters
	void set_id(char new_id);
	char get_id();
	void set_package_size(int new_package_size);
	int get_package_size();
	void set_file_name_size(int new_file_name_size);
	int get_file_name_size();
	void set_file_name(char new_file_name[]);
	char * get_file_name();
	void set_buffer_code(char new_buffer_code[]);
	int get_buffer_size();
	void set_package_amount(int new_package_amount);
	void set_last_package_size(int new_last_package_size);
	int get_package_amount();
	int get_last_package_size();
	//send single data package (packages are labeled from 1 to n)
	char * sendPackage(int packageId);
	//send information about loaded file (name size/package amount/name)
	char * sendFileInfo();
};