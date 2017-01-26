#include <project/background.h>

Background::Background(Camera* sceneCamera)
{
	addSprite("assets/images/background.png");
	this->sceneCamera = sceneCamera;
}


Background::~Background()
{
}

void Background::update(float deltaTime)
{
	if (this->getSprite()->getTextureSize().magnitude() > 0)
	{
		this->setScale(Camera::getWindowSize() / this->getSprite()->getTextureSize());
	}
	this->setPosition(sceneCamera->getPosition());
}
