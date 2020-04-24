// Message
// Autor: Lukasz Rombel
#include "message.cpp"

class Test_req_message: public Message {
    private:
        char id;
    public:
    Test_req_message (char id) : Message (id) {};

};
