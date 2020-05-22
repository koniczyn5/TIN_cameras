// Message
// Autor: Lukasz Rombel
#include <string.h>
#include <fstream>
//To Do: delete allocated memory

//class that loads file, splits into smaller chunks/packages and sends file, one package at the time
//packages are labeled from 1 to n

class fileMessage {
private:
	
	int package_size = 512;
	int buffer_size = 0;
	int package_amount = 0;
	int last_package_size = 0;
	char id = '\0';
	char * file_code;
	char ** packaged_code;
public:

	//constructors
	fileMessage(char id, const char * file_path)
	{
		set_id(id);
		fstream file_buffer;
		file_buffer.open(file_path, ios::in | ios::out | ios::binary);

		try {
			if (file_buffer.is_open())
			{
				convertFstreamToCharArray(file_buffer);
				packageValues();
				packageFileCode();
			}
			else
				throw "File is closed or nonexsistent";
		}
		catch (const char * throwmsg)
		{
			cerr << throwmsg << endl;
		}
		file_buffer.close();
	}

	fileMessage(char id, fstream& file_buffer)
	{
		set_id(id);
		try {
			if (file_buffer.is_open())
			{
				convertFstreamToCharArray(file_buffer);
				packageValues();
				packageFileCode();
			}
			else
				throw "File is closed or nonexsistent";
		}
		catch(const char * throwmsg)
		{
			cerr << throwmsg << endl;
		}
		file_buffer.close();
	}
	//Destructor
	/*~fileMessage()
	{
		for (int i = 0; i < get_package_amount(); i++)
			delete[] packaged_code[i];
		delete[] packaged_code;
		delete file_code;
	}*/
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

	void set_buffer_size(int new_buffer_size)
	{
		buffer_size = new_buffer_size;
	}

	int get_buffer_size()
	{
		return buffer_size;
	}

	void set_packaged_code(char ** new_packaged_code)
	{
		packaged_code = new char *[get_package_amount()];
		for (int i = 0; i < get_package_amount(); i++)
			packaged_code[i] = new char[get_package_size() + 9];
		for (int i = 0; i < get_package_amount(); i++)
			memcpy(packaged_code[i], new_packaged_code[i], get_package_size() + 9);
	}

	void set_file_code(char new_file_code[])
	{
		file_code = new char[get_buffer_size()];
		memcpy(file_code, new_file_code, get_buffer_size());
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
	//methods
	//convert fstream into char *
	void convertFstreamToCharArray(fstream& file_buffer)
	{
		file_buffer.seekg(0, file_buffer.end);
		int size = file_buffer.tellg();
		char * arr = new char[size];
		file_buffer.seekg(0, std::ios::beg);
		file_buffer.read(arr, size);
		set_buffer_size(size);
		set_file_code(arr);
		delete arr;
	}
	//initialize values based on uploaded file
	void packageValues()
	{
		set_package_amount(((int)(buffer_size / package_size) + 1));
		set_last_package_size((int)(buffer_size - (package_amount - 1)* package_size));
	}
	//split file code into smaller packages (packages are labeled from 1 to n)
	void packageFileCode()
	{
		//allocate the array
		char ** arr = new char*[get_package_amount()];
		for (int i = 0; i < get_package_amount(); i++)
			arr[i] = new char[get_package_size()+9];
		char temparr[521];											//irytuje mnie c++
		for (int i = 0; i * package_size < buffer_size; i++) 
		{	
			temparr[0] = id;
			temparr[1] = ((i + 1) >> 24) & 0xFF;
			temparr[2] = ((i + 1) >> 16) & 0xFF;
			temparr[3] = ((i + 1) >> 8) & 0xFF;
			temparr[4] = (i + 1) & 0xFF;
			temparr[5] = (package_amount >> 24) & 0xFF;
			temparr[6] = (package_amount >> 16) & 0xFF;
			temparr[7] = (package_amount >> 8) & 0xFF;
			temparr[8] = package_amount & 0xFF;
			if ((i + 1) * package_size < buffer_size)
				memcpy(temparr + 9, file_code + (i*get_package_size()), package_size);
			else
				memcpy(temparr + 9, file_code + (i*get_package_size()), last_package_size);
			arr[i] = temparr;
		}
		set_packaged_code(arr);
		/*for (int i = 0; i < get_package_amount(); i++)
			delete[] arr[i];
		delete[] arr;*/
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
