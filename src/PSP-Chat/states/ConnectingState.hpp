#pragma once

#include "..//Globals.h"
#include "..//framework/state/AppState.hpp"
#include "..//framework/gfx/RenderUtil.h"

class ConnectingState : public AppState {
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
		returnValue = 0;
	};

	void resume() {
		returnValue = 0;
		sceKernelDelayThread(100000);
	};

	void update() {
		Gui::selectableItemActive = false;
		Gui::update();
	};

	void draw() {
		Gui::begin(CENTER, 315, 215);

		Gui::text("%s %s %s", globalVariables::_ip, globalVariables::_username, globalVariables::_password);

		Gui::text("%s", Gui::selectableItemActive ? "true" : "false");

		if (Gui::button("fuck yeaj")) {
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