// Message
// Autor: Lukasz Rombel
#include <string.h>
#include <fstream>
#include <iostream>
//To Do: delete allocated memory

//class that loads file, splits into smaller chunks/packages and sends file, one package at the time
//packages are labeled from 1 to n

class fileMessage {
private:
	
	int package_size = 512;
	int package_amount = 0;
	int last_package_size = 0;
	char id;
	char ** packaged_code;

public:
	//constructors
	fileMessage(char id, const char * file_path)
	{
		set_id(id);
		std::fstream file_buffer;
		file_buffer.open(file_path, std::ios::in | std::ios::out | std::ios::binary);

		try {
			if (file_buffer.is_open())
			{
				//initialize temporary array buffer that reads file
				file_buffer.seekg(0, file_buffer.end);
				int size = file_buffer.tellg();
				char * temp_arr = new char[size];
				file_buffer.seekg(0, std::ios::beg);
				file_buffer.read(temp_arr, size);

				//initialize package_amount and last_package_size
				set_package_amount(((int)(size / package_size) + 1));
				set_last_package_size((int)(size - (package_amount - 1) * package_size));

				//initialize packaged_code pointer array
				packaged_code = new char *[package_amount];
				for (int i = 0; i < package_amount; i++)
					packaged_code[i] = new char[package_size + 9];

				//split package into smaller chunks
				for (int i = 0; i * package_size < size; i++)
				{
					packaged_code[i][0] = id;
					packaged_code[i][1] = ((i + 1) >> 24) & 0xFF;
					packaged_code[i][2] = ((i + 1) >> 16) & 0xFF;
					packaged_code[i][3] = ((i + 1) >> 8) & 0xFF;
					packaged_code[i][4] = (i + 1) & 0xFF;
					packaged_code[i][5] = (package_amount >> 24) & 0xFF;
					packaged_code[i][6] = (package_amount >> 16) & 0xFF;
					packaged_code[i][7] = (package_amount >> 8) & 0xFF;
					packaged_code[i][8] = package_amount & 0xFF;
					//copy from temporary buffer array
					if ((i + 1) * package_size < size)
						memcpy(packaged_code[i] + 9, temp_arr + (i*get_package_size()), package_size);
					else
						memcpy(packaged_code[i] + 9, temp_arr + (i*get_package_size()), last_package_size);
				}
				//delete temporary buffer array
				delete temp_arr;
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

	fileMessage(char id, std::fstream& file_buffer)
	{
		set_id(id);
		try {
			if (file_buffer.is_open())
			{
				//create temporary array buffer that reads file
				file_buffer.seekg(0, file_buffer.end);
				int size = file_buffer.tellg();
				char * temp_arr = new char[size];
				file_buffer.seekg(0, std::ios::beg);
				file_buffer.read(temp_arr, size);

				//initialize package_amount and last_package_size
				set_package_amount(((int)(size / package_size) + 1));
				set_last_package_size((int)(size - (package_amount - 1) * package_size));

				//initialize packaged_code pointer array
				packaged_code = new char *[package_amount];
				for (int i = 0; i < package_amount; i++)
					packaged_code[i] = new char[package_size + 9];
				
				//split package into smaller chunks
				for (int i = 0; i * package_size < size; i++)
				{
					packaged_code[i][0] = id;
					packaged_code[i][1] = ((i + 1) >> 24) & 0xFF;
					packaged_code[i][2] = ((i + 1) >> 16) & 0xFF;
					packaged_code[i][3] = ((i + 1) >> 8) & 0xFF;
					packaged_code[i][4] = (i + 1) & 0xFF;
					packaged_code[i][5] = (package_amount >> 24) & 0xFF;
					packaged_code[i][6] = (package_amount >> 16) & 0xFF;
					packaged_code[i][7] = (package_amount >> 8) & 0xFF;
					packaged_code[i][8] = package_amount & 0xFF;
					//copy from temporary buffer array
					if ((i + 1) * package_size < size)
						memcpy(packaged_code[i] + 9, temp_arr + (i*package_size), package_size);
					else
						memcpy(packaged_code[i] + 9, temp_arr + (i*package_size), last_package_size);
				}
				//delete temporary buffer array
				delete temp_arr;
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
		for (int i = 0; i < get_package_amount(); i++)
			delete[] packaged_code[i];
		delete[] packaged_code;
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

	void set_packaged_code(char ** new_packaged_code)
	{
		for (int i = 0; i < get_package_amount(); i++)
			memcpy(packaged_code[i], new_packaged_code[i], get_package_size() + 9);
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
		if (packageId < get_package_amount())
		{
			char * arr = new char[9 + get_package_size()];
			memcpy(arr, packaged_code[packageId - 1], (9 + get_package_size()));
			return arr;
		}
		else if (packageId == get_package_amount())
		{
			char * arr = new char[9 + get_last_package_size()];
			memcpy(arr, packaged_code[packageId - 1], (9 + get_last_package_size()));
			return arr;
		}
		else
		{
			return nullptr;
		}
	}

};




