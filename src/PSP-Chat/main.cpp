#include "framework/utils/common.h"

#include <stdio.h>

#include <string>
#include <sstream>

#include "framework/utils/Input.h"
#include "framework/gfx/intrafont/glib2d.h"
#include "framework/gfx/RenderUtil.h"
#include "framework/gfx/gui/Gui.h"
//#include "framework/connection/ConnectionManager.h"

// needed for psp module
PSP_MODULE_INFO("PSP CHAT", PSP_MODULE_USER, 1, 0);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER);
PSP_HEAP_SIZE_MAX();

static __inline__ unsigned int sceAllegrexWsbh(unsigned int x)
{
	return (((x << 8) & 0xFF00FF00) | ((x >> 8) & 0x00FF00FF));
}

int main(int argc, char *argv[])
{
	// basically makes the home button display the psp's own home menu
	// more on common.cpp
	setupExitCallback();

	// setup the needed connection things
	//ConnectionManager connection;

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

	// using psp's own latin font
	g_RenderUtil.init("flash0:/font/ltn0.pgf");

	// create the background texture
	// g2dTexture *bg = g2dTexLoad("data/background.png", G2D_SWIZZLE);

	// initialize the gui
	Gui::init(0.5f, 0xFFFFFFFF, 0x00000000, 0.f, INTRAFONT_ALIGN_LEFT);

	// text input variables
	char ip[25];
	char username[25];
	char password[25];

	// login page, connecting page ...
	int page = -2;

	// main loop
	while (isRunning())
	{
		// begin renderer frame
		g_RenderUtil.frameBegin();
		// clear the screen with a color
		g_RenderUtil.frameClear(G2D_RGBA(80, 80, 80, 255));

		// display the background image
		// too distracting, until its changed its removed
		//g_RenderUtil.image(bg, 0, 0);

		// begin the gui
		Gui::begin(CENTER, 315, 215);

		//switch between pages, login, connecting...
		switch (page)
		{
		// connect to the primary network page
		case -2:
		{
			//Gui::text(connection.GetErrorCode());
			//Gui::text("%i", result);
			if (Gui::button("next"))
			{
				page = -1;
			}
			break;
		}
		case -1:
		{
			// int connection_state;
			// if (connection.GetConnectionState(connection_state) != 0)
			// 	Gui::text(connection.GetErrorCode());
			// if (connection_state == 4)
			// {
			// 	Gui::text("connected");
			// 	sceKernelDelayThread(3000000);
			// 	page = 1;
			// }

			// Gui::text("connection state : %i", connection_state);

			int state;
			int err = sceNetApctlGetState(&state);
			if (err != 0)
				Gui::text("ERRRORORORO apctlgetstate returns %i", err);
			if (state == 4)
				sceKernelDelayThread(3000000);
			page = 1;

			Gui::text("connection state : %i", state);
		}
		// login page
		case 0:
			Gui::text("Please enter your details and the server's IP");

			Gui::textInput("ip", ip);
			Gui::textInput("username", username);
			Gui::textInput("password", password);

			Gui::text("%s", ip);
			Gui::text("%s", username);
			Gui::text("%s", password);

			Gui::text("%s", Gui::selectableItemActive ? "true" : "false");

			if (Gui::button("login"))
			{
				Gui::selectableItemActive = false;
				page = 1;
				sceKernelDelayThread(1000000);
			}
			break;
		// connecting page
		case 1:
		{
			// const char *error_text = "";
			// char *recvbuf = "";
			// Gui::text("Trying to connect to the server.");

			// if (Gui::button("connect send recieve"))
			// {
			// 	connection.Connect("192.168.2.190");
			// 	connection.SendMessage("hey you fucking shitty ass server");
			// 	connection.RecvMessage(recvbuf);
			// 	connection.CloseCurrentSocket();
			// }
			// Gui::text((const char *)recvbuf);

			const char* error_text = "";
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
			
			const char* buffer = "fuck you fucking server because you wont recieve this";
			char* recvbuf = "";

			// send it through to socket
			if (send(sock, buffer, sizeof(buffer), 0)) //Server reads a buffer of 1024
				error_text = "sending error";

			// server sends an message aswell
			if (recv(sock, recvbuf, sizeof(recvbuf), 0))
				error_text = "recv error";

			// since we dont need it anymore we can close the socket
			close(sock);
		}
		break;
		}

		Gui::update();
		// end the current frame
		g_RenderUtil.frameEnd();
	}

	sceKernelExitGame();

	return 0;
}
