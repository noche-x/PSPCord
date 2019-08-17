#include "..//framework/state/AppState.hpp"
#include "..//framework/gfx/RenderUtil.h"

class LoginState : public AppState {
public:
	void init() {
		g_RenderUtil.setFonts(0.5f, 0xFFFFFFFF, 0x00000000, 0.f, INTRAFONT_ALIGN_LEFT);
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
	
	};

	void draw() {
		g_RenderUtil.printf(20, 20, "LoginState Draw Called!");
	};

private:
};