#include "ConnectionManager.h"
#include <string>
#include <sstream>
#include <pspkernel.h>

ConnectionManager::ConnectionManager() {
	m_port = 35700;
}

ConnectionManager::~ConnectionManager() {
    
}

// this should be in a for state until the state returns 4
int ConnectionManager::GetConnectionState(int state) {	
	int ret = sceNetApctlGetState(&state);
	if (ret != 0)
		error = "ConnectionManager::GetConnectionState unable to get the connection state (is your network online?)";

	return ret;
}

int ConnectionManager::CreateSocket() {
	return m_socket = socket(PF_INET, SOCK_STREAM, 0);
}

int ConnectionManager::GetCurrentSocket() {
	return m_socket;
}

static __inline__ unsigned int sceAllegrexWsbw(unsigned int x) {
	return (((x & 0xFF)<<24) | ((x & 0xFF00)<<8) | ((x>>8) & 0xFF00) | ((x>>24) & 0xFF));
}

static __inline__ unsigned int sceAllegrexWsbh(unsigned int x) {
	return (((x<<8) & 0xFF00FF00) | ((x>>8) & 0x00FF00FF));
}

int ConnectionManager::Connect(const char* ipadd) {
	if (m_socket == NULL)
		CreateSocket();

	m_ip = ipadd;
   	struct sockaddr_in name;

	name.sin_family = AF_INET;
	name.sin_port = sceAllegrexWsbh(m_port);
// AF_INET
	inet_pton(AF_INET, m_ip, &name.sin_addr.s_addr);
	
	int ret = connect(m_socket, (struct sockaddr*) & name, sizeof(name));
	int filenameIndex = errno;      
	std::stringstream temp_str;
	temp_str<<(filenameIndex);
	std::string str = temp_str.str();
	const char* cstr2 = str.c_str();
	if (ret < 0)
		error = cstr2;
	
	return ret;
}

int ConnectionManager::SendMessage(const char* message) {
	if (send(m_socket, message, strlen(message), 0) > 0)
		return 1;
	else 
		return 0;
}

const char* ConnectionManager::RecvMessage() {
	char recv_buf[2000];
	if (recv(m_socket, recv_buf, 2000, 0) > 0)
		return (const char*)recv_buf;
	else
		return " ";
}

int ConnectionManager::CloseCurrentSocket() {
	close(m_socket);
}

const char* ConnectionManager::GetErrorCode() {
   return error;
}
