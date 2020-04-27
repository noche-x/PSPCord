#define _HAS_AUTO_PTR_ETC = 1
#define _CRT_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_WARNINGS
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS

#include <aegis.hpp>
#include "stardust_networking.h"
#include <argh.h>

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <limits>
#include <string>
#include <thread>
#include <vector>
#include <set>
#include <map>
#include <cassert>

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define LOG(x) std::cout << "[+] " << x << std::endl;
#define INFO(x) std::cout << "[*] " << x << std::endl;
#define ERROR(x) std::cout << "[!!!] " << x << std::endl;

#define IF(statement, conti, error, ret) \
    if (statement)                       \
    {                                    \
        ERROR(error);                    \
        return ret;                      \
    }                                    \
    else if (conti != "")                \
        LOG(conti);

enum PacketIDS
{
    NONE = 0,
    SEND_MESSAGE_PACKET = 302,
    RECV_MESSAGE_PACKET = 303,
};

struct discord_message {
    std::string username;
    std::string message;
    std::vector<std::string> recv_users;
};

std::vector<discord_message> discord_messages;

aegis::core bot(spdlog::level::trace);
long long int bot_channel = 0;

void *handle_connection(void *p_client_socket)
{
    int client_sock = *(int*)p_client_socket;
    free(p_client_socket);

    using namespace Stardust::Network;
    PacketIn pIn;

    std::vector<byte> len;
    byte newByte;
    IF(recv(client_sock, &newByte, 1, 0) == -1, "recieved first part", "recieve first failed", NULL);

    while (newByte & 128)
    {
        len.push_back(newByte);
        IF(recv(client_sock, &newByte, 1, 0) == -1, "", "recieve failed", NULL);
    }
    len.push_back(newByte);

    //We now have the length stored in len
    int length = decodeVarInt(len);

    int totalTaken = 0;

    byte *b = new byte[length];
    totalTaken += recv(client_sock, b, length, 0);

    for (int i = 0; i < length; i++)
    {
        pIn.bytes.push_back(b[i]);
    }

    pIn.pos = 0;
    std::string usr, msg;

    printf("[+] a: ");
    for (int i = 0; i < pIn.bytes.size(); i++)
    {
        printf("%x ", pIn.bytes[i]);
    }
    std::cout << std::endl;
    printf("[+] a: ");
    for (int i = 0; i < pIn.bytes.size(); i++)
    {
        printf("%c ", pIn.bytes[i]);
    }
    std::cout << std::endl;

    pIn.ID = decodeShort(pIn);
    usr = decodeString(pIn);
    msg = decodeString(pIn);

    //std::cout << "POS: " << pIn.pos << std::endl;

    //Utilities::detail::core_Logger->log("Received Packet!", Utilities::LOGGER_LEVEL_DEBUG);
    //Utilities::detail::core_Logger->log("Packet ID: " + std::to_string(pIn.ID), Utilities::LOGGER_LEVEL_DEBUG);
    std::cout << std::endl;
    LOG(fmt::format("packet id: {}", pIn.ID));
    LOG(fmt::format("packet bytes: {}", std::string(pIn.bytes.begin(), pIn.bytes.end())));

    LOG(fmt::format("username: {}", usr));
    LOG(fmt::format("message: {}", msg));

    //usr.replace(usr.begin(), usr.end(), ":", "");
    std::string illegal_chars = ":*_~`>@";
    std::size_t found = usr.find_first_of(illegal_chars);
    if (found != std::string::npos)
        LOG("illegal chars found in username");
    while (found != std::string::npos)
    {
        //usr.replace(found - 1, found, "");
        //usr[found] = '';
        usr.erase(found, 1);
        found = usr.find_first_of(illegal_chars, found);
    }
    found = msg.find_first_of(illegal_chars);
    if (found != std::string::npos)
        LOG("illegal chars found in message");
    while (found != std::string::npos)
    {
        //msg.replace(found, found + 1, "");
        //msg[found] = '';
        msg.erase(found, 1);
        found = msg.find_first_of(illegal_chars, found);
    }
    //msg.replace(msg.begin(), msg.end(), "\\", "\\\\");

    bot.create_message(bot_channel, fmt::format("<{}> {}", usr, msg));

    // std::cout << std::to_string(pIn.ID) << std::endl;
    // std::cout << std::string(pIn.bytes.begin(), pIn.bytes.end()) << std::endl;
    std::cout << std::endl;

    PacketOut *new_packet = new PacketOut;
    new_packet->ID = PacketIDS::RECV_MESSAGE_PACKET;//pIn.ID;
    //new_packet->bytes = pIn.bytes;

    encodeString("fuckyou", *new_packet);
    encodeString("hello sir", *new_packet);
    
    std::vector<byte> endByteBuffer;

    int packetLength = new_packet->bytes.size() + 2;

    //Header
    encodeVarInt(packetLength, endByteBuffer);
    encodeShort(new_packet->ID, endByteBuffer);

    //Add body
    for (int x =0; x < new_packet->bytes.size(); x++)
    {
        endByteBuffer.push_back(new_packet->bytes[x]);
    }

    printf("[+] hashedChars: ");
    for (int i = 0; i < endByteBuffer.size(); i++)
    {
        printf("%x ", endByteBuffer[i]);
    }
    std::cout << std::endl;
    
    //Send over socket
    //if (send(client_sock, endByteBuffer.data(), endByteBuffer.size() + 1, 0) > 0)
    //  std::cout << "yes" << std::endl;
    IF(send(client_sock, endByteBuffer.data(), endByteBuffer.size(), 0) == -1, "sent the packet", "send failed", NULL);

    close(client_sock);
    return NULL;
}

struct messages {
    std::string username;
    std::string message;
};

std::vector<messages> messages_vector;
std::vector<int> connections;

void *send_messages(void*)
{
    LOG(fmt::format("size {}", messages_vector.size()));
    LOG(fmt::format("empty {}", messages_vector.empty()));
    bool statement = !messages_vector.empty();
    if (statement)
    {
        //LOG(fmt::format("content {}", messages_vector.data()));
        Stardust::Network::PacketOut *new_packet = new Stardust::Network::PacketOut;
        Stardust::Network::PacketOut *temp_packet = new Stardust::Network::PacketOut;
        new_packet->ID = PacketIDS::RECV_MESSAGE_PACKET;
        //new_packet->bytes = pIn.bytes;
        int amount_of_messages = 0;
        
        for (auto &&c : messages_vector)
        {
            Stardust::Network::encodeString(c.username, *temp_packet);
            Stardust::Network::encodeString(c.message, *temp_packet);
            amount_of_messages++;
        }
        Stardust::Network::encodeInt(amount_of_messages, *new_packet);
        for (int i = 0; i < temp_packet->bytes.size(); i++)
        {
            new_packet->bytes.push_back(temp_packet->bytes[i]);
        }
        delete temp_packet;        

        std::vector<Stardust::Network::byte> endByteBuffer;

        int packetLength = new_packet->bytes.size();

        //Header
        Stardust::Network::encodeVarInt(packetLength, endByteBuffer);
        Stardust::Network::encodeShort(new_packet->ID, endByteBuffer);

        //Add body
        for (int x = 2; x < new_packet->bytes.size(); x++)
        {
            endByteBuffer.push_back(new_packet->bytes[x]);
        }

        printf("[+] hashedChars: ");
        for (int i = 0; i < endByteBuffer.size(); i++)
        {
            printf("%x ", endByteBuffer[i]);
        }
        std::cout << std::endl;
        for (auto c : connections)
        {
            //Send over socket
            //if (send(client_sock, endByteBuffer.data(), endByteBuffer.size() + 1, 0) > 0)
            //  std::cout << "yes" << std::endl;
            //IF(send(client_sock, endByteBuffer.data(), endByteBuffer.size() + 1, 0) == -1, "sent the package", "send failed", NULL);
            IF(send(c, endByteBuffer.data(), endByteBuffer.size() + 1, 0) == -1, "sent the package", "send failed", NULL);
        }
        messages_vector.clear();
    }
}

std::string close_message = "";

void exit_program(int sig) {
    LOG("Caught SIG_TERM");

    bot.create_message(bot_channel, close_message);

    exit(sig);
}

int main(int argc, char *argv[])
{
    signal(SIGINT, exit_program);  

    argh::parser cmdl(argv);
    using jsonf = nlohmann::json;

    bool use_command_line = false;

    messages_vector.clear();
    connections.clear();

    // std::ifstream arg_val;
    // if (!(cmdl("-om") >> arg_val)) {
    //     LOG("invalid open message");
    //     return 1;
    // }
    // else {
    //     open_message = arg_val;
    //     use_command_line = true;
    // }
    // if (!(cmdl("-bc") >> arg_val) {
    //     open_message = cmdl("-bc").str();
    //     use_command_line = true;
    // }

    if (cmdl[{"-a", "--add-user"}])
    {
        std::string username, password;

        std::cout << "please enter the details of the user," << std::endl;
        std::cout << "username: ";
        std::cin >> username;
        std::cout << "password: ";
        std::cin >> password;

        jsonf j;
        j["users"][username]["pass"] = password;

        std::ofstream of("database.json");
        of << j;
        of.flush();
        of.close();

        return 0;
    }

    bot.set_on_message_create([](aegis::gateway::events::message_create obj) {
        using embed = aegis::gateway::objects::embed;
        using field = aegis::gateway::objects::field;
        if (obj.get_user().is_bot())
            return;
        std::string message = obj.msg.get_content();

        if (message.at(0) == '%') {
            if (obj.msg.get_content() == "%embed_test")
                obj.msg.get_channel().create_message(
                    aegis::create_message_t()
                        .content(fmt::format("test3 {}", obj.msg.author.username))
                        .embed(
                            embed()
                                .color(0xa0a0a0)
                                .title("test2")
                                .description(obj.msg.author.username)
                                .fields({field().name("test1").value(obj.msg.author.username)})));
        }
        else {
            messages m;
            m.username = obj.msg.author.username;
            m.message = message;
            messages_vector.push_back(m);
        }

        discord_message m;
        m.username = obj.get_user().get_username();
        m.message = obj.msg.get_content();
        m.recv_users = {""};

        discord_messages.push_back(m);
    });
    bot.run();
    //bot.yield();
    bot.update_presence("testing fuck");
    std::ifstream config_file("config.json");
    std::string open_message = "";
    if (config_file.is_open())
    {
        jsonf jsonfile;
        config_file >> jsonfile;
        open_message = jsonfile["open-message"];
        close_message = jsonfile["close-message"];
        bot_channel = jsonfile["bot-channel"];
        config_file.close();
    }

    pthread_t send_messages_thread;
    int *apclient = (int *)malloc(sizeof(int));
    *apclient = 1;
    //pthread_create(&send_messages_thread, NULL, send_messages, apclient);

    using namespace std::chrono_literals;
    std::this_thread::sleep_for(5s);
    bot.update_presence("beep boop");
    LOG(open_message);
    LOG(bot_channel);

    int socket_desc, client_sock, c, read_size;
    struct sockaddr_in server, client;
    char client_message[2000] = {0};

    //Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    IF((socket_desc == -1), "socket created", "socket creation failed...", 1);

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(35700);

    //Bind
    IF(bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0, "bind done", "bind failed...", 1);

    //Listen
    IF(listen(socket_desc, 100) != 0, "listening to socket", "listen failed...", 1);

    bot.create_message(bot_channel, open_message);

    bool running = true;

    while (running)
    {
        //INFO("waiting for connections...");
        //c = sizeof(struct sockaddr_in);

        //accept connection from an incoming client
        //IF(accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c) < 0, "connection accepted", "accept failed", 1);

        // //accept connection from an incoming client
        // client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c);
        // if (client_sock < 0)
        // {
        //     perror("accept failed");
        //     return 1;
        // }
        // puts("Connection accepted");

        //Accept and incoming connection
        INFO("waiting for connections...");
        c = sizeof(struct sockaddr_in);

        //accept connection from an incoming client
        // client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c);
        // if (client_sock < 0)
        // {
        //     perror("accept failed");
        //     return 1;
        // }
        // puts("Connection accepted");
        auto client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c);
        IF(client_sock < 0, "connection accepted", "accept failed", 1);

        connections.push_back(client_sock);

        pthread_t thread;
        int *pclient = (int*)malloc(sizeof(int));
        *pclient = client_sock;
        pthread_create(&thread, NULL, handle_connection, pclient);
        
        //Receive a message from client
        //int recv_a = 0;
        //if (recv_a = recv(client_sock, client_message, 2000, 0) > 0)
        //   write(client_sock, client_message, strlen(client_message)); //Send the message back to client
    }

    puts("Disconnecting and closing");
    return 0;
}
