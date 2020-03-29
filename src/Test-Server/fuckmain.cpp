#define _HAS_AUTO_PTR_ETC = 1
#define _CRT_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_WARNINGS
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS

#include "stardust_networking.h"

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

int main(int argc, char *argv[])
{
    int socket_desc, client_sock, c, read_size;
    struct sockaddr_in server, client;
    char client_message[2000] = {0};

    //Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(35700);

    //Bind
    if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");

    //Listen
    listen(socket_desc, 3);

    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);

    //accept connection from an incoming client
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c);
    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
    puts("Connection accepted");

    //Receive a message from client
    //int recv_a = 0;
    //if (recv_a = recv(client_sock, client_message, 2000, 0) > 0)
    //   write(client_sock, client_message, strlen(client_message)); //Send the message back to client

    using namespace Stardust::Network;
    PacketIn pIn;

    std::vector<byte> len;
    byte newByte;
    recv(client_sock, &newByte, 1, 0);

    while (newByte & 128)
    {
        len.push_back(newByte);
        recv(client_sock, &newByte, 1, 0);
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



    pIn.ID = decodeShort(pIn);

    std::cout << "POS: " << pIn.pos << std::endl;

    
    

    //Utilities::detail::core_Logger->log("Received Packet!", Utilities::LOGGER_LEVEL_DEBUG);
    //Utilities::detail::core_Logger->log("Packet ID: " + std::to_string(pIn.ID), Utilities::LOGGER_LEVEL_DEBUG);

    std::cout << std::to_string(pIn.ID) << std::endl;
    std::cout << std::string(pIn.bytes.begin(), pIn.bytes.end()) << std::endl;
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
    printf("\n");


    printf("hashedChars: ");
    for (int i = 0; i < endByteBuffer.size(); i++)
    {
        printf("%x ", endByteBuffer[i]);
    }
    printf("\n");

    //Send over socket
    if (send(client_sock, endByteBuffer.data(), endByteBuffer.size() + 1, 0) > 0)
        std::cout << "yes" << std::endl;

    close(client_sock);

    puts("Disconnecting and closing");

    return 0;
}