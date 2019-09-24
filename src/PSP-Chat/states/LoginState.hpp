#pragma once

#include "..//framework/state/AppState.hpp"
#include "..//framework/gfx/RenderUtil.h"
#include "..//framework/gfx/gui/Gui.h"
#include "..//Globals.h"
#include "../framework/utils/Timer.h"

class LoginState : public AppState {
public:
	void init() {
		Gui::init(0.5f, 0xFFFFFFFF, 0x00000000, 0.f, INTRAFONT_ALIGN_LEFT);
		returnValue = 0;
		sceKernelDelayThread(100000);
	};

	void enter() {

	};

	void shutdown() {

	};

	void pause() {

	};

	void resume() {
		
	};

	void update() {
		Gui::update();
	};

	void draw() {
		Gui::begin(CENTER, 315, 215);
		
		Gui::text("Please enter your details and the server's IP");
		
		static char ip[25];
		static char username[25];
		static char password[25];

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
			returnValue = 1;
		}
	};

	int returnVal() {
		return returnValue;
	}
private:
	int returnValue;
};
