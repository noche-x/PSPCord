#include "ConnectionManager.h"

ConnectionManager::ConnectionManager(const char* address, int port) : m_ip(address), m_port(port) {
	//We need to load some basic kernel modules in order to use networking features
	sceUtilityLoadNetModule(PSP_NET_MODULE_COMMON); //All sceNetCommands
	sceUtilityLoadNetModule(PSP_NET_MODULE_INET); //All sceInetCommands (including the socket library)
	sceUtilityLoadNetModule(PSP_NET_MODULE_SSL); //Unused, but you can use SSL functions if you need.

	//Result stores our codes from the initialization process
	int result = 0;

	result = sceNetInit(128 * 1024, 42, 0, 42, 0); //Creates the network manager with a buffer
	if (result < 0) { //These If Blocks close the game on an error
		error = __FUNCTION__ + "INIT_NETWORK_MANAGER";
	}

	result = sceNetInetInit(); //Initializes Inet
	if (result < 0) {
		error = INIT_INET;
	}

	result = sceNetApctlInit(0x10000, 48); //Initializes Access Point Control
	if (result < 0) {
		error = INIT_ACCESS_POINT_CONTROL;
	}

	result = sceNetResolverInit(); //Initializes DNS resolver (unused)
	if (result < 0) {
		error = INIT_DNS_RESOLVER;
	}

	result = sceNetApctlConnect(1);	//Connects to your first (primary) internet connection.
    if (result < 0) {
		error = INIT_CONNECT_PRIMARY_INTERNET;
	}
}

ConnectionManager::~ConnectionManager() {
    
}

int ConnectionManager::GetConnectionState() {
	int state;
	
	if (sceNetApctlGetState(&state) < 0)
		error = CONNECTION_STATE_ERROR;

	return state;
}

int ConnectionManager::CreateSocket() {
	return m_socket = socket(PF_INET, SOCK_STREAM, 0);
}

int ConnectionManager::GetCurrentSocket() {
	return m_socket;
}

int ConnectionManager::Connect() {
   	struct sockaddr_in name;
	name.sin_family = AF_INET;
	name.sin_port = htons(m_port);

	inet_pton(AF_INET, m_ip, &name.sin_addr.s_addr);

	if (connect(m_socket, (struct sockaddr*) & name, sizeof(name)) < 0)
		error = (ConnectionLibErrors)*__errno();
}

int ConnectionManager::CloseCurrentSocket() {

}

ConnectionLibErrors ConnectionManager::GetErrorCode() {
   return error;
}
