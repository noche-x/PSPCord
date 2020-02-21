#include "ConnectionManager.h"
#include <pspkernel.h>

ConnectionManager::ConnectionManager() : m_port(35700) {
	// load the needed modules for the psp networking
	sceUtilityLoadNetModule(PSP_NET_MODULE_COMMON); // sceNetCommands
	sceUtilityLoadNetModule(PSP_NET_MODULE_INET); // sceINetCommands with socket stuff
	// ssl isn't needed because we use sockets
	//sceUtilityLoadNetModule(PSP_NET_MODULE_SSL);

	// rather than creating multiple results this is better
	// self explainotrayepoaajnfka
	int result = 0;

	// create the net manager with buffer
	result = sceNetInit(128 * 1024, 42, 0, 42, 0);
	if (result < 0)
	{
		sceKernelExitGame();
	}

	// initalize inet 
	result = sceNetInetInit();
	if (result < 0)
	{
		sceKernelExitGame();
	}

	// initalize access point protocol
	result = sceNetApctlInit(0x10000, 48);
	if (result < 0)
	{
		sceKernelExitGame();
	}

	// init the dns resolver
	result = sceNetResolverInit();
	if (result < 0)
	{
		sceKernelExitGame();
	}

	// connect to the primary network config
	result = sceNetApctlConnect(1);
}

ConnectionManager::~ConnectionManager() {
    
}
/*

	const char* buffer = "fuck you";
	char* recvbuf = "";
	const char* error_text = "";
	int sock = socket(PF_INET, SOCK_STREAM, 0);

	struct sockaddr_in name;
	name.sin_family = AF_INET;
	name.sin_port = sceAllegrexWsbh(35700);

	inet_pton(AF_INET, "192.168.2.190", &name.sin_addr.s_addr);


	close(sock);

*/
// run this func in a while loop
// returns
// 0 = good
// !=0 = not good
// states
// 4 = connected
// 4< error
// 4> in the proccess of connecting
int ConnectionManager::GetConnectionState(int state) {	
	int ret = sceNetApctlGetState(&state);
	if (ret != 0)
		error = "ConnectionManager::GetConnectionState unable to get the connection state (is your network online?)";

	return ret;
}

// create a socket for conneciton
int ConnectionManager::CreateSocket() {
	return m_socket = socket(PF_INET, SOCK_STREAM, 0);
}

// return the current socket for whatever
int ConnectionManager::GetCurrentSocket() {
	return m_socket;
}

// htons doesnt work so we have to use these ones
// which are the exact same things
static __inline__ unsigned int sceAllegrexWsbw(unsigned int x) {
	return (((x & 0xFF)<<24) | ((x & 0xFF00)<<8) | ((x>>8) & 0xFF00) | ((x>>24) & 0xFF));
}

static __inline__ unsigned int sceAllegrexWsbh(unsigned int x) {
	return (((x<<8) & 0xFF00FF00) | ((x>>8) & 0x00FF00FF));
}

// i dont know what this func does
// i think it connects to a ip address that the user gives
// interesting...
int ConnectionManager::Connect(const char* ipadd) {
	m_ip = ipadd;

	// if i forgot to create a socket
	// forgive myself and create a socket
	if (m_socket == NULL)
		CreateSocket();

	// initalize the port and the connection type
   	struct sockaddr_in name;

	name.sin_family = AF_INET;
	name.sin_port = sceAllegrexWsbh(m_port);

	inet_pton(AF_INET, m_ip, &name.sin_addr.s_addr);
	
	// connect to the server
	int ret = connect(m_socket, (struct sockaddr*) & name, sizeof(name));
	if (ret < 0)
		error = "ConnectionManager::Connect unable to connect to the given ip";
	
	return ret;
}

int ConnectionManager::SendMessage(const char* message) {
	int ret = send(m_socket, message, sizeof(message), 0);
	if (ret < 0) 
		error = "ConMan::Send unable to send message";
	return ret;
}

int ConnectionManager::RecvMessage(char* recieve) {
	int ret = recv(m_socket, recieve, sizeof(recieve), 0);
	if (ret < 0)
		error = "ConMan::Recv unable to recv message";
	return ret;
}

int ConnectionManager::CloseCurrentSocket() {
	close(m_socket);
}

const char* ConnectionManager::GetErrorCode() {
   return error;
}
