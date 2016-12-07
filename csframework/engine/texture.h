#pragma once

#include <GL/glew.h>
#include <SDL.h>
#include <SDL_image.h>
#include <engine/resourcemanager.h>

class Texture
{
public:
	Texture();
	~Texture();

	GLuint getTextureBuffer() { return textureBuffer; }

	int getWidth() { return width; }
	int getHeight() { return height; }

	bool loadTexture(std::string path);

	void setPreloaded(int width, int height, GLuint textureBuffer);
private:
	GLuint textureBuffer;
	int width;
	int height;
	unsigned char* pixels[];
};

