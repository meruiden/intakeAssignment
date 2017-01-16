#pragma once

#include <engine/hudelement.h>
#include <engine/resourcemanager.h>
#include <engine/csutils.h>

class HudText : public HudElement
{
public:
	HudText();
	virtual ~HudText();

	
	void setText(std::string text);
	void setMultiLine(bool value);
	void pullTextInput();

	bool loadFont(std::string path);
	bool isMultiLine() { return multiline; }
	std::string getText() { return text; }

private:
	TTF_Font* font;
	SDL_Surface* fontSurface;
	std::string text;
	std::string lastText;
	bool multiline;
};

