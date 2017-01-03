#include <project/playergroundtrigger.h>

PlayerGroundTrigger::PlayerGroundTrigger()
{
	this->groundedCollisions = 0;
	this->getPhysicsBody()->setPhysicsActive(true);
	setName("player");
}


PlayerGroundTrigger::~PlayerGroundTrigger()
{

}

void PlayerGroundTrigger::onCollisionBegin(Entity* other)
{

	if (other->getName() != "player" && other->getName() != "bullet")
	{
		groundedCollisions++;
	}
}

void PlayerGroundTrigger::onCollisionEnd(Entity* other)
{
	if (other->getName() != "player" && other->getName() != "bullet")
	{
		groundedCollisions--;
	}
}