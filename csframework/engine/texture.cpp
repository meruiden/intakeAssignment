#include <engine/texture.h>

Texture::Texture()
{
	width = 0;
	height = 0;
	textureBuffer = ResourceManager::getInstance()->getEmptyTexture();
	
}

void Texture::setPreloaded(int width, int height, GLuint textureBuffer)
{
	this->width = width;
	this->height = height;
	this->textureBuffer = textureBuffer;
}


Texture::~Texture()
{
	glDeleteTextures(1, &textureBuffer);
}

bool Texture::loadTexture(std::string path)
{
	bool succes = true;
	std::cout << "Reading image: " << path << std::endl;

	SDL_Surface * plaatje = NULL;
	plaatje = IMG_Load(path.c_str());
	if (plaatje == NULL)
	{
		std::cout << "Cant open: " << path << std::endl;
		succes = false;
		return succes;
	}
	int mode = GL_BGRA;
	switch (plaatje->format->BytesPerPixel)
	{
	case 4:
		if (plaatje->format->Rmask == 0x000000ff)
			mode = GL_RGBA;
		else
			mode = GL_BGRA;
		break;
	case 3:
		if (plaatje->format->Rmask == 0x000000ff)
			mode = GL_RGB;
		else
			mode = GL_BGR;
		break;
	default:
		std::cout << "Error, image is not truecolor." << std::endl;
		succes = false;
		return succes;
	}
	GLuint texturebffr;
	glGenTextures(1, &texturebffr);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, texturebffr);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glTexImage2D(GL_TEXTURE_2D, 0, plaatje->format->BytesPerPixel, plaatje->w, plaatje->h, 0, mode, GL_UNSIGNED_BYTE, plaatje->pixels);

	// Poor filtering, or ...
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// ... nice trilinear filtering.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glGenerateMipmap(GL_TEXTURE_2D);

	this->width = plaatje->w;
	this->height = plaatje->h;
	this->textureBuffer = texturebffr;

	SDL_FreeSurface(plaatje);
	plaatje = NULL;

	return succes;
}