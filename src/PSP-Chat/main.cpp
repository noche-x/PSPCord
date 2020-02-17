#include "framework/utils/common.h"

#include <stdio.h>

#include <string>
#include <iostream>
#include <sstream>

#include "Globals.h"
#include "framework/utils/Input.h"
#include "framework/utils/Logger.h"
#include "framework/gfx/intrafont/glib2d.h"
#include "framework/gfx/RenderUtil.h"
#include "framework/gfx/gui/Gui.h"
//#include "framework/connection/ConnectionManager.h"

// needed for psp module
PSP_MODULE_INFO("PSP CHAT", PSP_MODULE_USER, 1, 0);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER);
PSP_HEAP_SIZE_MAX();

int main(int argc, char *argv[])
{
	// basically makes the home button display the psp's own home menu
	// more on common.cpp
	setupExitCallback();

	Logger::Init("file.log");
	Logger::Log("starting...");

	// setup the needed connection things
	//We need to load some basic kernel modules in order to use networking features
	Logger::Log(sceUtilityLoadNetModule(PSP_NET_MODULE_COMMON)); //All sceNetCommands
	Logger::Log(sceUtilityLoadNetModule(PSP_NET_MODULE_INET)); //All sceInetCommands (including the socket library)
	Logger::Log(sceUtilityLoadNetModule(PSP_NET_MODULE_SSL)); //Unused, but you can use SSL functions if you need.
	
	int result = sceNetInit(128 * 1024, 42, 0, 42, 0); //Creates the network manager with a buffer
	if (result < 0) { //These If Blocks close the game on an error
		Logger::Log("sceinit");
	}

	result = sceNetInetInit(); //Initializes Inet
	if (result < 0) {
		Logger::Log("sceinetinit");
	}

	result = sceNetApctlInit(0x10000, 48); //Initializes Access Point Control
	if (result < 0) {
		Logger::Log("apctlinit");
	}

	result = sceNetResolverInit(); //Initializes DNS resolver (unused)
	if (result < 0) {
		Logger::Log("resolver init");
	}

	result = sceNetApctlConnect(1);	//Connects to your first (primary) internet connection.

	//ConnectionManager connection;

	// using psp's own latin font
	g_RenderUtil.init("flash0:/font/ltn0.pgf");

	// create the background texture
	g2dTexture *bg = g2dTexLoad("data/background.png", G2D_SWIZZLE);

	// initialize the gui
	Gui::init(0.5f, 0xFFFFFFFF, 0x00000000, 0.f, INTRAFONT_ALIGN_LEFT);

	// text input variables
	char ip[25];
	char username[25];
	char password[25];

	// login page, connecting page ...
	int page = 0;

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

		// switch between pages, login, connecting ...
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
			int connect_state;
			int ret = sceNetApctlGetState(&connect_state);
			//int ret = connection.GetConnectionState(connect_state);
			if (ret != 0)
			{
				// Gui::text(connection.GetErrorCode());
				Gui::text("get state returns %i", ret);
			}
			else if (connect_state == 4)
			{
				Gui::text("connected");
				sceKernelDelayThread(3000000);
				page = 1;
			}
			Gui::text("connection state : %i", connect_state);

			break;
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
				strcpy(globalVariables::_ip, ip);
				Gui::selectableItemActive = false;
				page = 1;
				sceKernelDelayThread(1000000);
			}
			break;
		// connecting page
		case 1:
			// Gui::text("Trying to connect to the server.");

			// int ret = connection.Connect("192.168.2.190");
			// if (ret == 0)
			// {
			// 	Gui::text("Connection ESTHABLISHED!");
			// 	if (connection.SendMessage("hello from psp chat"))
			// 		Gui::text("MESSAGE SENT");

			// 	const char *recv_message = connection.RecvMessage();
			// 	if (recv_message != " ")
			// 		Gui::text(recv_message);
			// }
			// else
			// {
			// 	Gui::text("error");
			// 	Gui::text(connection.GetErrorCode());
			// }

			// if (Gui::button("back"))
			// {
			// 	Gui::selectableItemActive = false;
			// 	page = 0;
			// 	sceKernelDelayThread(1000000);
			// }
			break;
		}

		Gui::update();
		// end the current frame
		g_RenderUtil.frameEnd();
	}

	sceKernelExitGame();

	return 0;
}
