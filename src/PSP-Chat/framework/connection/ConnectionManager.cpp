#include "ConnectionManager.h"
#include <string>

ConnectionManager::ConnectionManager() {
	//We need to load some basic kernel modules in order to use networking features
	sceUtilityLoadNetModule(PSP_NET_MODULE_COMMON); //All sceNetCommands
	sceUtilityLoadNetModule(PSP_NET_MODULE_INET); //All sceInetCommands (including the socket library)
	sceUtilityLoadNetModule(PSP_NET_MODULE_SSL); //Unused, but you can use SSL functions if you need.

	//Result stores our codes from the initialization process
	int result = 0;

	result = sceNetInit(128 * 1024, 42, 0, 42, 0); //Creates the network manager with a buffer
	if (result < 0) { //These If Blocks close the game on an error
		error = "ConnectionManager::ConnectionManager unable to init network manager";
	}

	result = sceNetInetInit(); //Initializes Inet
	if (result < 0) {
		error = "ConnectionManager::ConnectionManager unable to init inet";
	}

	result = sceNetApctlInit(0x10000, 48); //Initializes Access Point Control
	if (result < 0) {
		error = "ConnectionManager::ConnectionManager unable to init access point control";
	}

	result = sceNetResolverInit(); //Initializes DNS resolver (unused)
	if (result < 0) {
		error = "ConnectionManager::ConnectionManager unable to init dns resolver";
	}

	result = sceNetApctlConnect(1);	//Connects to your first (primary) internet connection.
    if (result < 0) {
		error = "ConnectionManager::ConnectionManager unable to connect to the primary internet connection";
	}

	m_port = 35700;
}

ConnectionManager::~ConnectionManager() {
    
}

// this should be in a for state until the state returns 4
int ConnectionManager::GetConnectionState() {
	int state;
	
	if (sceNetApctlGetState(&state) < 0)
		error = "ConnectionManager::GetConnectionState unable to get the connection state (is your network online?)";

	return state;
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

	inet_pton(AF_INET, m_ip, &name.sin_addr.s_addr);
	
	int ret = -1;
	if (ret = connect(m_socket, (struct sockaddr*) & name, sizeof(name)) < 0)
		error = "ConnectionManager::Connect unable to connect to the given ip";
	
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
