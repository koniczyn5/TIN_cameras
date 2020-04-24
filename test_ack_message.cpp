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
    };
    void set_num (char new_num)
    {
        num=new_num;
    }

    char get_num()
    {
        return num;
    }
    string get_code()
    {
        string arr;
        arr.push_back(id);
        arr.push_back(num+48);
        return arr;
    }
};
