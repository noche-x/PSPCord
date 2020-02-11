

class PacketManager
{
private:
    /* data */
public:
    PacketManager(/* args */);
    ~PacketManager();

    void LoginPacket(const char* username, const char* password) {
        
    }
};


/*

LOGIN PACKET

SEND 
username
password        | hash the passwords

RECIEVE
server ids

*/

/*

GET CHANNELS PACKET

SEND
server id

RECIEVE 
channel ids

*/

/*

GET MESSAGES FROM X CHANNEL PACKET

SEND
channel id

RECIEVE
messages

*/

/*

SEND MESSAGE PACKET

SEND
user
message

RECIEVE
sent or no

*/

/*
MESSAGE STRUCT
user, date, message
*/
