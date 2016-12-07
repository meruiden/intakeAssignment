#pragma once

#include <engine/entity.h>
#include <engine/resourcemanager.h>

class TextMesh : public Entity
{
public:
	TextMesh();
	virtual ~TextMesh();
	virtual void update(float deltaTime);

	bool loadFont(std::string path);
	void setText(std::string text);
	std::string getText() { return text; }

private:
	TTF_Font* font;
	SDL_Surface* fontSurface;
	std::string text;
	std::string lastText;

};

