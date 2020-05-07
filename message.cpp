// Message
// Autor: Lukasz Rombel

using namespace std;

enum Message_type {
	EMPTY_MSG,
	TEST_REQ,
	TEST_ACK,
	CONF_REQ, 
	CONF_ACK
};

class Message {
    private:
        char id;
        char *msg;
    public:

    //constructors
    Message (char id)
    {
        set_id(id);
        this->msg = NULL;
    }
    Message (char id, char *msg)
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

    void set_msg (char * new_msg)
    {
        msg=new_msg;
    }

    char * get_msg()
    {
        return msg;
    }

    char * get_code()
    {
        char *arr = new char[1+sizeof(msg)];
        arr[0] = get_id();
        memcpy(arr+1,msg,sizeof(msg));
        return arr;
    }
};

class Test_ack_message: public Message {
    private:
        char id;
        char num;
    public:
    Test_ack_message (char num) : Message (TEST_ACK)
    {
        set_num(num);
    }

    void set_num (char new_num)
    {
        num=new_num;
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




