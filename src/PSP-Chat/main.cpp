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
#include "framework/gfx/Sprite.h"
#include "framework/state/StateManager.hpp"
#include "states/LoginState.hpp"
#include "states/ConnectingState.hpp"
#include "framework/utils/Input.h"

PSP_MODULE_INFO("PSP CHAT", PSP_MODULE_USER, 1, 0);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER); 
PSP_HEAP_SIZE_MAX();

int main(int argc, char* argv[])
{
	setupExitCallback();
	
	g_RenderUtil.init();
	g_RenderUtil.initFonts("data/font.pgf");

	//g_AudioManager.Init();

	LoginState* loginState;
	ConnectingState* connectingState;
	g_StateManager.pushState(loginState);

	//Texture* bg_texture = TextureUtil::LoadPng("data/background.png");

	while (1) {
		//g_StateManager.update();
		//input.Update();

		g_RenderUtil.frameBegin();
		g_RenderUtil.frameClear();
		
		//Sprite bg(bg_texture);

		g_StateManager.draw();

		g_RenderUtil.frameEnd();

		//if (input.isActionDown(PSP_CROSS))
			//g_StateManager.pushState(connectingState);
	}
	

	sceKernelExitGame();

	return 0;
}
