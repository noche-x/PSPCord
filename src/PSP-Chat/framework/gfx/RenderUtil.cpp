#include "RenderUtil.h"
#include "../utils/vram.h"
#include <stdio.h>
#include <stdarg.h>

void RenderUtil::init()
{

	void* fbp0 = getStaticVramBuffer(BUF_WIDTH, SCR_HEIGHT, GU_PSM_8888);
	void* fbp1 = getStaticVramBuffer(BUF_WIDTH, SCR_HEIGHT, GU_PSM_8888);
	void* zbp = getStaticVramBuffer(BUF_WIDTH, SCR_HEIGHT, GU_PSM_4444);

	sceGuInit();

	sceGuStart(GU_DIRECT, list);
	sceGuDrawBuffer(GU_PSM_8888, fbp0, BUF_WIDTH);
	sceGuDispBuffer(SCR_WIDTH, SCR_HEIGHT, fbp1, BUF_WIDTH);
	sceGuDepthBuffer(zbp, BUF_WIDTH);
	sceGuOffset(2048 - (SCR_WIDTH / 2), 2048 - (SCR_HEIGHT / 2));
	sceGuViewport(2048, 2048, SCR_WIDTH, SCR_HEIGHT);
	sceGuDepthRange(65535, 0);
	sceGuScissor(0, 0, SCR_WIDTH, SCR_HEIGHT);
	sceGuEnable(GU_SCISSOR_TEST);
	sceGuDepthFunc(GU_GEQUAL);
	sceGuEnable(GU_DEPTH_TEST);
	sceGuFrontFace(GU_CCW);
	sceGuShadeModel(GU_SMOOTH);
	sceGuEnable(GU_CULL_FACE);
	sceGuEnable(GU_TEXTURE_2D);
	sceGuEnable(GU_CLIP_PLANES);
	sceGuEnable(GU_LINE_SMOOTH);

	sceGuEnable(GU_BLEND);
	sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);

	sceGuStencilFunc(GU_ALWAYS, 1, 1);
	sceGuStencilOp(GU_KEEP, GU_KEEP, GU_REPLACE);

	sceGuTexFilter(GU_LINEAR, GU_LINEAR);

	
	sceGuFinish();
	sceGuSync(0, 0);

	sceDisplayWaitVblankStart();
	sceGuDisplay(GU_TRUE);
	

	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

	//Sets up orthogonal drawing

	sceGumMatrixMode(GU_PROJECTION);
	sceGumLoadIdentity();
	sceGumOrtho(0, 480, 272, 0, -30, 30);

	sceGumMatrixMode(GU_VIEW);
	sceGumLoadIdentity();

	sceGumMatrixMode(GU_MODEL);
	sceGumLoadIdentity();
}

void RenderUtil::frameBegin(bool dialog)
{
	if (dialog) {
		sceGuStart(GU_DIRECT, list);
		sceGuClear(GU_COLOR_BUFFER_BIT | GU_DEPTH_BUFFER_BIT);
		sceGuFinish();
		sceGuSync(0, 0);
	}
	else {
		sceGuStart(GU_DIRECT, list);

		sceGuClearColor(clearColor);
		sceGuClearStencil(0);
		sceGuClearDepth(0);
		sceGuClear(GU_COLOR_BUFFER_BIT | GU_STENCIL_BUFFER_BIT | GU_DEPTH_BUFFER_BIT);
	}
}

void RenderUtil::frameEnd(bool dialog)
{
	if (dialog) {
		sceDisplayWaitVblankStart();
		sceGuSwapBuffers();
	}
	else {
		sceGuFinish();
		sceGuSync(0, 0);

		sceDisplayWaitVblankStart();
		sceGuSwapBuffers();
	}
}

void RenderUtil::frameClear()
{
	sceGuClearColor(clearColor);
	sceGuClearStencil(0);
	sceGuClearDepth(0);
	sceGuClear(GU_COLOR_BUFFER_BIT | GU_STENCIL_BUFFER_BIT | GU_DEPTH_BUFFER_BIT);
}

void RenderUtil::setClearColor(int r, int g, int b, int a)
{
	clearColor = GU_RGBA(r, g, b, a);
}

void RenderUtil::initFonts(const char * path)
{
	intraFontInit();
	font = intraFontLoad(path, INTRAFONT_CACHE_ASCII);
	intraFontSetStyle(font, 0.5f, 0xFFFFFFFF, 0x00000000, 0.0f, INTRAFONT_ALIGN_LEFT);
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

	sceGuEnable(GU_BLEND);
	sceGuEnable(GU_TEXTURE_2D);
	intraFontPrint(font, x, y, cbuffer);
	sceGuDisable(GU_BLEND);
	sceGuDisable(GU_TEXTURE_2D);
}

RenderUtil g_RenderUtil = RenderUtil();
