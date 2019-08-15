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
#include "intraFont/intraFont.h"


#define BUF_WIDTH (512)
#define SCR_WIDTH (480)
#define SCR_HEIGHT (272)

class RenderUtil {
public:
	
	//Rendering Engine
	void init();

	void frameBegin(bool dialog = false);
	void frameEnd(bool dialog = false);

	void frameClear();
	void setClearColor(int r, int g, int b, int a);


	//Font Engine
	void initFonts(const char* path);
	void setFonts(float size, unsigned int color, unsigned int shadowColor, float angle, unsigned int options);

	void print(int x, int y, const char* message);
	void printf(int x, int y, const char *message, ...);

private:
	unsigned int __attribute__((aligned(16))) list[262144];
	unsigned int clearColor;
	intraFont* font;
};

extern RenderUtil g_RenderUtil;
