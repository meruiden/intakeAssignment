#include <project/player.h>

Player::Player(PlayerGroundTrigger* groundTrigger) : Entity()
{
	this->addSpriteAsSpriteSheet("assets/character_idle.png", 256, 256, 2, 2);
	this->getSpriteSheet()->setAnimateFrames(0, 3);
	this->getSpriteSheet()->play();
	this->getSpriteSheet()->setLoop(true);
	this->getPhysicsBody()->setPhysicsActive(true);
	this->groundTrigger = groundTrigger;
	this->setName("player");
	walkAnimationFps = 10.0f;
	idleAnimationFps = 5.0f;
	jumpAnimationFps = 10.0f;

	animationFps = idleAnimationFps;
	wasGrounded = false;
}

Player::~Player()
{

}

void Player::update(float deltaTime)
{
	if (!wasGrounded && isGrounded())
	{
		onIsGrounded();
		wasGrounded = true;

	}
	this->getSpriteSheet()->animate(deltaTime, 1.0f / animationFps);
	if (!this->isGrounded())
	{
		if (this->getSprite()->getFileName() != "assets/character_jump.png")
		{
			this->addSpriteAsSpriteSheet("assets/character_jump.png", 256, 256, 2, 3);
			this->getSpriteSheet()->setAnimateFrames(0, 5);
			this->getSpriteSheet()->setLoop(false);
			this->getSpriteSheet()->play();
			animationFps = jumpAnimationFps;
		}
		wasGrounded = false;
	}
}

void Player::onCollisionBegin(Entity* other)
{

}

void Player::onCollisionEnd(Entity* other)
{

}

void Player::onWalk()
{
	if (!isGrounded())
	{
		return;
	}
	this->addSpriteAsSpriteSheet("assets/character_walk.png", 256, 256, 4, 2);
	this->getSpriteSheet()->setAnimateFrames(0, 7);
	this->getSpriteSheet()->play();
	this->getSpriteSheet()->setLoop(true);
	animationFps = walkAnimationFps;
}

void Player::onIdle()
{
	if (!isGrounded())
	{
		return;
	}
	this->addSpriteAsSpriteSheet("assets/character_idle.png", 256, 256, 2, 2);
	this->getSpriteSheet()->setAnimateFrames(0, 3);
	this->getSpriteSheet()->play();
	this->getSpriteSheet()->setLoop(true);
	animationFps = idleAnimationFps;

	setLayer(2);
}

void Player::onIsGrounded()
{
	onIdle();
}


