#include "..//framework/state/AppState.hpp"
#include "..//framework/gfx/RenderUtil.h"

class ConnectingState : public AppState {
public:
	void init() {

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
		GuiPosition pos;
		Gui::begin(CENTERRIGHT, 200, 100);
		Gui::text("fuck");
		Gui::text("yeah");
		//g_RenderUtil.printf(20, 20, "ConnectingState Draw Called!");
	};

private:
};