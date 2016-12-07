#pragma once

#include <SDL.h>
#include <SDL_ttf.h>

#include <engine/config.h>

#include <iostream>
#include <string>
class ResourceLoader
{
public:
	static TTF_Font* getFont(std::string path, bool &succes);
};

