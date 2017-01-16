#include <project/background.h>

Background::Background(Camera* sceneCamera)
{
	addSprite("assets/background.png");
	this->sceneCamera = sceneCamera;
}


Background::~Background()
{
}

void Background::update(float deltaTime)
{
	this->setScale(Camera::getWindowSize() / this->getSprite()->getTextureSize());
	this->setPosition(sceneCamera->getPosition());
}
