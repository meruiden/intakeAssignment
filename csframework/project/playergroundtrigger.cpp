#include <project/playergroundtrigger.h>

PlayerGroundTrigger::PlayerGroundTrigger()
{
	this->color = Color(0, 0, 0, 0);
	this->addSprite("assets/square.png");
	groundedCollisions = 0;

}


PlayerGroundTrigger::~PlayerGroundTrigger()
{

}

void PlayerGroundTrigger::onCollisionBegin(Entity* other)
{

	if (other->getName() == "ground")
	{
		groundedCollisions++;
	}
}

void PlayerGroundTrigger::onCollisionEnd(Entity* other)
{
	if (other->getName() == "ground")
	{
		groundedCollisions--;
	}
}