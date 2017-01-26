#include <project/zombie.h>

Zombie::Zombie() : Entity()
{
	this->addSpriteAsSpriteSheet("assets/images/zombie.png", 256, 256, 4, 4);
	this->onIdle();
	this->getSpriteSheet()->setLoop(true);
	this->getSpriteSheet()->play();
	this->getPhysicsBody()->setPhysicsActive(true);
	this->getPhysicsBody()->setFixedRotation(true);
	this->canattack = true;
	this->healthText = new TextMesh();
	this->healthText->loadFont("assets/arial.ttf");
	this->addChild(healthText);
	this->health = 100;
	this->healthText->setPosition(Vector2(0, -160));
	this->setName("zombie");
	this->setScale(Vector2(0.8f, 0.8f));
	this->getPhysicsBody()->setBoxCollider(128, 200);
	this->setLayer(2);
	this->attacking = false;
	this->walking = false;
}

Zombie::~Zombie()
{
	removeChild(healthText);
}

void Zombie::update(float deltaTime)
{

	std::stringstream ss;
	ss << health;
	ss << " / 100";
	healthText->setText(ss.str());
	this->getSpriteSheet()->animate(deltaTime, 1.0f / 8.0f);
	if (getScale().x < 0)
	{
		healthText->setScale(Vector2(-1, 1));
	}
	else
	{
		healthText->setScale(Vector2(1, 1));
	}
	Vector2 zombieVel = this->getPhysicsBody()->getVelocity();
	if (zombieVel.x > 2.0f)
	{
		this->getPhysicsBody()->setVelocity(Vector2(2.0f, zombieVel.y));
	}

	if (zombieVel.x < -2.0f)
	{
		this->getPhysicsBody()->setVelocity(Vector2(-2.0f, zombieVel.y));
	}
	if (getSpriteSheet()->getCurSpriteId() == 0)
	{
		canattack = true;
	}
}

void Zombie::applyDamage(float damage)
{
	this->health -= damage;
	if (health < 0)
	{
		health = 0;
	}
}

void Zombie::startAttack()
{
	attacking = true;
	this->getSpriteSheet()->setAnimateFrames(0, 3);
}

void Zombie::stopAttack()
{
	attacking = false;
	walking = false;
}

void Zombie::onIdle()
{
	if (getSpriteSheet()->getAnimationStartFrame() != 4)
	{
		this->getSpriteSheet()->setAnimateFrames(4, 7);
	}
	walking = false;
}

void Zombie::onWalk()
{
	if (!walking)
	{
		walking = true;
		if (getSpriteSheet()->getAnimationStartFrame() != 8)
		{
			this->getSpriteSheet()->setAnimateFrames(8, 13);
		}
	}
}
