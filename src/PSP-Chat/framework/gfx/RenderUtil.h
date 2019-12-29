#pragma once

#include <pspgu.h>
#include <pspgum.h>
#include <pspctrl.h>
#include <pspkernel.h>
#include <pspdisplay.h>
#include <pspdebug.h>
#include <psppower.h>
#include <psptypes.h>
#include <stdarg.h>
#include <stdio.h>
#include "intrafont/glib2d.h"
#include "intrafont/intraFont.h"


#define BUF_WIDTH (512)
#define SCR_WIDTH (480)
#define SCR_HEIGHT (272)

class RenderUtil {
public:
	
	//Rendering Engine
	void init(const char* path);

	void image(g2dTexture* tex, float x, float y);

	void frameBegin();
	void frameEnd();

	void frameClear(g2dColor color);

	//Font Engine
	void setFonts(float size, unsigned int color, unsigned int shadowColor, float angle, unsigned int options);

	void print(int x, int y, const char *message);

	void printf(int x, int y, const char *message, ...);

	void rect(float x, float y, float width, float height, g2dColor color);

	float calculateTextLength(const char* message);
private:
	unsigned int __attribute__((aligned(16))) list[262144];
	unsigned int clearColor;
	intraFont* font;
};

extern RenderUtil g_RenderUtil;
