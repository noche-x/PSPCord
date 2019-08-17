#include "RenderUtil.h"
#include <stdio.h>
#include <stdarg.h>

void RenderUtil::init(const char* path)
{
	intraFontInit();
	font = intraFontLoad("data/font.pgf", INTRAFONT_CACHE_ALL);
	intraFontSetStyle(font, 0.5f, 0xFFFFFFFF, 0x00000000, 0, INTRAFONT_ALIGN_LEFT);
	font->texYSize = font->texYSize - 6;
}

void RenderUtil::image(g2dTexture* tex, float x, float y)
{
	g2dBeginRects(tex);
	g2dSetCoordMode(G2D_UP_LEFT);
	g2dSetCoordXY(x, y);
	g2dAdd();
	g2dEnd();
}

void RenderUtil::frameBegin()
{
	g2dClear(G2D_RGBA(80, 80, 80, 255));
	//image(textures::background, 0, 0);
	//image(textures::logo, 318, 210);
	//rect_fill(0, 0, 480, 15, G2D_RGBA(50, 50, 50, 255));
	//intraFontPrint(font, 0, 0 + font->texYSize, get_time());
}

void RenderUtil::frameEnd()
{
	g2dFlip(G2D_VSYNC);
}

void RenderUtil::frameClear(g2dColor color)
{
	g2dClear(color);
}

void RenderUtil::setFonts(float size, unsigned int color, unsigned int shadowColor, float angle, unsigned int options)
{
	intraFontSetStyle(font, size, color, shadowColor, angle, options);
}

void RenderUtil::printf(int x, int y, const char * message, ...)
{
	va_list argList;
	char cbuffer[512];
	va_start(argList, message);
	vsnprintf(cbuffer, 512, message, argList);
	va_end(argList);

	intraFontPrint(font, x, y, cbuffer);
}

RenderUtil g_RenderUtil = RenderUtil();
