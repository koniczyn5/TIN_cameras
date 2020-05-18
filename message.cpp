// Message
// Autor: Lukasz Rombel

using namespace std;

enum Message_type {
	EMPTY_MSG,
	TEST_REQ,
	TEST_ACK,
    CONF_REQ,
    CONF_ACK,
    INST_REQ,
    INST_HASH,
    INST_ACK,
    DISC_REQ,
    DISC_ACK,
    NO_PAIR,
    IS_PAIR
};

class Message {
    private:
        char id;
        char * msg;
        //char * code;
        int length;
    public:

    //constructors
    Message (char id)
    {
        set_id(id);
        this->msg = nullptr;
        this->length = 1;
    }
    Message (char id, char * msg, int length)
    {
        set_id(id);
        set_msg(msg);
        set_length(length);
        //coding();
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

    void set_length (int new_length)
    {
        length=new_length + 1;
    }

    int get_length()
    {
        return length;
    }

    char * get_code()
    {
        char *arr = new char[length];
        arr[0] = get_id();
        int n = length - 1;
        if(n>0){
            for(int i=0;i<n;i++) {
                arr[i+1] = msg[i];
            }
        }
        return arr;
    }
};






