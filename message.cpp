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

    //constructors
    Message (char id)
    {
        set_id(id);
        msg.clear();
    }
    Message (char id, string msg)
    {
        set_id(id);
        set_msg(msg);
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
    string get_code()
    {
        string arr;
        arr.push_back(id);
        int n = msg.length();
        if(n>0){
            for(int i=0;i<n;i++) {
                arr.push_back(msg[i]);
            }
        }
        return arr;
    }

};






