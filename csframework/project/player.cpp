#include <project/player.h>

Player::Player(PlayerGroundTrigger* groundTrigger) : Entity()
{
	this->addSpriteAsSpriteSheet("assets/character.png", 256, 256, 4, 8);

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

	landsound = new Sound("assets/footStep.wav");
	setLayer(2);
}

Player::~Player()
{
	delete landsound;
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
		if (this->getSpriteSheet()->getAnimationStartFrame() != 12)
		{
			this->getSpriteSheet()->setAnimateFrames(12, 17);
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
	this->getSpriteSheet()->setAnimateFrames(4, 11);
	this->getSpriteSheet()->play();
	this->getSpriteSheet()->setLoop(true);
	walking = true;
	animationFps = walkAnimationFps;
}

void Player::onIdle()
{
	if (!isGrounded())
	{
		return;
	}
	this->getSpriteSheet()->setAnimateFrames(0, 3);
	this->getSpriteSheet()->play();
	this->getSpriteSheet()->setLoop(true);
	walking = false;
	animationFps = idleAnimationFps;

}

void Player::setWalk(bool value)
{
	walking = value;
}

void Player::onIsGrounded()
{
	if (walking)
	{
		onWalk();
	}
	else 
	{
		onIdle();
	}
	landsound->play();
}


