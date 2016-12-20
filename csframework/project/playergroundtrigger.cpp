#include <project/playergroundtrigger.h>

PlayerGroundTrigger::PlayerGroundTrigger()
{
	this->color = Color(0, 0, 0, 0);
	this->addSprite("assets/square.png");
	this->groundedCollisions = 0;
	this->setScale(Vector2(1.9f, 1.0));
	this->getPhysicsBody()->setPhysicsActive(true);
	this->getPhysicsBody()->setDrawColliders(true);
}


PlayerGroundTrigger::~PlayerGroundTrigger()
{

}

void PlayerGroundTrigger::onCollisionBegin(Entity* other)
{

	if (other->getName() != "player")
	{
		groundedCollisions++;
	}
}

void PlayerGroundTrigger::onCollisionEnd(Entity* other)
{
	if (other->getName() != "player")
	{
		groundedCollisions--;
	}
}