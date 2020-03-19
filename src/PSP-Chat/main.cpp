// //Okay, let's walk through this entire file...
// //I dropped all the common includes in common.h
// #include "framework/utils/common.h"
// #include "framework/utils/console_logger.h"

// //This is the PSP_MODULE_HEADER
// PSP_MODULE_INFO("PSP-Chat", PSP_MODULE_USER, 1, 0);
// PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER);
// PSP_HEAP_SIZE_MAX();

// // make it easier to type
// #define pspprint pspDebugScreenPrintf

// extern "C"
// {
// 	char buffer[1024] = "xdtbrh bxhdtrgbzrdsg";
// 	char recvbuf[1024] = "";

// 	//Pointer to a buffer
// 	//Then we write to said buffer
// 	void set_buffer(char *buffer[], size_t bufferLength, char *yourData)
// 	{
// 		if (bufferLength >= strlen((const char *)yourData))
// 		{
// 			*buffer = yourData;
// 		}
// 		else
// 		{ //Copy the stuff that fits
// 			for (unsigned int i = 0; i < bufferLength; i++)
// 			{
// 				*buffer[i] = yourData[i];
// 			}
// 		}
// 	}
// }

// bool connect()
// {
// 	pspprint("[*] connecting to LAN server...\n");
// 	// create a socket to connect to the server
// 	int sock = socket(PF_INET, SOCK_STREAM, 0);

// 	// initialize the ip and the port
// 	struct sockaddr_in name;
// 	name.sin_family = AF_INET;
// 	name.sin_port = __builtin_bswap16(35700);

// 	inet_pton(AF_INET, "192.168.2.190", &name.sin_addr.s_addr); //Protect your IP... don't show it to the world... use the NAT for your advantage

// 	// connect to the server via the socket and the address
// 	if (connect(sock, (struct sockaddr *)&name, sizeof(name)) < 0) {
// 		pspprint("[!] connection error\n");
// 		return false;
// 	}

// 	// XD , this is a random xd left by the developers of this test project

// 	// send it through to socket
// 	if (send(sock, buffer, sizeof(buffer), 0) < 0) //Server reads a buffer of 1024
// 	{
// 		pspprint("[!] sending error\n");
// 		return false;
// 	}

// 	// server sends an message aswell
// 	if (recv(sock, recvbuf, sizeof(recvbuf), 0) < 0) {
// 		pspprint("[!] recv error\n");
// 		return false;
// 	}

// 	// since we dont need it anymore we can close the socket
// 	close(sock);
// 	return true;
// }

// //MAIN
// int main(void)
// {
// 	//Initialize the Screen
// 	pspDebugScreenInit();
// 	setupExitCallback();

// 	/*
//     ____  _____ ____        ________          __
//    / __ \/ ___// __ \      / ____/ /_  ____ _/ /_
//   / /_/ /\__ \/ /_/ /_____/ /   / __ \/ __ `/ __/
//  / ____/___/ / ____/_____/ /___/ / / / /_/ / /_
// /_/    /____/_/          \____/_/ /_/\__,_/\__/

// 	*/
// 	pspprint("    ____  _____ ____        ________          __ \n");
// 	pspprint("   / __ \/ ___// __ \      / ____/ /_  ____ _/ /_\n");
// 	pspprint("  / /_/ /\__ \/ /_/ /_____/ /   / __ \/ __ `/ __/\n");
// 	pspprint(" / ____/___/ / ____/_____/ /___/ / / / /_/ / /_  \n");
// 	pspprint("/_/    /____/_/          \____/_/ /_/\__,_/\__/  \n");
// 	pspprint("\n");
// 	pspprint("[+] temporary console gui :(\n");
// 	pspprint("======================================\n");

// 	pspprint("[*] loading kernel modules\n");

// 	// //We need to load some basic kernel modules in order to use networking features
// 	// sceUtilityLoadNetModule(PSP_NET_MODULE_COMMON);	//All sceNetCommands
// 	// sceUtilityLoadNetModule(PSP_NET_MODULE_INET); 	//All sceInetCommands (including the socket library)
// 	// sceUtilityLoadNetModule(PSP_NET_MODULE_SSL); 	//Unused, but you can use SSL functions if you need.
// 	// //Result stores our codes from the initialization process
// 	// int result = 0;
// 	// pspprint("[+] done\n\n");

// 	// pspprint("[*] initalizing inet and apctl\n");
// 	// result = sceNetInit(128 * 1024, 42, 0, 42, 0); //Creates the network manager with a buffer
// 	// if (result < 0)
// 	// { //These If Blocks close the game on an error
// 	// 	sceKernelExitGame();
// 	// }

// 	// result = sceNetInetInit(); //Initializes Inet
// 	// if (result < 0)
// 	// {
// 	// 	sceKernelExitGame();
// 	// }

// 	// result = sceNetApctlInit(0x10000, 48); //Initializes Access Point Control
// 	// if (result < 0)
// 	// {
// 	// 	sceKernelExitGame();
// 	// }

// 	// result = sceNetResolverInit(); //Initializes DNS resolver (unused)
// 	// if (result < 0)
// 	// {
// 	// 	sceKernelExitGame();
// 	// }

// 	// result = sceNetApctlConnect(1); //Connects to your first (primary) internet connection.

// 	// pspprint("[+] done\n\n");

// 	// SceCtrlData buttonInput;

// 	// sceCtrlSetSamplingCycle(0);
// 	// sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

// 	// int x_pos = pspDebugScreenGetX();
// 	// int y_pos = pspDebugScreenGetY();
// 	// //Displays connection status
// 	// int err;
// 	// while (isRunning())
// 	// {
// 	// 	int state;
// 	// 	err = sceNetApctlGetState(&state);
// 	// 	if (err != 0)
// 	// 	{
// 	// 		pspDebugScreenSetXY(x_pos, y_pos + 1);
// 	// 		pspprint("[!] sceNetApctlGetState returns $%x\n", err);
// 	// 		break;
// 	// 	}
// 	// 	if (state == 4) {
// 	// 		pspDebugScreenSetXY(x_pos, y_pos + 1);
// 	// 		pspprint("[*] connection state %d of 4      connected!\n\n", state);
// 	// 		break; // connected!
// 	// 	}

// 	// 	pspDebugScreenSetXY(x_pos, y_pos);
// 	// 	pspprint("[*] connecting to primary internet connection...\n");
// 	// 	pspprint("[*] connection state %d of 4\n\n", state);
// 	// 	sceDisplayWaitVblankStart();
// 	// }

// 	SceNetApctlInfo info;
// 	sceNetApctlGetInfo(PSP_NET_APCTL_INFO_IP, &info);

// 	pspprint("[+] sceNetApctlInfo -> strength %c\n", info.strength);
// 	pspprint("[+] sceNetApctlInfo -> ssid %s\n", info.ssid);
// 	pspprint("[+] sceNetApctlInfo -> name %s\n\n", info.name);

// 	x_pos = pspDebugScreenGetX();
// 	y_pos = pspDebugScreenGetY();
// 	/**
// 	 * The code above works and connects you to the local network
// 	 * The code below is WIP and connects you to the server and sends a message
// 	 */
// 	int d = 0;
// 	while (isRunning())
// 	{
// 		bool doublebreak = false;
// 		d++;
// 		bool ret = connect();

// 		while (isRunning())
// 		{
// 			pspDebugScreenSetXY(x_pos, y_pos);
// 			if (ret)
// 				pspprint("[+] connected! recieved message: %s\n\n", recvbuf);
// 			else
// 				pspprint("[!] failed to connect errno: %i\n\n", errno);

// 			pspprint("[+] amount of connects: %i\n\n", d);
// 			pspprint("[i] press up arrow to reconnect\n");
// 			pspprint("[i] press down arrow to close the program\n\n");

// 			sceCtrlPeekBufferPositive(&buttonInput, 1);

// 			if (buttonInput.Buttons != 0)
// 			{
// 				if (buttonInput.Buttons & PSP_CTRL_UP)
// 				{
// 					sceKernelDelayThread(1000000 / 7);
// 					break;
// 				}
// 				if (buttonInput.Buttons & PSP_CTRL_DOWN) {
// 					sceKernelDelayThread(1000000 / 7);
// 					doublebreak = true;
// 					break;
// 				}
// 			}
// 			sceDisplayWaitVblankStart();
// 		}

// 		if (doublebreak) {
// 			pspprint("[+] see ya next time bye\n\n");
// 			break;
// 		}
// 		else
// 		{
// 			pspprint("[*] reconnecting...\n\n");
// 		}

// 	}

// 	pspprint("[*] exitting...\n");

// 	sceKernelExitGame();

// 	return 0;
// }

#include <Platform/Platform.h>
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspmath.h>
#include <iostream>

PSP_MODULE_INFO("PSP-Chat", 0, 0, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_VFPU | THREAD_ATTR_USER);
PSP_HEAP_SIZE_KB(-1024);

#include <Utilities/Timer.h>
#include <Utilities/Logger.h>
#include <Utilities/Input.h>
#include <Network/NetworkDriver.h>

using namespace Stardust;

#define pspprint pspDebugScreenPrintf

enum PacketIDS
{
    NONE = 0,
    LOGIN_PACKET = 301,
    SEND_MESSAGE_PACKET = 302,
};

Network::PacketOut *loginPacket(std::string username, std::string password)
{
    Network::PacketOut *p = new Network::PacketOut();
    p->ID = PacketIDS::LOGIN_PACKET;
    Network::encodeString(username, *p);
    Network::encodeString(password, *p);
    std::string str(p->bytes.begin(), p->bytes.end());
    Utilities::app_Logger->log(str.c_str());
    return p;
}

Network::PacketOut *sendMessage(std::string username, std::string password)
{
    Network::PacketOut *p = new Network::PacketOut();
    p->ID = PacketIDS::LOGIN_PACKET;
    Network::encodeString(username, *p);
    Network::encodeString(password, *p);
    std::string str(p->bytes.begin(), p->bytes.end());
    Utilities::app_Logger->log(str.c_str());
    return p;
}

int handler(Network::PacketIn *packet)
{
    Utilities::app_Logger->log("recieved!");
    Utilities::app_Logger->log(std::to_string(packet->ID));
    printf("hashedChars: ");
    for (int i = 0; i < packet->bytes.size(); i++)
    {
        printf("%x", packet->bytes[i]);
    }
    printf("\n");

    auto username = Network::decodeString(*packet);
    auto password = Network::decodeString(*packet);
    Utilities::app_Logger->log(username);
    Utilities::app_Logger->log(password);
}

int main()
{
    // stardust init
    Platform::initPlatform("psp-chat");

    // test log
    Utilities::app_Logger->log("Hello World!");
    // get the keys that are pressed
    Utilities::updateInputs();

    // network init
    Network::g_NetworkDriver.Init();
    // connect to lan server
    Network::g_NetworkDriver.Connect(35700, "192.168.43.155");
    // login packet creation
    auto packet = loginPacket("noche", "123");

    // add packet to the list
    Network::g_NetworkDriver.AddPacket(packet);
    // send the packet queue
    Network::g_NetworkDriver.SendPackets();

    Network::g_NetworkDriver.AddPacketHandler(PacketIDS::LOGIN_PACKET, handler);
    Network::g_NetworkDriver.ReceivePacket();
    Network::g_NetworkDriver.HandlePackets();
    //Network::PacketOut recvPacket = Network::g_NetworkDriver.packetQueue().back();
    //Utilities::g_Logger->log(recvPacket.ID);
    //Utilities::g_Logger->log(recvPacket.bytes);

    // while (!Utilities::KeyPressed(PSP_CTRL_START)) {
    //     Utilities::updateInputs();
    // }

    Platform::exitPlatform();

    return 0;
}