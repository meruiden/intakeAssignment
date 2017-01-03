#include <project/bullet.h>

Bullet::Bullet(Vector2 dir) : Entity()
{
	this->direction = dir;
	this->addSprite("assets/bullet.png");
	speed = 1200.0f;
	this->setScale(Vector2(0.5f, 0.5f));
	lifeTime = 5.0f;
	lifeTimeCounter = 0;
	destroyme = false;

	this->getPhysicsBody()->setPhysicsActive(true);
	this->getPhysicsBody()->setPhysicsMode(PhysicsBody::DYNAMIC);
	this->getPhysicsBody()->setTrigger(true);

	setName("bullet");
}


Bullet::~Bullet()
{
}

void Bullet::update(float deltaTime)
{
	this->getPhysicsBody()->setVelocity(Vector2());
	
	this->setRotation(direction.getAngle());
	this->setPosition(this->getPosition() + direction * speed * deltaTime);

	lifeTimeCounter += deltaTime;
	if (lifeTimeCounter >= lifeTime)
	{
		destroyme = true;
	}
}

void Bullet::onCollisionBegin(Entity * other)
{
	if (other->getName() != "player")
	{
		destroyme = true;
	}
	if (other->getName() == "zombie")
	{
		((Zombie*)other)->applyDamage(10);
	}

	if (other->getName() == "crate")
	{
		((Crate*)other)->applyDamage(10);
	}
}

void Bullet::onCollisionEnd(Entity * other)
{
}
