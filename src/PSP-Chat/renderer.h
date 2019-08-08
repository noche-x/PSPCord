#ifndef RENDER_H
#define RENDER_H

#include "common.h"
#include <stdio.h>
#include <stdarg.h>
#include <pspgu.h>
#include <psppower.h>
#include <psprtc.h>
#include <pspnet_apctl.h>
#include "intrafont/intraFont.h"
#include "intrafont/glib2d.h"

/// Checks whether a result code indicates success.
#define R_SUCCEEDED(res)   ((res) >= 0)
/// Checks whether a result code indicates failure.
#define R_FAILED(res)      ((res) < 0)

static char* get_time(void) {
	static char buffer[15];
	pspTime time;

	if (R_SUCCEEDED(sceRtcGetCurrentClockLocalTime(&time))) {
		bool amOrPm = false;

		if (time.hour < 12)
			amOrPm = true;
		if (time.hour == 0)
			time.hour = 12;
		else if (time.hour > 12)
			time.hour = time.hour - 12;

		if ((time.hour >= 1) && (time.hour < 10))
			snprintf(buffer, 15, "%2i:%02i %s", time.hour, time.minutes, amOrPm ? "AM" : "PM");
		else
			snprintf(buffer, 15, "%2i:%02i %s", time.hour, time.minutes, amOrPm ? "AM" : "PM");
	}

	return buffer;
}

namespace renderer {
	intraFont* font = NULL;

	namespace textures {
		g2dTexture* background = NULL;
		g2dTexture* logo = NULL;
	};
	
	inline int keyboard(unsigned short* descritpion, unsigned short* outtext, int maxtextinput)
	{
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

	inline void rect_fill(float x, float y, float width, float height, g2dColor color) {
		g2dBeginRects(NULL);
		g2dSetColor(color);
		g2dSetCoordMode(G2D_UP_LEFT);
		g2dSetScaleWH(width, height);
		g2dSetCoordXY(x, y);
		g2dAdd();
		g2dEnd();
	}
	
	inline void init() {
		intraFontInit();
		font = intraFontLoad("data/font.pgf", INTRAFONT_CACHE_ALL);
		intraFontSetStyle(font, 0.5f, 0xFFFFFFFF, 0x00000000, 0, INTRAFONT_ALIGN_LEFT);
		renderer::font->texYSize = renderer::font->texYSize - 6;

		textures::background = g2dTexLoad("data/background.png", G2D_SWIZZLE);
		textures::logo = g2dTexLoad("data/logo.png", G2D_SWIZZLE);
	};

	inline void image(g2dTexture* tex, float x, float y) {
		g2dBeginRects(tex);
		g2dSetCoordMode(G2D_UP_LEFT);
		g2dSetCoordXY(x, y);
		g2dAdd();
		g2dEnd();
	}

	inline void begin() {
		g2dClear(G2D_RGBA(80, 80, 80, 255));
		image(textures::background, 0, 0);
		image(textures::logo, 318, 210);
		rect_fill(0, 0, 480, 15, G2D_RGBA(50, 50, 50, 255));
		set_font_style();
		intraFontPrint(font, 0, 0 + font->texYSize, get_time());
	}

	inline void set_font_style(float size = 0.5f, unsigned int color = 0xFFFFFFFF, unsigned int shadow = 0x00000000, unsigned int opt = INTRAFONT_ALIGN_LEFT) {
		intraFontSetStyle(font, size, color, shadow, 0, opt);
	}

	inline void end() {
		g2dFlip(G2D_VSYNC);
	}

	inline void quit() {
		intraFontShutdown();
	};

	inline void clear() {
		g2dClear(G2D_RGBA(80, 80, 80, 255));
	};

	inline void print(int x, int y, const char* message, ...) {

		intraFontPrint(font, x, y, message);
	};

}

#endif