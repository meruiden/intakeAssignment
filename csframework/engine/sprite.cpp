#include <engine/sprite.h>


Sprite::Sprite()
{
	size = Vector2();
	textureSize = Vector2();
	uvOffset = Vector2();
	uvSize = Vector2(1, 1);
	filename = "";
	dynamicMesh = NULL;
	dynamicTexture = NULL;
}


Sprite::~Sprite()
{
	if (dynamicMesh != NULL)
	{
		delete dynamicMesh;
	}

	if (dynamicTexture != NULL)
	{
		delete dynamicTexture;
	}
}

void Sprite::setDynamics(Mesh* mesh, Texture* texture)
{
	this->dynamicMesh = mesh;
	this->dynamicTexture = texture;

}

void Sprite::setUvSize(Vector2 size)
{
	uvSize = size;
}

void Sprite::setUpSprite(std::string filename)
{
	this->filename = filename;
}

void Sprite::setSpriteSize(Vector2 size)
{
	this->size = size;
}

void Sprite::setUvOffset(Vector2 newOffset)
{
	this->uvOffset = newOffset;
}

void Sprite::setTextureSize(Vector2 size)
{
	this->textureSize = size;
}