// message.cpp
// Projekt:	Camera-Link
// Autor: Lukasz Rombel
// Data utworzenia: 18.04.2020
#include <message.h>

//constructors
Message::Message(char id)
{
	set_id(id);
	int msg_size = 0;
	msg = nullptr;
}

Message::Message(char new_id, char new_msg[], int new_msg_size)
{
	set_id(new_id);
	set_msg_size(new_msg_size);
	char * arr = new char[get_msg_size()];
	msg = arr;
	set_msg(new_msg);
};
//Destructor
Message::~Message()
{
	delete msg;
}
//getters and setters
void Message::set_id(char new_id)
{
	this->id = new_id;
}

char Message::get_id()
{
	return id;
}

void Message::set_msg_size(char new_msg_size)
{
	this->msg_size = new_msg_size;
}

char Message::get_msg_size()
{
	return msg_size;
}

void Message::set_msg(char new_msg[])
{
	memcpy(msg, new_msg, get_msg_size());
}
//methods
char * Message::get_code()
{
	char *arr = new char[1 + msg_size];
	arr[0] = get_id();
	memcpy(arr + 1, msg, msg_size);
	return arr;
}