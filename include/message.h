#pragma once
// Message.h
// Projekt:	Camera-Link
// Autor: Adam Bieniek
// Data utworzenia: 25.05.2020
#include <string.h>

enum Message_type {
	EMPT_MSG,
	TEST_REQ,
	TEST_ACK,
	CONF_REQ,
	CONF_ACK,
	INST_REQ,
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
	Message(char id);
	Message(char new_id, char new_msg[], int new_msg_size);
	~Message();
	void set_id(char new_id);
	char get_id();
	void set_msg_size(char new_msg_size);
	char get_msg_size();
	void set_msg(char new_msg[]);
	char * get_code();
};



