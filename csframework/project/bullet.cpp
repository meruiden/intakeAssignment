#include <project/bullet.h>

Bullet::Bullet(Vector2 dir)
{
	this->direction = dir;
	this->addSprite("assets/bullet.png");
	speed = 1200.0f;
	this->setScale(Vector2(0.5f, 0.5f));
	lifeTime = 5.0f;
	lifeTimeCounter = 0;
	destroyme = false;
	gunShot = new Sound("assets/gunShot.wav");
	gunShot->play();
}


Bullet::~Bullet()
{
	delete gunShot;
}

void Bullet::update(float deltaTime)
{
	this->setRotation(direction.getAngle());
	this->setPosition(this->getPosition() + direction * speed * deltaTime);

	lifeTimeCounter += deltaTime;
	if (lifeTimeCounter >= lifeTime)
	{
		destroyme = true;
	}
}
