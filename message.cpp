// Message
// Autor: Lukasz Rombel

#include <iostream>
#include <algorithm>
#include <string>

using namespace std;

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












