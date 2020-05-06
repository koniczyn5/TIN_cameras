// Message
// Autor: Lukasz Rombel
#include "message.cpp"

class Test_ack_message: public Message {
    private:
        char id;
        int num;
    public:
    Test_ack_message (char id, int num) : Message (id)
    {
        set_num(num);
        set_length(2);
    };
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
        char *arr = new char[get_length()];
        arr[0] = get_id();
        arr[1] = get_num()+48;
        return arr;
    }
};

