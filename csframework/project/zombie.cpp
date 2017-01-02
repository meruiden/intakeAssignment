#include <project/zombie.h>

Zombie::Zombie()
{
	this->addSpriteAsSpriteSheet("assets/zombie.png", 256, 256, 4, 4);
	this->getSpriteSheet()->setAnimateFrames(8, 13);
	this->getSpriteSheet()->setLoop(true);
	this->getSpriteSheet()->play();
	this->getPhysicsBody()->setPhysicsActive(true);
	this->getPhysicsBody()->setFixedRotation(true);
	this->getPhysicsBody()->setDrawColliders(true);
}


Zombie::~Zombie()
{

}

void Zombie::update(float deltaTime)
{
	this->getSpriteSheet()->animate(deltaTime, 1.0f / 8.0f);
}
