#pragma once

#include <engine/hudelement.h>
#include <engine/resourcemanager.h>

class HudText : public HudElement
{
public:
	HudText();
	virtual ~HudText();

	bool loadFont(std::string path);
	void setText(std::string text);
	std::string getText() { return text; }

private:
	TTF_Font* font;
	SDL_Surface* fontSurface;
	std::string text;
	std::string lastText;
};

