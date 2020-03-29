#include "packet_manager.h"

packet_manager::packet_manager(/* args */)
{
}

packet_manager::~packet_manager()
{
}

std::vector<char*> packet_manager::login_packet(const char* username, const char* password) {

}

std::vector<char*> packet_manager::get_channels_packet(const char* server_id) {

}

std::vector<message_t> packet_manager::get_messages_from_channel_packet(const char* channel_id) {

}

bool packet_manager::send_message_to_channel(const char* channel_id, message_t message) {
    
}