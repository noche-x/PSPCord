#include "..//framework/state/AppState.hpp"
#include "..//framework/gfx/RenderUtil.h"
#include "..//framework/gfx/Texture.h"
#include "..//framework/gfx/TextureUtil.h"

class LoginState : public AppState {
public:
	void init() {
		g_RenderUtil.setFonts(1.f, 0x00000000, 0xFFFFFFFF, 0.f, INTRAFONT_CACHE_ALL);
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