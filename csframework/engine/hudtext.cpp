#include <engine/hudtext.h>

HudText::HudText()
{
	font = NULL;
	fontSurface = NULL;
	sprite = new Sprite();
	multiline = false;
}

void HudText::setText(std::string text)
{
	this->text = text;

	if (text == lastText) {
		return;
	}

	if (sprite->hasDynamicMesh())
	{
		delete sprite->getDynamicMesh();
	}

	if (sprite->hasDynamicTexture())
	{
		delete sprite->getDynamicTexture();
	}
	sprite->setDynamics(NULL, NULL);

	if (text == "")
	{
		return;
	}

	GLuint textureBuffer;
	glGenTextures(1, &textureBuffer);
	glBindTexture(GL_TEXTURE_2D, textureBuffer);
	SDL_Color c = { 255, 255, 255, 0 };
	if (multiline)
	{
		fontSurface = TTF_RenderText_Blended_Wrapped(font, text.c_str(), c, (int)text.length() * (int)FONT_SIZE);
	}
	else
	{
		fontSurface = TTF_RenderText_Blended(font, text.c_str(), c);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fontSurface->w, fontSurface->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, fontSurface->pixels);

	Mesh* mesh = new Mesh();
	Texture* texture = new Texture();
	mesh->setAsSquare(fontSurface->w, fontSurface->h, 1.0f, 1.0f);
	texture->setPreloaded(fontSurface->w, fontSurface->h, textureBuffer);
	sprite->setDynamics(mesh, texture);

	SDL_FreeSurface(fontSurface);
	fontSurface = NULL;

	lastText = text;
}

void HudText::pullTextInput()
{
	std::string curText = getText();
	if (input()->getKeyDown(SDLK_BACKSPACE))
	{
		if (curText.length() > 0)
		{
			curText.pop_back();
		}
	}
	if (input()->getKeyDown(SDLK_RETURN) && multiline)
	{
		curText += "\n";
	}
	if (input()->getTextInput() != "")
	{
		curText += input()->getTextInput();
	}
	setText(curText);
}

bool HudText::loadFont(std::string path)
{
	bool loadSucces = false;
	font = ResourceManager::getInstance()->getLoadedFont(path, loadSucces);

	return loadSucces;
}



HudText::~HudText()
{
	if (fontSurface != NULL) {
		SDL_FreeSurface(fontSurface);
		fontSurface = NULL;
	}

}
