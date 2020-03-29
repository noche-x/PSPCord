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
#include <Graphics/Dialogs.h>
#include <Graphics/RendererCore.h>
#include <Network/NetworkDriver.h>

using namespace Stardust;

#define pspprint pspDebugScreenPrintf

enum PacketIDS
{
    NONE = 0,
    SEND_MESSAGE_PACKET = 302,
};

Network::PacketOut *sendMessagePacket(std::string username, std::string message)
{
    Network::PacketOut *p = new Network::PacketOut();
    p->ID = PacketIDS::SEND_MESSAGE_PACKET;
    Network::encodeString(username, *p);
    Network::encodeString(message, *p);
    return p;
}

int handler(Network::PacketIn *packet)
{
    std::string username = Network::decodeString(*packet);
    std::string password = Network::decodeString(*packet);
    Utilities::app_Logger->log(username);
    Utilities::app_Logger->log(password);

    return 0;
}

int main()
{
    // stardust init
    Platform::initPlatform("psp-chat");

    //Enable low-level logger
    //Utilities::app_Logger->currentLevel = Utilities::LOGGER_LEVEL_TRACE;
    //Utilities::detail::core_Logger->currentLevel = Utilities::LOGGER_LEVEL_TRACE;

    FILE *pFile;
    char buffer[100];

    pFile = fopen("ip.txt", "r");
    if (pFile == NULL)
        Utilities::app_Logger->log("Error opening file");
    else
    {
        while (!feof(pFile))
        {
            if (fgets(buffer, 100, pFile) == NULL)
                break;
            fputs(buffer, stdout);
        }
        fclose(pFile);
    }

    // test log
    Utilities::app_Logger->log("Hello World!");

    // get the keys that are pressed
    Utilities::updateInputs();

    // network init
    if (!Network::g_NetworkDriver.Init())
        Graphics::ShowMessageError("failed to initalize the network driver", 1);

    std::string username = "";
    unsigned short outtext[128];
    unsigned short *desc = (unsigned short *)strtol("username", NULL, 0);
    while (true)
    {
        Graphics::g_RenderCore.BeginCommands(true);
        Graphics::g_RenderCore.SetClearColor(200, 200, 200, 255);
        Graphics::g_RenderCore.Clear();

        if (Graphics::ShowOSK(desc, outtext, 128) != -1)
        {
            for (int j = 0; outtext[j]; j++)
            {
                unsigned c = outtext[j];

                if (32 <= c && c <= 127) // print ascii only
                    username += c;
            }
            break;
        }
        else
        {
            if (Graphics::ShowMessageYesNo("are you sure you want to exit") == 1)
                sceKernelExitGame();
        }

        Graphics::g_RenderCore.EndCommands(false);
    }

    std::string message = "do you want " + username + " as your username?";
    while (true)
    {
        Graphics::g_RenderCore.BeginCommands(true);
        Graphics::g_RenderCore.SetClearColor(104, 79, 137, 255);
        Graphics::g_RenderCore.Clear();
        auto val = Graphics::ShowMessageYesNo(message.c_str());
        if (val == -1)
        {
            if (Graphics::ShowOSK(desc, outtext, 128) != -1)
            {
                for (int j = 0; outtext[j]; j++)
                {
                    unsigned c = outtext[j];

                    if (32 <= c && c <= 127) // print ascii only
                        username += c;
                }
                message = "do you want " + username + " as your username?";
            }
            else
            {
                if (Graphics::ShowMessageYesNo("are you sure you want to exit") == 1)
                    sceKernelExitGame();
            }
        }
        else if (val == 1)
            break;

        Graphics::g_RenderCore.EndCommands(false);
    }

    while (1)
    {
        // connect to lan server
        Network::g_NetworkDriver.Connect(35700, buffer);
        // login packet creation

        std::string field = "";
        unsigned short outtext[128];
        unsigned short *desc = (unsigned short *)strtoul("message", NULL, 0);
        while (true)
        {
            Graphics::g_RenderCore.BeginCommands(true);
            Graphics::g_RenderCore.SetClearColor(100, 100, 100, 255);
            Graphics::g_RenderCore.Clear();
            if (Graphics::ShowOSK(desc, outtext, 128) != -1)
            {
                for (int j = 0; outtext[j]; j++)
                {
                    unsigned c = outtext[j];

                    if (32 <= c && c <= 127) // print ascii only
                        field += c;
                }
                break;
            }
            else
            {
                if (Graphics::ShowMessageYesNo("are you sure you want to exit") == 1)
                    sceKernelExitGame();
            }
            Graphics::g_RenderCore.EndCommands(false);
        }

        auto packet = sendMessagePacket(username, std::string(field));

        // add packet to the list
        Network::g_NetworkDriver.AddPacket(packet);
        // send the packet queue
        Network::g_NetworkDriver.SendPackets();

        //Network::g_NetworkDriver.AddPacketHandler(PacketIDS::LOGIN_PACKET, handler);

        //Network::g_NetworkDriver.ReceivePacket();
        //Network::g_NetworkDriver.HandlePackets();
    }

    Network::g_NetworkDriver.ClearPacketHandlers();
    Network::g_NetworkDriver.ClearPacketQueue();
    Network::g_NetworkDriver.Cleanup();

    Platform::exitPlatform();
    return 0;
}