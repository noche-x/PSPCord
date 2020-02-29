struct message_t
{
    /* data */
    char* username;
    char* date;
    char* message;
};

class packet_manager
{
private:
    /* data */

public:
    packet_manager(/* args */);
    ~packet_manager();

    /*
        @username   : const char* that holds the username
        @password   : const char* that holds the password

        @return     : vector of char* server ids  
    */
    std::vector<char*> login_packet(const char* username, const char* password);

    /*
        @server_id  : const char* the server id which you want the channels recieved
        
        @return     : vector of char* channel ids of the given server id 
    */
    std::vector<char*> get_channels_packet(const char* server_id);

    /*
        @channel_id : const char* of the channel id the messages you want to be recieved

        @return     : vector of message_t which holds the messages of the given channel id
    */
    std::vector<message_t> get_messages_from_channel_packet(const char* channel_id);

    /*
        @channel_id : const char* of the channel id you want to send the message to
        @message    : message_t of the message

        @return     : bool of sent status 
    */
    bool send_message_to_channel(const char* channel_id, message_t message);
};

/*
    VISUALIZATION

    client                          server
    login packet ----------------------->
    <-----------------------server ids/no

    get channels packet ---------------->
    <-------------------------channel ids

    get messages from x channel packet-->
    <----------------------------messages

    send message to x channel----------->
    <-------------------------sent status
*/

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
