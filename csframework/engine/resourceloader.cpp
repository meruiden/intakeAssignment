#include <engine/resourceloader.h>


TTF_Font* ResourceLoader::getFont(std::string path, bool &succes)
{
	succes = true;
	TTF_Font* f;
	f = TTF_OpenFont(path.c_str(), (int)FONT_SIZE);


	if (!f) {
		std::cout << "Can't open font: " << TTF_GetError();
		succes = false;
		return NULL;
	}

	return f;
}