#include "Vertex.hpp"
#include "TextureUtil.h"

class Sprite{
public:
	Sprite();
	Sprite(Texture* tex);
	Sprite(Texture* tex, int offX, int offY, int extX, int extY);

	~Sprite();

	void SetPosition(float x, float y);
	void AddExtent(float x);
	void Scale(float x, float y);
	void Draw(int r = 255, int g = 255, int b = 255, int a = 255, int filtermin = GU_NEAREST, int filtermag = GU_NEAREST);

private:

	Vertex* vertices;

	Texture * texture;

	float scaleX, scaleY;
	float offX, offY;

	int width, height;
};