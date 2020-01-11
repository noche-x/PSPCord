#include "framework/utils/common.h"

#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspctrl.h>

#include <stdio.h>

#include <string>
#include <iostream>

#include "Globals.h"
#include "framework/utils/Input.h"
#include "framework/gfx/RenderUtil.h"
#include "framework/gfx/intrafont/glib2d.h"
#include "framework/gfx/RenderUtil.h"
#include "framework/gfx/gui/Gui.h"
#include "framework/connection/ConnectionManager.h"

// needed for psp module
PSP_MODULE_INFO("PSP CHAT", PSP_MODULE_USER, 1, 0);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER); 
PSP_HEAP_SIZE_MAX();

int main(int argc, char* argv[])
{
	// basically makes the home button display the psp's own home menu
	// more on common.cpp
	setupExitCallback();
	
	ConnectionManager connection("192.168.2.189", 35412);

	// using psp's own latin font
	g_RenderUtil.init("flash0:/font/ltn0.pgf");

	// create the background texture
	g2dTexture* bg = g2dTexLoad("data/background.png", G2D_SWIZZLE);
	
	// initialize the gui
	Gui::init(0.5f, 0xFFFFFFFF, 0x00000000, 0.f, INTRAFONT_ALIGN_LEFT);
	
	// text input variables
	char ip[25];
	char username[25];
	char password[25];
	
	// login page, connecting page ...
	int page = 0;

	// main loop
	while (isRunning()) {
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

			if (Gui::button("login")) {
				strcpy(globalVariables::_ip, ip);
				Gui::selectableItemActive = false;
				page = 1;
				sceKernelDelayThread(1000000);				
			}			
			break;
		// connecting page
		case 1:
			Gui::text("Trying to connect to the server.");	

			if (Gui::button("back")) {
				Gui::selectableItemActive = false;
				page = 0;
				sceKernelDelayThread(1000000);
			}
			break;
		// main page
		case 2:
			Gui::text("Sending \"Hello World\" message to the server...");
			break;
		// invalid
		default:
			Gui::text("This is a invalid state please report this issue on GitHub.");
			break;
		}

		Gui::update();
		// end the current frame
		g_RenderUtil.frameEnd();
	}
	

	sceKernelExitGame();

	return 0;
}
