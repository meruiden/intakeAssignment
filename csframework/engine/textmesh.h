#pragma once

#include <engine/entity.h>
#include <engine/resourcemanager.h>

class TextMesh : public Entity
{
public:
	TextMesh();
	virtual ~TextMesh();
	virtual void update(float deltaTime);

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

