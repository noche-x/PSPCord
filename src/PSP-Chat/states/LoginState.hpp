#include "..//framework/state/AppState.hpp"
#include "..//framework/gfx/RenderUtil.h"
#include "..//framework/gfx/gui/Gui.h"

class LoginState : public AppState {
public:
	void init() {
		Gui::init(0.5f, 0xFFFFFFFF, 0x00000000, 0.f, INTRAFONT_ALIGN_LEFT);
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
		Gui::begin(CENTER, 315, 215);
		Gui::text("Please enter your details and the server's IP");
		Gui::textInput("ip");
		Gui::textInput("username");
		Gui::textInput("password");
		Gui::button("login");

		for (int i = 0; i < Gui::textInputArray.size(); i++) {
			textInputText text = Gui::textInputArray.at(i);
			Gui::text("textInputArray : %i : %s", text.itemCount, text.text);
		}

		//Gui::text("selectedItem : %i", Gui::selectedItem);
		//Gui::text("itemCount : %i", Gui::itemCount);
		//Gui::text("selectableItemCount : %i", Gui::selectableItemCount);

	};

private:
};