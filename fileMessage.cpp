// Message
// Autor: Lukasz Rombel
#include <string.h>
#include <fstream>
#include <iostream>
//To Do: exception handling and tests

//class that loads file, splits into smaller chunks/packages and sends file, one package at the time
//packages are labeled from 1 to n

class fileMessage {
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
	fileMessage(char id, const char * file_path, char * file_name, int file_name_size)
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
				buffer_code = new char [size];
				file_buffer.seekg(0, std::ios::beg);
				file_buffer.read(buffer_code, size);

				//initialize package_amount and last_package_size
				set_package_amount(((int)(size / package_size) + 1));
				set_last_package_size((int)(size - (package_amount - 1) * package_size));
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
	fileMessage(char id, std::fstream& file_buffer, char * file_name, int file_name_size)
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
				buffer_code = new char[size];
				file_buffer.seekg(0, std::ios::beg);
				file_buffer.read(buffer_code, size);

				//initialize package_amount and last_package_size
				set_package_amount(((int)(size / package_size) + 1));
				set_last_package_size((int)(size - (package_amount - 1) * package_size));
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
	~fileMessage()
	{
		delete buffer_code;
		if (!file_name)
			delete file_name;
	}
	//getters and setters
	void set_id(char new_id)
	{
		id = new_id;
	}

	char get_id()
	{
		return id;
	}

	void set_package_size(int new_package_size)
	{
		package_size = new_package_size;
	}

	int get_package_size()
	{
		return package_size;
	}

	void set_file_name_size(int new_file_name_size)
	{
		file_name_size = new_file_name_size;
	}

	int get_file_name_size()
	{
		return file_name_size;
	}

	void set_file_name(char new_file_name[])
	{
		if (file_name_size)
		{
			file_name = new char[file_name_size];
			memcpy(file_name, new_file_name, file_name_size);
		}
	}

	char * get_file_name()
	{
		return file_name;
	}

	void set_buffer_code(char new_buffer_code[])
	{
		memcpy(buffer_code, new_buffer_code, get_buffer_size());
	}

	int get_buffer_size()
	{
		return (package_amount - 1) * package_size + last_package_size;
	}

	void set_package_amount(int new_package_amount)
	{
		package_amount = new_package_amount;
	}

	void set_last_package_size(int new_last_package_size)
	{
		last_package_size = new_last_package_size;
	}

	int get_package_amount()
	{
		return package_amount;
	}

	int get_last_package_size()
	{
		return last_package_size;
	}

	//send single data package (packages are labeled from 1 to n)
	char * sendPackage(int packageId)
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

		char * arr = new char[x + 9];
		arr[0] = id;
		arr[1] = (packageId >> 24) & 0xFF;
		arr[2] = (packageId >> 16) & 0xFF;
		arr[3] = (packageId >> 8) & 0xFF;
		arr[4] = packageId & 0xFF;
		arr[5] = (package_amount >> 24) & 0xFF;
		arr[6] = (package_amount >> 16) & 0xFF;
		arr[7] = (package_amount >> 8) & 0xFF;
		arr[8] = package_amount & 0xFF;
		memcpy(arr + 9, buffer_code + ((packageId-1) * package_size), x);
		return arr;
	}
	//send information about loaded file (name size/package amount/name)
	char * sendFileInfo()
	{
		char *arr = new char[9 + file_name_size];
		arr[0] = 12;
		arr[1] = (file_name_size >> 24) & 0xFF;
		arr[2] = (file_name_size >> 16) & 0xFF;
		arr[3] = (file_name_size >> 8) & 0xFF;
		arr[4] = file_name_size & 0xFF;
		arr[5] = (package_amount >> 24) & 0xFF;
		arr[6] = (package_amount >> 16) & 0xFF;
		arr[7] = (package_amount >> 8) & 0xFF;
		arr[8] = package_amount & 0xFF;
		memcpy(arr + 9, file_name , file_name_size);
		return arr;
	}
};













