////Okay, let's walk through this entire file...
////I dropped all the common includes in common.h
//#include "common.h"
//#include <time.h>
//#include <stdio.h>
//
////This is the PSP_MODULE_HEADER
//PSP_MODULE_INFO("psp chatter", PSP_MODULE_USER, 1, 0);
//PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER); 
//PSP_HEAP_SIZE_MAX();
//
//// make it easier to type
//#define printf_psp pspDebugScreenPrintf
//
//extern "C" {
//	char buffer[1024] = "Hello World!";
//
//	//Pointer to a buffer
//	//Then we write to said buffer
//	void set_buffer(char* buffer[], size_t bufferLength, char* yourData) {
//		if (bufferLength >= strlen((const char*)yourData)) {
//			*buffer = yourData;
//		}
//		else { //Copy the stuff that fits
//			for (unsigned int i = 0; i < bufferLength; i++) {
//				*buffer[i] = yourData[i];
//			}
//		}
//	}
//
//	void print_time() {
//		time_t clk = time(NULL);
//		char* time_char = "";
//		sprintf(time_char, ctime(&clk));
//
//		pspDebugScreenSetXY(0, 0);
//		pspDebugScreenSetTextColor(0xFFFF00FF);
//		printf_psp(time_char);
//	}
//
//	void print(const char* h) {
//		print_time();
//		pspDebugScreenSetTextColor(0xFFFFFFFF);
//
//		printf_psp(h);
//		printf_psp("\n");
//	}
//
//	void printi(const char* h, int t) {
//		print_time();
//		pspDebugScreenSetTextColor(0xFFFFFFFF);
//
//		printf_psp(h, t);
//		printf_psp("\n");
//	}
//
//	void printc(const char* h, char* t) {
//		print_time();
//		pspDebugScreenSetTextColor(0xFFFFFFFF);
//
//		printf_psp(h, t);
//		printf_psp("\n");
//	}
//}
//
//int startMenuCtrlDelay() {
//	sceKernelDelayThread(1000000 / 7);
//	return 0;
//}
//
////MAIN
//int main(void) 
//{       
//	//Initialize the Screen
//	pspDebugScreenInit();
//	setupExitCallback();
//	
//	//We need to load some basic kernel modules in order to use networking features
//	sceUtilityLoadNetModule(PSP_NET_MODULE_COMMON); //All sceNetCommands
//	sceUtilityLoadNetModule(PSP_NET_MODULE_INET); //All sceInetCommands (including the socket library)
//	sceUtilityLoadNetModule(PSP_NET_MODULE_SSL); //Unused, but you can use SSL functions if you need.
//	
//	//Result stores our codes from the initialization process
//	int result = 0;
//
//	result = sceNetInit(128*1024, 42, 0, 42, 0); //Creates the network manager with a buffer
//	if(result < 0){ //These If Blocks close the game on an error
//		sceKernelExitGame();
//	}
//	
//	result = sceNetInetInit(); //Initializes Inet
//	if(result < 0){
//		sceKernelExitGame();
//	}
//	
//	result = sceNetApctlInit(0x10000, 48); //Initializes Access Point Control
//	if(result < 0){
//		sceKernelExitGame();
//	}
//	
//	result = sceNetResolverInit(); //Initializes DNS resolver (unused)
//	if(result < 0){
//		sceKernelExitGame();
//	}
//
//	result = sceNetApctlConnect(1);	//Connects to your first (primary) internet connection.
//	
//	//Displays connection status
//	int err;
//	while (1){
//		int state;
//		err = sceNetApctlGetState(&state);
//		if (err != 0)
//		{
//			printi("ERROR: sceNetApctlGetState returns $%x\n", err);
//			break;
//		}
//		if (state == 4)
//			break;  // connected!
//
//		pspDebugScreenSetXY(0, 1);
//		print("connecting..."); //THERE
//		pspDebugScreenSetXY(0, 2);
//		printi("connection state %d of 4\n", state);
//		sceDisplayWaitVblankStart();
//	}
//
//	SceNetApctlInfo info;
//	sceNetApctlGetInfo(PSP_NET_APCTL_INFO_IP, &info);
//
//	/**
//	 * The code above works and connects you to the local network
//	 * The code below is WIP and connects you to the server and sends a message
//	 */
//
//	// create a socket to connect to the server
//	int sock = socket(PF_INET, SOCK_STREAM, 0);
//
//	// initialize the ip and the port
//	struct sockaddr_in name;
//	name.sin_family = AF_INET;
//	name.sin_port = htons(5555);
//
//	inet_pton(AF_INET, "192.168.2.189", &name.sin_addr.s_addr); //Protect your IP... don't show it to the world... use the NAT for your advantage
//
//	// connect to the server via the socket and the address
//	connect(sock, (struct sockaddr *)&name, sizeof(name));
//
//	// XD , this is a random xd left by the developers of this test project
//	
//	int error_code = 0;
//
//	// send it through to socket
//	if (send(sock, buffer, sizeof(buffer), 0) == -1)  //Server reads a buffer of 
//		error_code = 1; //THERE
//
//	// server sends an message aswell
//	if (recv(sock, buffer, sizeof(buffer), 0) == 0)
//		error_code = 2; //THERE
//	
//	// since we dont need it anymore we can close the socket
//	close(sock);
//
//	while(isRunning())
//	{
//		pspDebugScreenSetXY(0, 0);
//		if (error_code == 1) 
//			print("send fail!"); //THERE		
//		if (error_code == 2)
//			print("recv fail!"); //THERE
//		if (error_code == 0)
//			printc("Connected! MESSAGE RECIEVED: %s", buffer); //THERE
//		sceDisplayWaitVblankStart();
//	}
//
//
//	sceKernelExitGame();	
//
//	return 0;
//}

#include "common.h"
#include "renderer.h"
#include "page/login.h"
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspctrl.h>
#include <stdio.h>
#include <string>
#include <iostream>

PSP_MODULE_INFO("PSP CHAT", PSP_MODULE_USER, 1, 0);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER); 
PSP_HEAP_SIZE_MAX();

int main(int argc, char* argv[])
{
	setupExitCallback();
	renderer::init();
	
	SceCtrlData buttonInput;

	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

	int menu_item = 0;
	bool menu_active = false;
	int rect_y = 0;
	int rect_w = 0;
	int rect_x = 0;

	char ip[25];
	char psw[25];
	char usr[25];

	while (isRunning()) {
		renderer::begin();
		sceCtrlPeekBufferPositive(&buttonInput, 1);
		
		if (menu_item > 3)
			menu_item = 0;

		if (menu_item < 0)
			menu_item = 3;

		if (buttonInput.Buttons != 0)
		{
			if (buttonInput.Buttons & PSP_CTRL_DOWN) { 
				menu_item++;
				sceKernelDelayThread(1000000 / 7);
			}

			if (buttonInput.Buttons & PSP_CTRL_UP) {
				menu_item--;
				sceKernelDelayThread(1000000 / 7);
			}

			if (buttonInput.Buttons & PSP_CTRL_CROSS) {
				menu_active = true;
				sceKernelDelayThread(1000000 / 7);
			}

		}
			
		//if (page::login::run() == 0)
		//	continue;
		renderer::rect_fill(120, 81, 240, 110, G2D_RGBA(25, 25, 25, 255));
		renderer::rect_fill(125, 86, 230, 100, G2D_RGBA(50, 50, 50, 255));

		//renderer::rect_fill(120, 136, 240, 2, G2D_RGBA(255, 0, 0, 255));
		//renderer::rect_fill(240, 86, 2, 100, G2D_RGBA(255, 0, 0, 255));
				
		if (menu_item == 0) {
			rect_x = 126 + 70;
			rect_y = 88;
			rect_w = 154 + 4;
		}
		if (menu_item == 1) {
			rect_x = 126 + 70;
			rect_y = 94 + renderer::font->texYSize;
			rect_w = 154 + 4;
		}
		if (menu_item == 2) {
			rect_x = 126 + 70;
			rect_y = 100 + renderer::font->texYSize * 2;
			rect_w = 154 + 4;
		}
		if (menu_item == 3) {
			rect_x = 126;
			rect_y = 106 + renderer::font->texYSize * 4;
			rect_w = 154 + 74;
		}

		if (menu_item == 0 && menu_active) {
			unsigned short temp[128];
			unsigned short desc[10] = { 'I','p',' ',' ',' ',' ',' ',' ', ' ', ' ' };
			if (renderer::keyboard(desc, temp, 128) != -1)
			{
				std::string conv = "";
				for (int j = 0; temp[j]; j++)
				{
					unsigned c = temp[j];

					if (32 <= c && c <= 127) // print ascii only
						conv += c;
				}

				sprintf(ip, "%s", conv.c_str());
			}

			menu_active = false;
		}

		if (menu_item == 1 && menu_active) {
			unsigned short temp[128];
			unsigned short desc[10] = { 'U','s','e','r','n','a','m','e', ' ', ' ' };
			if (renderer::keyboard(desc, temp, 128) != -1)
			{
				std::string conv = "";
				for (int j = 0; temp[j]; j++)
				{
					unsigned c = temp[j];

					if (32 <= c && c <= 127) // print ascii only
						conv += c;
				}

				sprintf(usr, "%s", conv.c_str());
			}

			menu_active = false;
		}

		if (menu_item == 2 && menu_active) {
			unsigned short temp[128];
			unsigned short desc[10] = { 'P','a','s','s','w','o','r','d', ' ', ' ' };
			if (renderer::keyboard(desc, temp, 128) != -1)
			{
				std::string conv = "";
				for (int j = 0; temp[j]; j++)
				{
					unsigned c = temp[j];

					if (32 <= c && c <= 127) // print ascii only
						conv += c;
				}

				sprintf(psw, "%s", conv.c_str());
			}

			menu_active = false;
		}

		if (menu_item == 3 && menu_active) {
			// login
		}

		renderer::rect_fill(rect_x, rect_y, rect_w, renderer::font->texYSize + 5.5f, G2D_RGBA(100, 100, 100, 255));

		renderer::set_font_style(0.5f, 0xFFFFFFFF, 0x00000000, INTRAFONT_ALIGN_LEFT);
		renderer::print(128, 89 + renderer::font->texYSize, "ip: ");
		renderer::rect_fill(128 + 70, 90, 154, renderer::font->texYSize + 2, G2D_RGBA(75, 75, 75, 255));
		renderer::print(128 + 70 + 5, 89 + renderer::font->texYSize, ip);

		renderer::print(128, 95 + renderer::font->texYSize * 2, "username: ");
		renderer::rect_fill(128 + 70, 96 + renderer::font->texYSize, 154, renderer::font->texYSize + 2, G2D_RGBA(75, 75, 75, 255));
		renderer::print(128 + 70 + 5, 95 + renderer::font->texYSize * 2, usr);

		renderer::print(128, 101 + renderer::font->texYSize * 3, "password: ");
		renderer::rect_fill(128 + 70, 102 + renderer::font->texYSize * 2, 154, renderer::font->texYSize + 2, G2D_RGBA(75, 75, 75, 255));
		renderer::print(128 + 70 + 5, 101 + renderer::font->texYSize * 3, psw);
		/*8*/
		/*intraFontMeasureText(renderer::font, usr) + 2, renderer::font->texYSize + 2*/

		renderer::set_font_style(0.5f, 0xFFFFFFFF, 0x00000000, INTRAFONT_ALIGN_CENTER);
		renderer::rect_fill(128 + 70, 108 + renderer::font->texYSize * 4, 154, renderer::font->texYSize + 2, G2D_RGBA(75, 75, 75, 255));
		renderer::rect_fill(128, 108 + renderer::font->texYSize * 4, 154, renderer::font->texYSize + 2, G2D_RGBA(75, 75, 75, 255));
		renderer::print((115 + 125) - intraFontMeasureText(renderer::font, "login") + 18, 107 + renderer::font->texYSize * 5, "login");
		
		renderer::end();
	}
	
	renderer::quit();

	sceKernelExitGame();

	return 0;
}
