// Message
// Autor: Lukasz Rombel

#include <iostream>
#include <algorithm>
#include <string>

using namespace std;

enum Message_type {
	EMPTY_MSG,
	TEST_REQ,
	TEST_ACK
};

class Message {
    private:
        char id;
        string msg;
    public:

    //constructor
    Message (char id, string msg)
    {
        this->id = id;
        this->msg = msg;
    };

    void set_id (char new_id)
    {
	id=new_id;
    }

    char get_id()
    {
	return id;
    }

    void set_msg (string new_msg)
    {
	msg=new_msg;
    }

    string get_msg()
    {
	return msg;
    }

    //id + message
    char * get_code()
    {
        int n = msg.length();
        char arr[n+2];
        arr[0] = id;
        for(int i=0;i<n;i++) {
            arr[i+1] = msg[i];
        }
        return arr;
    }

};
