// Message
// Autor: Lukasz Rombel
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
		int x = get_msg_size();
		memcpy(msg, new_msg, msg_size);
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

class Test_ack_message : public Message {
private:
	char id;
	char num;
public:
	Test_ack_message(char num) : Message(TEST_ACK)
	{
		set_num(num);
	}

	void set_num(char new_num)
	{
		num = new_num;
	}

	char get_num()
	{
		return num;
	}

	char * get_code()
	{
		char *arr = new char[2];
		arr[0] = get_id();
		arr[1] = get_num();
		return arr;
	}
};


