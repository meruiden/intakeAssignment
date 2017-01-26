#include <project/explosionparticle.h>

ExplosionParticle::ExplosionParticle(Vector2 direction) : Entity()
{
	this->addSpriteAsSpriteSheet("assets/images/smoke_clouds.png", 128, 128, 2, 2);
	this->getSpriteSheet()->setSprite(rand() % 3);
	this->destroyme = false;
	this->direction = direction;
	this->speed = 400;
	this->rotationSpeed = ((rand() % 30) - 30);
	this->alpha = 255;
	if (rand() % 2 == 1)
	{
		this->setScale(Vector2(0.5f, 0.5f));
	}
}


ExplosionParticle::~ExplosionParticle()
{

}

void ExplosionParticle::update(float deltaTime)
{
	this->setPosition(this->getPosition() + direction * deltaTime * speed);
	this->setRotation(this->getRotation() + rotationSpeed * deltaTime);

	this->alpha -= deltaTime * 700.0f;
	if (this->alpha <= 1)
	{
		this->alpha = 0;
	}

	this->color.a = alpha;

}
