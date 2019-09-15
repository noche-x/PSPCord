#include "..//RenderUtil.h"
#include "..//..//utils/Input.h"
#include <vector>
#include <string>

enum GuiPosition {
	TOPLEFT = 0,
	TOPCENTER = 1,
	TOPRIGHT = 2,
	CENTERLEFT = 3,
	CENTER = 4,
	CENTERRIGHT = 5,
	BOTTOMLEFT = 6,
	BOTTOMCENTER = 7,
	BOTTOMRIGHT = 8,
};

int onScreenKeyboard(unsigned short* descritpion, unsigned short* outtext, int maxtextinput) {

	//osk params
	SceUtilityOskData oskData;
	SceUtilityOskParams oskParams;

	//init osk data
	memset(&oskData, 0, sizeof(SceUtilityOskData));
	oskData.language = PSP_UTILITY_OSK_LANGUAGE_DEFAULT; // Use system default for text input
	oskData.lines = 1;
	oskData.unk_24 = 1;
	oskData.inputtype = PSP_UTILITY_OSK_INPUTTYPE_ALL; // Allow all input types
	oskData.desc = descritpion;
	//oskData.intext = intext[i];
	oskData.outtextlength = maxtextinput;
	oskData.outtextlimit = 32; // Limit input to 32 characters
	oskData.outtext = outtext;

	//init osk dialog
	memset(&oskParams, 0, sizeof(SceUtilityOskParams));
	oskParams.base.size = sizeof(oskParams);
	sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_LANGUAGE, &oskParams.base.language);
	sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_UNKNOWN, &oskParams.base.buttonSwap);
	oskParams.base.graphicsThread = 17;
	oskParams.base.accessThread = 19;
	oskParams.base.fontThread = 18;
	oskParams.base.soundThread = 16;
	oskParams.datacount = 1;
	oskParams.data = &oskData;

	sceUtilityOskInitStart(&oskParams);

	bool done = true;

	while (done)
	{
		g2dClear(G2D_RGBA(39, 50, 56, 255));

		sceGuFinish();
		sceGuSync(0, 0);

		switch (sceUtilityOskGetStatus())
		{
		case PSP_UTILITY_DIALOG_INIT:
			break;

		case PSP_UTILITY_DIALOG_VISIBLE:
			sceUtilityOskUpdate(1);
			break;

		case PSP_UTILITY_DIALOG_QUIT:
			sceUtilityOskShutdownStart();
			break;

		case PSP_UTILITY_DIALOG_FINISHED:
			break;

		case PSP_UTILITY_DIALOG_NONE:
			done = false;

		default:
			break;
		}

		g2dFlip(G2D_VSYNC);
	}

	if (oskData.result == PSP_UTILITY_OSK_RESULT_CANCELLED)
		return -1;

	return 0;

}

struct textInputText
{
	int itemCount;
	const char* text;
};

namespace Gui {
	int x = 0, y = 0;
	int itemPaddingY = 5;
	int itemPaddingX = 7;
	int itemSpacingFactor = 20;

	int itemCount = 0;
	int selectableItemCount = 0;
	bool selectableItemActive = false;

	int m_width = 0;
	int m_height = 0;

	int selectedItem = 0;

	float textSize = 0;
	unsigned int textColor = 0;
	unsigned int textShadowColor = 0;
	float textAngle = 0;
	unsigned int textOptions = 0;

	std::vector<textInputText> textInputArray;

	namespace Colors {
		int backGround = 0xFF515151,
			header = 0xFF757575,
			text = 0xFFE0E0E0,
			foreGround = 0xFF828282,
			contrast = 0xFFFFD8A4;
	};

	void init(float tSize, unsigned int tColor, unsigned int tShadowColor, float tAngle, unsigned int tOptions) {
		textSize = tSize;
		textColor = tColor;
		textShadowColor = tShadowColor;
		textAngle = tAngle;
		textOptions = tOptions;

		selectedItem = 0;
	}

	void setTextStyle(float size = textSize, unsigned int color = textColor, unsigned int shadowColor = textShadowColor, float angle = textAngle, unsigned int options = textOptions) {
		g_RenderUtil.setFonts(size, color, shadowColor, angle, options);
	}

	void begin(GuiPosition pos, int width, int height) {
		switch (pos)
		{
		case TOPLEFT:
			x = 0, y = 0;
			break;
		case TOPCENTER:
			x = (480 / 2) - (width / 2),
				y = 0;
			break;
		case TOPRIGHT:
			x = 480 - width,
				y = 0;
			break;
		case CENTERLEFT:
			x = 0,
				y = (272 / 2) - (height / 2);
			break;
		case CENTER:
			x = (480 / 2) - (width / 2),
				y = (272 / 2) - (height / 2);
			break;
		case CENTERRIGHT:
			x = 480 - width,
				y = (272 / 2) - (height / 2);
			break;
		case BOTTOMLEFT:
			x = 0,
				y = 272 - height;
			break;
		case BOTTOMCENTER:
			x = (480 / 2) - (width / 2),
				y = 272 - height;
			break;
		case BOTTOMRIGHT:
			x = 480 - width,
				y = 272 - height;
			break;
		default:
			x = 0, y = 0;
			break;
		}

		m_width = width;
		m_height = height;

		g_RenderUtil.rect(x, y, width, height, Colors::backGround);
		Gui::setTextStyle(textSize);
	}

	void text(const char* message, ...) {
		itemCount++;
		va_list argList;
		char cbuffer[512];
		va_start(argList, message);
		vsnprintf(cbuffer, 512, message, argList);
		va_end(argList);

		g_RenderUtil.print(x + itemPaddingX, (y + itemPaddingY) + (itemCount * itemSpacingFactor), cbuffer);
	}

	void update() {
		input.Update();

		if (input.isActionDown(PSP_DOWN))
			selectedItem++;

		if (input.isActionDown(PSP_UP))
			selectedItem--;

		if (input.isActionDown(PSP_CROSS))
			selectableItemActive = true;

		if (selectedItem >= selectableItemCount)
			selectedItem = 0;

		if (selectedItem < 0)
			selectedItem = selectableItemCount - 1;

		itemCount = 0;
		selectableItemCount = 0;
	}

	bool button(const char* label) {
		int currentItem = selectableItemCount;
		itemCount++;
		selectableItemCount++;

		int selected = Colors::contrast;
		if (currentItem == selectedItem) {
			selected = 0xFFF9C804;
		}

		g_RenderUtil.rect(x + itemPaddingX, (y + itemPaddingY - 12) + (itemCount * itemSpacingFactor), m_width - 14, 18, selected);

		Gui::setTextStyle(textSize, textColor, textShadowColor, textAngle, INTRAFONT_ALIGN_CENTER);
		g_RenderUtil.printf(x + (m_width / 2), (y + itemPaddingY) + (itemCount * itemSpacingFactor), label);
		Gui::setTextStyle(textSize, textColor, textShadowColor, textAngle, INTRAFONT_ALIGN_LEFT);

		return false;
	}

	void textInput(const char* label) {
		int currentItem = selectableItemCount;
		itemCount++;
		selectableItemCount++;

		int selected = 0;
		if (currentItem == selectedItem) {
			selected = 0x00202020;
		}

		std::string field = "";

		if (!textInputArray.empty()) {
			for (size_t i = 0; i < textInputArray.size(); i++)
				if (itemCount == textInputArray.at(i).itemCount) {
					field = textInputArray.at(i).text;
				}
		}

		if (currentItem == selectedItem && selectableItemActive == true) {
			unsigned short temp[128];

			unsigned short* desc = (unsigned short*)strtol(label, NULL, 0);
			//unsigned short desc[10] = { 'I','p',' ',' ',' ',' ',' ',' ', ' ', ' ' };
			std::string conv = "";
			if (onScreenKeyboard(desc, temp, 128) != -1)
			{
				for (int j = 0; temp[j]; j++)
				{
					unsigned c = temp[j];

					if (32 <= c && c <= 127) // print ascii only
						conv += c;
				}

				field = conv;
			}
			selectableItemActive = false;
			if (!textInputArray.empty())
				for (size_t i = 0; i < textInputArray.size(); i++)
					if (itemCount == textInputArray.at(i).itemCount)
						textInputArray.erase(textInputArray.begin() + i);

			textInputText text;
			text.itemCount = itemCount;
			text.text = conv.c_str();
			textInputArray.push_back(text);
		}
		
		std::string display = "";

		if (field == "")
			display = label;
		else 
			display = field;
		
		g_RenderUtil.rect(x + itemPaddingX, (y + itemPaddingY - 12) + (itemCount * itemSpacingFactor), m_width - 14, 18, Colors::foreGround + selected);
		g_RenderUtil.printf(x + itemPaddingX + 4, (y + itemPaddingY) + (itemCount * itemSpacingFactor), display.c_str());
	}


}