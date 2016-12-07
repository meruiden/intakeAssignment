#include <engine/textmesh.h>

TextMesh::TextMesh() : Entity()
{
	font = NULL;
	fontSurface = NULL;
	sprite = new Sprite();
}

void TextMesh::update(float deltaTime) 
{
		
}

void TextMesh::setText(std::string text) 
{
	this->text = text;

	if (text == lastText) {
		return;
	}
	if (sprite->getDynamicMesh() != NULL)
	{
		delete sprite->getDynamicMesh();
	}

	if (sprite->getDynamicTexture() != NULL)
	{
		delete sprite->getDynamicTexture();
	}

	sprite->setDynamics(NULL, NULL);
	GLuint textureBuffer;
	glGenTextures(1, &textureBuffer);
	glBindTexture(GL_TEXTURE_2D, textureBuffer);
	SDL_Color c = { 255, 255, 255, 0 };
	fontSurface = TTF_RenderText_Blended_Wrapped(font, text.c_str(), c, (int)text.length() * (int)FONT_SIZE);
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

bool TextMesh::loadFont(std::string path)
{
	bool loadSucces = false;
	font = ResourceManager::getInstance()->getLoadedFont(path, loadSucces);

	return loadSucces;
}


TextMesh::~TextMesh()
{
	if (fontSurface != NULL) {
		SDL_FreeSurface(fontSurface);
		fontSurface = NULL;
	}

}
