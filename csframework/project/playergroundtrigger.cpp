#include <project/playergroundtrigger.h>

PlayerGroundTrigger::PlayerGroundTrigger()
{
	this->groundedCollisions = 0;
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