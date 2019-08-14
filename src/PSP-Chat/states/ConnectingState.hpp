#include "..//framework/state/AppState.hpp"
#include "..//framework/gfx/RenderUtil.h"
#include "..//framework/gfx/Texture.h"
#include "..//framework/gfx/TextureUtil.h"

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

	};

	void draw() {
		g_RenderUtil.printf(20, 20, "ConnectingState Draw Called!");
	};

private:
};