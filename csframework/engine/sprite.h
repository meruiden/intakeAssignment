#pragma once

#include <engine/mesh.h>
#include <engine/texture.h>
#include <engine/vector2.h>

class Sprite
{
public:
	Sprite();
	virtual ~Sprite();
	
	void setUpSprite(std::string filename);
	void setSpriteSize(Vector2 size);
	void setUvOffset(Vector2 newOffset);
	void setTextureSize(Vector2 size);
	void setUvSize(Vector2 size);

	Vector2 getUvOffset() { return uvOffset; }
	Vector2 getSpriteSize() { return size; }
	Vector2 getTextureSize() { return textureSize; }
	Vector2 getUvSize() { return uvSize; }

	std::string getFileName() { return filename; }

	void setDynamics(Mesh* mesh, Texture* texture);

	Mesh* getDynamicMesh() { return dynamicMesh; }
	Texture* getDynamicTexture() { return dynamicTexture; }

	bool hasDynamicMesh() { return (dynamicMesh != NULL); }
	bool hasDynamicTexture() { return (dynamicTexture != NULL); }
private:
	std::string filename;

	Vector2 uvSize;
	Vector2 uvOffset;
	Vector2 size;
	Vector2 textureSize;

	Mesh* dynamicMesh;
	Texture* dynamicTexture;

};

