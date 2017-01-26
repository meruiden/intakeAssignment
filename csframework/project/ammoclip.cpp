#include <project/ammoclip.h>

AmmoClip::AmmoClip()
{
	lifeTime = 1.0f;
	lifeTimeCounter = 0.0f;
	addSprite("assets/images/ammo_clip.png");
	this->getPhysicsBody()->setPhysicsActive(true);
	this->getPhysicsBody()->setBoxCollider(9, 16);
	this->getPhysicsBody()->setTrigger(true);
	
}


AmmoClip::~AmmoClip()
{

}

void AmmoClip::update(float deltaTime)
{
	lifeTimeCounter += deltaTime;
}
