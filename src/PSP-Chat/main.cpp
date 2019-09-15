#include "framework/utils/common.h"

#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspctrl.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include "framework/audio/AudioManager.hpp"
#include "framework/gfx/RenderUtil.h"
#include "framework/state/StateManager.hpp"
#include "states/LoginState.hpp"
#include "states/ConnectingState.hpp"
#include "framework/utils/Input.h"
#include "framework/gfx/intrafont/glib2d.h"

PSP_MODULE_INFO("PSP CHAT", PSP_MODULE_USER, 1, 0);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER); 
PSP_HEAP_SIZE_MAX();

int main(int argc, char* argv[])
{
	setupExitCallback();
	
	g_RenderUtil.init("flash0:/font/ltn0.pgf");

	LoginState* loginState = new LoginState();
	ConnectingState* connectingState = new ConnectingState();
	loginState->init();
	connectingState->init();

	g_StateManager.changeState(loginState);

	g2dTexture* bg = g2dTexLoad("data/background.png", G2D_SWIZZLE);
	
	while (isRunning()) {
		g_StateManager.update();

		g_RenderUtil.frameBegin();
		g_RenderUtil.frameClear(G2D_RGBA(80, 80, 80, 255));

		g_RenderUtil.image(bg, 0, 0);

		g_StateManager.draw();
		
		g_RenderUtil.frameEnd();

	}
	

	sceKernelExitGame();

	return 0;
}
