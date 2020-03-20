#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <string>

typedef unsigned char byte;

struct PacketOut
{
    short ID;
    std::vector<byte> bytes;
};
struct PacketIn
{
    short ID;
    short int pos;
    std::vector<byte> bytes;
};

inline std::vector<byte> encodeVarInt(int value)
{
    std::vector<byte> vec;

    while (value > 127)
    {
        vec.push_back(((byte)(value & 127)) | 128);

        value >> 7;
    }
    vec.push_back((byte)value & 127);
    return vec;
}

inline void encodeVarInt(int value, std::vector<byte> &p)
{

    while (value > 127)
    {
        p.push_back(((byte)(value & 127)) | 128);

        value >> 7;
    }
    p.push_back((byte)value & 127);
}

inline int decodeVarInt(std::vector<byte> input)
{
    int ret = 0;

    for (int i = 0; i < input.size(); i++)
    {
        ret |= (input[i] & 127) << (7 * i);

        if (!input[i] & 128)
        {
            break;
        }
    }

    return ret;
}

inline int decodeVarInt(PacketIn &p)
{
    int ret = 0;

    for (int i = p.pos; i < p.bytes.size(); i++)
    {
        p.pos++;
        ret |= (p.bytes[i] & 127) << (7 * i);

        if (!p.bytes[i] & 128)
        {
            break;
        }
    }

    return ret;
}
inline void encodeBool(bool v, PacketOut &p)
{
    p.bytes.push_back((byte)v);
}

inline bool decodeBool(PacketIn &p)
{
    return (bool)p.bytes[p.pos++];
}

inline void encodeByte(byte v, PacketOut &p)
{
    p.bytes.push_back(v);
}

inline byte decodeByte(PacketIn &p)
{
    return p.bytes[p.pos++];
}

inline void encodeShort(short int v, PacketOut &p)
{
    p.bytes.push_back((v >> 8) & 0xff);
    p.bytes.push_back(v & 0xff);
}

inline void encodeShort(short int v, std::vector<byte> &p)
{
    p.push_back((v >> 8) & 0xff);
    p.push_back(v & 0xff);
}

inline short int decodeShort(PacketIn &p)
{
    short int res = 0;
    for (int i = 0; i != 2; ++i)
    {
        res |= (byte)p.bytes[p.pos] << (8 - i * 8);
        p.pos++;
    }
    return res;
}

inline void encodeInt(long int v, PacketOut &p)
{
    p.bytes.push_back((v >> 24) & 0xff);
    p.bytes.push_back((v >> 16) & 0xff);
    p.bytes.push_back((v >> 8) & 0xff);
    p.bytes.push_back(v & 0xff);
}

inline long int decodeInt(PacketIn &p)
{
    long int res = 0;
    for (int i = 0; i != 4; ++i)
    {
        res |= (byte)p.bytes[p.pos] << (24 - i * 8);
        p.pos++;
    }
    return res;
}

inline void encodeLong(long long int v, PacketOut &p)
{
    p.bytes.push_back((v >> 56) & 0xff);
    p.bytes.push_back((v >> 48) & 0xff);
    p.bytes.push_back((v >> 40) & 0xff);
    p.bytes.push_back((v >> 32) & 0xff);
    p.bytes.push_back((v >> 24) & 0xff);
    p.bytes.push_back((v >> 16) & 0xff);
    p.bytes.push_back((v >> 8) & 0xff);
    p.bytes.push_back(v & 0xff);
}

inline long long int decodeLong(PacketIn &p)
{
    long long int res = 0;
    for (int i = 0; i != 8; ++i)
    {
        res |= (byte)p.bytes[p.pos] << (56 - i * 8);
        p.pos++;
    }
    return res;
}

inline void encodeString(std::string str, PacketOut &p)
{
    std::vector<byte> prePend = encodeVarInt(str.size());

    for (int i = 0; i < prePend.size(); i++)
    {
        p.bytes.push_back(prePend[i]);
    }

    for (int i = 0; i < str.size(); i++)
    {
        p.bytes.push_back(str[i]);
    }
    p.bytes.push_back((byte)'\0');
}

inline std::string decodeString(PacketIn &p)
{
    std::string res = "";

    int size = decodeVarInt(p);

    for (int i = 0; i < size; i++)
    {
        char a = (char)p.bytes[p.pos++];
        res.append(&a);
    }

    return res;
}

inline void encodeFloat(float v, PacketOut &p)
{
    long int l = reinterpret_cast<long int>(&v);
    p.bytes.push_back((l >> 24) & 0xff);
    p.bytes.push_back((l >> 16) & 0xff);
    p.bytes.push_back((l >> 8) & 0xff);
    p.bytes.push_back(l & 0xff);
}

inline float decodeFloat(PacketIn &p)
{
    long int res = 0;
    for (int i = 0; i != 4; ++i)
    {
        res |= (byte)p.bytes[p.pos] << (24 - i * 8);
        p.pos++;
    }

    float *f = reinterpret_cast<float *>(&res);

    return *f;
}

inline void encodeDouble(double v, PacketOut &p)
{
    long long int *l = reinterpret_cast<long long int *>(&v);
    p.bytes.push_back(((*l) >> 56) & 0xff);
    p.bytes.push_back(((*l) >> 48) & 0xff);
    p.bytes.push_back(((*l) >> 40) & 0xff);
    p.bytes.push_back(((*l) >> 32) & 0xff);
    p.bytes.push_back(((*l) >> 24) & 0xff);
    p.bytes.push_back(((*l) >> 16) & 0xff);
    p.bytes.push_back(((*l) >> 8) & 0xff);
    p.bytes.push_back((*l) & 0xff);
}

inline float decodeDouble(PacketIn &p)
{
    long long int res = 0;
    for (int i = 0; i != 8; ++i)
    {
        res |= (byte)p.bytes[p.pos] << (56 - i * 8);
        p.pos++;
    }

    double *f = reinterpret_cast<double *>(&res);

    return *f;
}

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
