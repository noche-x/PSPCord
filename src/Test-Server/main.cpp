#define _HAS_AUTO_PTR_ETC = 1
#define _CRT_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_WARNINGS
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS

#include <aegis.hpp>
#include "stardust_networking.h"
#include "lib/argh/include/argh.h"
#include "lib/json/include/nlohmann/json.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <limits>
#include <string>
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
    LOGIN_PACKET = 301,
    SEND_MESSAGE_PACKET = 302,
};

aegis::core bot(aegis::create_bot_t().log_level(spdlog::level::trace).token("NjI2MDgyMDczOTgzMzIwMDY0.Xn0Y7Q.bwfKkmJ0Rop_8Daac4ZqSq3uVbY"));

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

    bot.create_message(647466644385300500, fmt::format("<{}> {}", usr, msg));

    // std::cout << std::to_string(pIn.ID) << std::endl;
    // std::cout << std::string(pIn.bytes.begin(), pIn.bytes.end()) << std::endl;
    std::cout << std::endl;

    PacketOut *new_packet = new PacketOut;
    new_packet->ID = pIn.ID;
    new_packet->bytes = pIn.bytes;

    std::vector<byte> endByteBuffer;

    int packetLength = new_packet->bytes.size();

    //Header
    encodeVarInt(packetLength, endByteBuffer);
    encodeShort(new_packet->ID, endByteBuffer);

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

    //std::this_thread::sleep_for(2s);

    //Send over socket
    //if (send(client_sock, endByteBuffer.data(), endByteBuffer.size() + 1, 0) > 0)
    //    std::cout << "yes" << std::endl;
    //IF(send(client_sock, endByteBuffer.data(), endByteBuffer.size() + 1, 0) > 0, "sent the package", "send failed", 1);

    close(client_sock);
    return NULL;
}

int main(int argc, char *argv[])
{
    argh::parser cmdl(argv);
    using jsonf = nlohmann::json;

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

    std::ifstream file("database.json");
    if (!file.good())
    {
        jsonf jsonfile;
        jsonfile["users"]["noche"]["pass"] = "123";
        std::ofstream of("database.json");
        of << jsonfile;
        of.flush();
        of.close();
    }

    bot.set_on_message_create([](aegis::gateway::events::message_create obj) {
        using embed = aegis::gateway::objects::embed;
        using field = aegis::gateway::objects::field;
        if (obj.get_user().is_bot())
            return;
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
    });
    bot.run();
    //bot.yield();
    bot.update_presence("testing fuck");

    using namespace std::chrono_literals;
    std::this_thread::sleep_for(5s);
    bot.update_presence("beep boop");
    bot.create_message(647466644385300500, "BOT HAS BEEN STARTED\nNOW LET THE PSP'S COME!!!!!!");



    // for (size_t i = 0; i < 30; i++)
    // {
    //     using namespace std::chrono_literals;
    //     bot.create_message(647466644385300500, fmt::format("test {}", i));
    //     std::this_thread::sleep_for(2s);
    // }

    //auto channel = bot.find_channel(aegis::snowflake(647466644385300500));
    // auto is_connected = bot.get_shard_mgr().get_shard(1).is_connected();
    // //auto is_online = bot.get_shard_mgr().get_shard(1).is_online();
    // if (is_connected)
    //     bot.create_message(647466644385300500, "started");
    // else
    //     std::this_thread::sleep_for(2s);
    // for (size_t i = 0; i < 30; i++)
    // {
    //     using namespace std::chrono_literals;
    //     bot.create_message(647466644385300500, "started");
    //     std::this_thread::sleep_for(2s);
    // }

    /*

    while (running)
    {
        //Accept and incoming connection
        INFO("waiting for connections...");

        c = sizeof(struct sockaddr_in);

        //accept connection from an incoming client
        IF(accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c) < 0, "connection accepted", "accept failed", 1);

        using namespace Stardust;
        Network::PacketIn pIn;

        std::vector<Network::byte> len;
        Network::byte newByte;
        recv(client_sock, &newByte, 1, 0);

        while (newByte & 128)
        {
            len.push_back(newByte);
            recv(client_sock, &newByte, 1, 0);
        }
        len.push_back(newByte);

        //We now have the length stored in len
        int length = Network::decodeVarInt(len);

        int totalTaken = 0;

        Network::byte *b = new Network::byte[length];
        totalTaken += recv(client_sock, b, length, 0);

        for (int i = 0; i < length; i++)
        {
            pIn.bytes.push_back(b[i]);
        }

        pIn.pos = 0;

        pIn.ID = Network::decodeShort(pIn);
        std::string usr, psw;
        usr = Network::decodeString(pIn);
        psw = Network::decodeString(pIn);

        INFO(fmt::format("usr: {}", usr));
        INFO(fmt::format("psw {}", psw));

        if (pIn.ID == PacketIDS::LOGIN_PACKET)
        {
            std::ifstream ref("database.json", std::ios::binary | std::ios::ate);
            if (ref.good() && ref.tellg())
            {
                jsonf ja;
                ref >> ja;
                if (ja["users"][usr]["pass"] == psw)
                    INFO(fmt::format("{}'s password matches", usr));
                ref.close();
            }
        }

        //Utilities::detail::core_Logger->log("Received Packet!", Utilities::LOGGER_LEVEL_DEBUG);
        //Utilities::detail::core_Logger->log("Packet ID: " + std::to_string(pIn.ID), Utilities::LOGGER_LEVEL_DEBUG);

        std::cout << std::to_string(pIn.ID) << std::endl;
        std::cout << std::string(pIn.bytes.begin(), pIn.bytes.end()) << std::endl;
        std::cout << std::endl;

        Network::PacketOut *new_packet = new Network::PacketOut;
        new_packet->ID = pIn.ID;
        new_packet->bytes = pIn.bytes;

        std::vector<Network::byte> endByteBuffer;

        int packetLength = new_packet->bytes.size();

        //Header
        Network::encodeVarInt(packetLength, endByteBuffer);
        Network::encodeShort(new_packet->ID, endByteBuffer);

        //Add body
        for (int x = 2; x < new_packet->bytes.size(); x++)
        {
            endByteBuffer.push_back(new_packet->bytes[x]);
        }
        std::cout << std::endl;

        printf("hashedChars: ");
        for (int i = 0; i < endByteBuffer.size(); i++)
        {
            printf("%x ", endByteBuffer[i]);
        }
        std::cout << std::endl;

        //Send over socket
        if (send(client_sock, endByteBuffer.data(), endByteBuffer.size() + 1, 0) > 0)
            std::cout << "yes" << std::endl;
        
        std::chrono::time_point last_send_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = std::chrono::high_resolution_clock::now()-last_send_time;
        if (elapsed.count() > 2000)
            bot.create_message(647466644385300500, usr);


        //int rec;
        //if (rec = recv(client_socket, recvbuf, 4096, 0) > 0)
        //	LOG(fmt::format("recieved {} bytes.", rec));

        //LOG(fmt::format("recieved {} bytes.", recvbuf));
        //if (rec > 0 && recvbuf != "") {
        //	bot.create_message(647466644385300500, recvbuf);
        //	bot.create_message(647466644385300500, recvbuf);
        //	bot.create_message(647466644385300500, recvbuf);
        //	bot.create_message(647466644385300500, recvbuf);
        //	bot.create_message(647466644385300500, recvbuf);
        //	bot.create_message(647466644385300500, recvbuf);
        //	bot.create_message(647466644385300500, recvbuf);
        //	bot.create_message(647466644385300500, recvbuf);
        //	bot.create_message(647466644385300500, recvbuf);
        //	bot.create_message(647466644385300500, recvbuf);
        //	bot.create_message(647466644385300500, recvbuf);
        //	bot.create_message(647466644385300500, recvbuf);
        //	bot.create_message(647466644385300500, recvbuf);
        //}

        //int sent_amount;
        //IF((sent_amount = send(client_socket, recvbuf, rec, 0)) == SOCKET_ERROR, "", "send failed...", 1);
        //
        //LOG(fmt::format("sent {} bytes.", sent_amount));
    }

    close(client_sock);
    puts("Disconnecting and closing");
*/
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
