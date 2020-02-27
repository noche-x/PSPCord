//Okay, let's walk through this entire file...
//I dropped all the common includes in common.h
#include "framework/utils/common.h"

//This is the PSP_MODULE_HEADER
PSP_MODULE_INFO("Hello World", PSP_MODULE_USER, 1, 0);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER);
PSP_HEAP_SIZE_MAX();

// make it easier to type
#define pspprint pspDebugScreenPrintf

#include <iostream>
#include <fstream>

namespace Logger
{
    std::ofstream m_out;

    void Init(const char* filename) {
        if (std::ifstream(filename).good())
            std::remove(filename);

        m_out.open(filename, std::ofstream::out | std::ofstream::app );
    }

    void Log(const char* msg) {
        m_out << msg << std::endl;
        m_out.flush();
    }

    void Log(int msg) {
        m_out << msg << std::endl;
        m_out.flush();
    }

    void Close() {
        m_out.close();
    }
} // namespace Logger

extern "C"
{
	char buffer[1024] = "xdtbrh bxhdtrgbzrdsg";
	char recvbuf[1024] = "";

	//Pointer to a buffer
	//Then we write to said buffer
	void set_buffer(char *buffer[], size_t bufferLength, char *yourData)
	{
		if (bufferLength >= strlen((const char *)yourData))
		{
			*buffer = yourData;
		}
		else
		{ //Copy the stuff that fits
			for (unsigned int i = 0; i < bufferLength; i++)
			{
				*buffer[i] = yourData[i];
			}
		}
	}
}

const char *error_text = "";

static __inline__ unsigned int sceAllegrexWsbw(unsigned int x)
{
	return (((x & 0xFF) << 24) | ((x & 0xFF00) << 8) | ((x >> 8) & 0xFF00) | ((x >> 24) & 0xFF));
}

static __inline__ unsigned int sceAllegrexWsbh(unsigned int x)
{
	return (((x << 8) & 0xFF00FF00) | ((x >> 8) & 0x00FF00FF));
}

void connect()
{
	error_text = "";
	// create a socket to connect to the server
	int sock = socket(PF_INET, SOCK_STREAM, 0);

	// initialize the ip and the port
	struct sockaddr_in name;
	name.sin_family = AF_INET;
	name.sin_port = sceAllegrexWsbh(35700);

	inet_pton(AF_INET, "192.168.2.190", &name.sin_addr.s_addr); //Protect your IP... don't show it to the world... use the NAT for your advantage

	// connect to the server via the socket and the address
	if (connect(sock, (struct sockaddr *)&name, sizeof(name)) < 0)
		error_text = "connection error";

	// XD , this is a random xd left by the developers of this test project

	// send it through to socket
	if (send(sock, buffer, sizeof(buffer), 0)) //Server reads a buffer of 1024
		error_text = "sending error";

	// server sends an message aswell
	if (recv(sock, recvbuf, sizeof(recvbuf), 0))
		error_text = "recv error";

	// since we dont need it anymore we can close the socket
	close(sock);
}

//MAIN
int main(void)
{
	//Initialize the Screen
	pspDebugScreenInit();
	setupExitCallback();

    // Logger::Init("comeon.log");
	// Logger::Log("STARTED");

	//We need to load some basic kernel modules in order to use networking features
	// Logger::Log(sceUtilityLoadNetModule(PSP_NET_MODULE_COMMON)); //All sceNetCommands
	// Logger::Log(sceUtilityLoadNetModule(PSP_NET_MODULE_INET)); //All sceInetCommands (including the socket library)
	// Logger::Log(sceUtilityLoadNetModule(PSP_NET_MODULE_SSL)); //Unused, but you can use SSL functions if you need.
	sceUtilityLoadNetModule(PSP_NET_MODULE_COMMON);
	sceUtilityLoadNetModule(PSP_NET_MODULE_INET);
	sceUtilityLoadNetModule(PSP_NET_MODULE_SSL);
	//Result stores our codes from the initialization process
	int result = 0;

	result = sceNetInit(128 * 1024, 42, 0, 42, 0); //Creates the network manager with a buffer
	if (result < 0)
	{ //These If Blocks close the game on an error
		sceKernelExitGame();
	}
	// Logger::Log(result);

	result = sceNetInetInit(); //Initializes Inet
	if (result < 0)
	{
		sceKernelExitGame();
	}
	// Logger::Log(result);

	result = sceNetApctlInit(0x10000, 48); //Initializes Access Point Control
	if (result < 0)
	{
		sceKernelExitGame();
	}
	// Logger::Log(result);

	result = sceNetResolverInit(); //Initializes DNS resolver (unused)
	if (result < 0)
	{
		sceKernelExitGame();
	}
	// Logger::Log(result);

	result = sceNetApctlConnect(1); //Connects to your first (primary) internet connection.

	// Logger::Log(result);

	SceCtrlData buttonInput;

	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

	//Displays connection status
	int err;
	while (1)
	{
		int state;
		err = sceNetApctlGetState(&state);
		if (err != 0)
		{
			pspprint("ERROR: sceNetApctlGetState returns $%x\n", err);
			break;
		}
		if (state == 4)
			break; // connected!

		pspDebugScreenSetXY(0, 0);
		pspprint("Connecting...");
		pspprint("  connection state %d of 4\n", state);
		sceDisplayWaitVblankStart();
	}

	SceNetApctlInfo info;
	sceNetApctlGetInfo(PSP_NET_APCTL_INFO_IP, &info);

	/**
	 * The code above works and connects you to the local network
	 * The code below is WIP and connects you to the server and sends a message
	 */
	int d = 0;
	while (isRunning())
	{
		d++;
		connect();

		while (isRunning())
		{
			pspDebugScreenSetXY(0, 0);
			if (error_text = "")
				pspprint("Connected! MESSAGE RECIEVED: %s \n", recvbuf); //THERE
			else
				pspprint("%s : %i \n", error_text, errno);

			pspprint("\n %i \n", d); //THERE

			sceCtrlPeekBufferPositive(&buttonInput, 1);

			if (buttonInput.Buttons != 0)
			{
				if (buttonInput.Buttons & PSP_CTRL_UP)
				{
					sceKernelDelayThread(1000000 / 7);
					break;
				}
			}

			sceDisplayWaitVblankStart();
		}
	}

	sceKernelExitGame();

	return 0;
}