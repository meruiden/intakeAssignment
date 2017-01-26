#include <project/explosivebarrel.h>

ExplosiveBarrel::ExplosiveBarrel() : Entity()
{
	this->addSprite("assets/images/explosive_barrel.png");
	this->getPhysicsBody()->setPhysicsActive(true);
	this->getPhysicsBody()->setBoxCollider(80, 150);
	this->health = 20;
	this->destroyme = false;
	this->exploding = false;
}


ExplosiveBarrel::~ExplosiveBarrel()
{

}

void ExplosiveBarrel::update(float deltaTime)
{
	getSpriteSheet()->animate(deltaTime, 1.0f / 30.0f);
	if (exploding)
	{
		if (getSpriteSheet()->getCurSpriteId() == getSpriteSheet()->getAnimationEndFrame())
		{
			this->color.a = 0;
			this->destroyme = true;
		}

	}
}

void ExplosiveBarrel::applyDamage(int damage)
{
	health -= damage;
	if (health < 0)
	{
		health = 0;
	}
}

std::vector<ExplosionParticle*> ExplosiveBarrel::explode()
{
	exploding = true;

	this->addSpriteAsSpriteSheet("assets/images/explosion.png", 256, 256, 2, 5);
	this->getSpriteSheet()->setAnimateFrames(0, 9);
	this->getSpriteSheet()->play();
	std::vector<ExplosionParticle*> particles;

	for (int i = 0; i < 10; i++)
	{
		float angle = rand() % 360;
		Vector2 direction = Vector2(std::sin(angle * DEG_TO_RAD), std::cos(angle * DEG_TO_RAD));
		ExplosionParticle* particle = new ExplosionParticle(direction);
		particle->setPosition(this->getPosition() + direction * (((rand() % 2)+1) * 50));
		particles.push_back(particle);
	}
	this->getPhysicsBody()->setPhysicsActive(false);
	return particles;
}
