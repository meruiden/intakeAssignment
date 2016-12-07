#pragma once

#include <string>
#include <vector>
#include <map>

#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <sstream>
#include <engine/texture.h>
#include <engine/mesh.h>
#include <engine/resourceloader.h>

class Texture;

class ResourceManager
{
public:
	virtual ~ResourceManager();
	
	static ResourceManager* getInstance();
	GLuint getEmptyTexture();
	Texture* getTexture(std::string path, bool &succes);
	Mesh* getSpriteMesh(int width, int height, float uvWidth, float uvHeight);
	Mix_Chunk* getLoadedWav(std::string path, bool &succes);
	TTF_Font* getLoadedFont(std::string path, bool &succes);
private:
	ResourceManager();

	void createEmptyTexture();

	std::map<std::string, Mesh*> loadedSpriteMeshes;
	std::map<std::string, Texture*> loadedTextures;
	std::map<std::string, Mix_Chunk*> loadedWavs;
	std::map<std::string, TTF_Font*> loadedFonts;

	GLuint emptyTexture;

	static ResourceManager* instance;
};

