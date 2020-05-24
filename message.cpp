// Message
// Projekt:	Camera-Link
// Autor: �ukasz Rombel
// Data utworzenia: 02.05.2020
#include <string.h>

enum Message_type {
	TEST_REQ,
	TEST_ACK,
	CONF_REQ,
	CONF_ACK,
	INST_REQ,
	INST_HASH,
	INST_ACK,
	DISC_REQ,
	DISC_ACK,
	DATA_MSG,
	DATA_RQT,
	DATA_ACK,
	DATA_HDR,
	NO_PAIR,
	IS_PAIR
};

class Message {
private:
	char id;
	int msg_size;
	char * msg;

public:

	//constructors
	Message(char id)
	{
		set_id(id);
		int msg_size = 0;
		msg = nullptr;
	}
	Message(char new_id, char new_msg[], int new_msg_size)
	{
		set_id(new_id);
		set_msg_size(new_msg_size);
		char * arr = new char[get_msg_size()];
		msg = arr;
		set_msg(new_msg);
	};
	//Destructor
	~Message()
	{
		delete msg;
	}
	//getters and setters
	void set_id(char new_id)
	{
		this->id = new_id;
	}

	char get_id()
	{
		return id;
	}

	void set_msg_size(char new_msg_size)
	{
		this->msg_size = new_msg_size;
	}

	char get_msg_size()
	{
		return msg_size;
	}

	void set_msg(char new_msg[])
	{
		memcpy(msg, new_msg, get_msg_size());
	}
	//methods
	char * get_code()
	{
		char *arr = new char[1 + msg_size];
		arr[0] = get_id();
		memcpy(arr + 1, msg, msg_size);
		return arr;
	}
};



