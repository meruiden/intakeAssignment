#include <project/zombie.h>

Zombie::Zombie() : Entity()
{
	this->addSpriteAsSpriteSheet("assets/zombie.png", 256, 256, 4, 4);
	this->getSpriteSheet()->setAnimateFrames(8, 13);
	this->getSpriteSheet()->setLoop(true);
	this->getSpriteSheet()->play();
	this->getPhysicsBody()->setPhysicsActive(true);
	this->getPhysicsBody()->setFixedRotation(true);
	this->healthText = new TextMesh();
	this->healthText->loadFont("assets/arial.ttf");
	this->addChild(healthText);
	this->health = 100;
	this->healthText->setPosition(Vector2(0, -160));
	this->setName("zombie");
	this->setScale(Vector2(0.8f, 0.8f));
	this->getPhysicsBody()->setBoxCollider(128, 200);

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
}

void Zombie::applyDamage(float damage)
{
	this->health -= damage;
	if (health < 0)
	{
		health = 0;
	}
}
