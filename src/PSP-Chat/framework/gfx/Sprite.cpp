#include "Sprite.h"
#include <malloc.h>
#include <pspkernel.h>
#include <psptypes.h>

Vertex genVertex(float u, float v, float x, float y, float z)
{
	Vertex vert;
	vert.u = u;
	vert.v = v;
	vert.x = x;
	vert.y = y;
	vert.z = z;
	return vert;
}

Sprite::Sprite(Texture * tex)
{
	texture = tex;
	vertices = (Vertex*)memalign(16, 4 * sizeof(Vertex));

	width = tex->width;
	height = tex->height;

	float hPercent = (float)tex->height / (float)tex->pHeight;
	float wPercent = (float)tex->width / (float)tex->pWidth;

	if (vertices)
	{
		vertices[0] = genVertex(0.0f, 0.0f, -tex->width / 2, -tex->height / 2, 0.0f);
		vertices[1] = genVertex(0.0f, hPercent, -tex->width / 2, tex->height / 2, 0.0f);
		vertices[2] = genVertex(wPercent, 0.0f, tex->width / 2, -tex->height / 2, 0.0f);
		vertices[3] = genVertex(wPercent, hPercent, tex->width / 2, tex->height / 2, 0.0f);
	}

	sceKernelDcacheWritebackInvalidateRange(vertices, 4 * sizeof(Vertex));

}

Sprite::Sprite(Texture * tex, int offX, int offY, int extX, int extY)
{
	texture = tex;

	//generate wertices
	vertices = (Vertex*)memalign(16, 4 * sizeof(Vertex));

	width = extX;
	height = extY;

	float hstart = (float)offY / (float)tex->pHeight;
	float wstart = (float)offX / (float)tex->pWidth;
	float hPercent = (float)(offY + extY) / (float)tex->pHeight;
	float wPercent = (float)(offX + extX) / (float)tex->pWidth;

	if (vertices)
	{
		vertices[0] = genVertex(wstart, hstart, -width / 2, -height / 2, 0.0f);
		vertices[1] = genVertex(wstart, hPercent, -width / 2, height / 2, 0.0f);
		vertices[2] = genVertex(wPercent, hstart, width / 2, -height / 2, 0.0f);
		vertices[3] = genVertex(wPercent, hPercent, width / 2, height / 2, 0.0f);
	}

	sceKernelDcacheWritebackInvalidateRange(vertices, 4 * sizeof(Vertex));
}

Sprite::~Sprite()
{
	free(vertices);

}

void Sprite::SetPosition(float x, float y)
{
	offX = x;
	offY = y;
}

void Sprite::AddExtent(float x)
{
	vertices[2].x += x;
	vertices[3].x += x;
}

void Sprite::Scale(float x, float y)
{
	scaleX = x;
	scaleY = y;

	for (int i = 0; i<4; i++)
	{
		vertices[i].x = vertices[i].x * scaleX;
		vertices[i].y = vertices[i].y * scaleY;
	}
}

void Sprite::Draw(int r, int g, int b, int a, int filtermin, int filtermag)
{
	sceGumPushMatrix();

	ScePspFVector3 loc = { offX,offY,0.0f };
	sceGumTranslate(&loc);

	sceGuEnable(GU_TEXTURE_2D);
	sceGuEnable(GU_BLEND);

	sceGuColor(GU_RGBA(r, g, b, a)); //Modulate to whatever set color
	texture->bindTexture(filtermin, filtermag, true);

	sceGumDrawArray(GU_TRIANGLE_STRIP, GU_TEXTURE_32BITF | GU_VERTEX_32BITF | GU_TRANSFORM_3D, 4, 0, vertices);

	sceGuDisable(GU_BLEND);
	sceGuDisable(GU_TEXTURE_2D);
	sceGumPopMatrix();
}
