#include <project/player.h>

Player::Player(PlayerGroundTrigger* groundTrigger) : Entity()
{
	this->addSpriteAsSpriteSheet("assets/character_idle.png", 256, 256, 2, 2);
	this->getSpriteSheet()->setAnimateFrames(0, 3);
	this->getSpriteSheet()->play();
	this->getSpriteSheet()->setLoop(true);
	this->getPhysicsBody()->setPhysicsActive(true);
	this->groundTrigger = groundTrigger;

	walkAnimationFps = 10.0f;
	idleAnimationFps = 5.0f;

	animationFps = idleAnimationFps;
}

void Player::update(float deltaTime)
{
	this->getSpriteSheet()->animate(deltaTime, 1.0f / animationFps);


}

void Player::onCollisionBegin(Entity* other)
{

}

void Player::onCollisionEnd(Entity* other)
{

}

void Player::onWalk()
{
	this->addSpriteAsSpriteSheet("assets/character_walk.png", 256, 256, 4, 2);
	this->getSpriteSheet()->setAnimateFrames(0, 7);
	this->getSpriteSheet()->play();
	animationFps = walkAnimationFps;
}

void Player::onIdle()
{
	this->addSpriteAsSpriteSheet("assets/character_idle.png", 256, 256, 2, 2);
	this->getSpriteSheet()->setAnimateFrames(0, 3);
	this->getSpriteSheet()->play();
	animationFps = idleAnimationFps;
}

Player::~Player()
{

}
