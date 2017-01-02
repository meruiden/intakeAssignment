#include <project/player.h>

Player::Player(PlayerGroundTrigger* groundTrigger) : Entity()
{
	this->addSprite("assets/character_idle.png");
	this->getPhysicsBody()->setPhysicsActive(true);
	this->groundTrigger = groundTrigger;
	this->setName("player");

	getPhysicsBody()->setDrawColliders(false);
	wasGrounded = false;

	landsound = new Sound("assets/footStep.wav");
	setLayer(3);

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
	if (!this->isGrounded())
	{
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
	walking = true;

}

void Player::onIdle()
{
	if (!isGrounded())
	{
		return;
	}
	walking = false;
}

void Player::setWalk(bool value)
{
	walking = value;
}

Bullet * Player::shoot(float dir)
{
	Vector2 bulletDir;
	dir *= -1;

	if (this->getScale() == Vector2(1, 1))
	{
		dir -= 90;
	}

	if (this->getScale() == Vector2(-1, 1))
	{
		dir += 90;
	}
	bulletDir = Vector2(std::sin(dir * DEG_TO_RAD), std::cos(dir * DEG_TO_RAD));
	Bullet* bullet = new Bullet(bulletDir);
	return bullet;
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


