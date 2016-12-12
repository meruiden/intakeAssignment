#include <project/player.h>

Player::Player() : Entity()
{
	this->addSpriteAsSpriteSheet("assets/character_run.png", 256, 256, 4, 2);
	this->getSpriteSheet()->setAnimateFrames(0, 7);
	this->getSpriteSheet()->play();
	this->getSpriteSheet()->setLoop(true);
}

void Player::update(float deltaTime)
{
	this->getSpriteSheet()->animate(deltaTime, 1.0f / 15.0f);
}

void Player::onCollisionBegin(Entity* other)
{
	
}

void Player::onCollisionExit(Entity* other)
{

}


Player::~Player()
{

}
