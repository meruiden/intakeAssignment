#include <engine/resourcemanager.h>
#include <engine/texture.h>

ResourceManager* ResourceManager::instance = NULL;

ResourceManager::ResourceManager()
{
	createEmptyTexture();
}

ResourceManager* ResourceManager::getInstance() 
{
	if (instance == NULL) 
	{
		instance = new ResourceManager();
	}

	return instance;
}

void ResourceManager::createEmptyTexture()
{
	GLuint texturebffr;
	glGenTextures(1, &texturebffr);

	glBindTexture(GL_TEXTURE_2D, texturebffr);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	std::vector<unsigned char>pixels;

	int w = 32;
	int h = 32;
	int bithDepth = 3; //rgb

	unsigned int size = w * h * bithDepth;
	for (int i = 0; i < size; i++)
	{
		pixels.push_back(255);
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 10, 10, 0, GL_RGB, GL_UNSIGNED_BYTE, &pixels[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenerateMipmap(GL_TEXTURE_2D);

	emptyTexture = texturebffr;
}

GLuint ResourceManager::getEmptyTexture()
{
	return emptyTexture;
}

Mix_Chunk* ResourceManager::getLoadedWav(std::string path, bool &succes) {
	succes = true;

	std::map<std::string, Mix_Chunk*>::iterator it;
	it = loadedWavs.find(path);

	if (it != loadedWavs.end()) {
		return it->second;
	}

	std::cout << "Loading wav: " << path << std::endl;
	Mix_Chunk* wav;
	wav = Mix_LoadWAV(path.c_str());

	if (wav == NULL) {
		succes = false;
		std::cout << "Failed to load sound!!!" << std::endl;
		return NULL;
	}

	loadedWavs.emplace(path, wav);

	return wav;

}

Texture* ResourceManager::getTexture(std::string path, bool &succes)
{
	succes = true;

	Texture* texture = NULL;
	texture = loadedTextures[path];

	if (texture != NULL)
	{
		return texture;
	}

	bool imageloadSucces = false;
	texture = new Texture();
	imageloadSucces = texture->loadTexture(path);

	if (imageloadSucces) 
	{
		loadedTextures[path] = texture;
		return texture;
	}

	succes = false;
	return NULL;
}

Mesh* ResourceManager::getSpriteMesh(int width, int height, float uvWidth, float uvHeight)
{
	char buf[64];
	sprintf(buf, "%d,%d,%f,%f", width, height, uvWidth, uvHeight);
	std::string meshname(buf);

	Mesh* mesh = NULL;
	
	mesh = loadedSpriteMeshes[meshname];
	if (mesh != NULL)
	{
		return mesh;
	}

	mesh = new Mesh();
	mesh->setAsSquare(width, height, uvWidth, uvHeight);
	loadedSpriteMeshes[meshname] = mesh;

	return mesh;
}

TTF_Font* ResourceManager::getLoadedFont(std::string path, bool &succes)
{
	
	succes = true;

	std::map<std::string, TTF_Font*>::iterator it;
	it = loadedFonts.find(path);

	if (it != loadedFonts.end()) {
		return it->second;
	}

	bool fontloadSucces = false;
	TTF_Font* newFont = ResourceLoader::getFont(path, fontloadSucces);

	if (fontloadSucces)
	{
		loadedFonts.emplace(path, newFont);
		return newFont;
	}

	succes = false;
	return NULL;
}

ResourceManager::~ResourceManager()
{
	glDeleteTextures(1, &emptyTexture);

	std::map<std::string, Texture*>::iterator iIt;
	for (iIt = loadedTextures.begin(); iIt != loadedTextures.end(); iIt++)
	{
		delete iIt->second;
	}

	loadedTextures.clear();

	std::map<std::string, Mesh*>::iterator mIt;
	for (mIt = loadedSpriteMeshes.begin(); mIt != loadedSpriteMeshes.end(); mIt++)
	{
		delete mIt->second;
	}

	loadedSpriteMeshes.clear();

	std::map<std::string, Mix_Chunk*>::iterator wIt;
	for (wIt = loadedWavs.begin(); wIt != loadedWavs.end(); wIt++)
	{
		Mix_FreeChunk(wIt->second);
	}

	loadedWavs.clear();

	std::map<std::string, TTF_Font*>::iterator fIt;
	for (fIt = loadedFonts.begin(); fIt != loadedFonts.end(); fIt++)
	{
		TTF_CloseFont(fIt->second);
	}

	loadedFonts.clear();
}
