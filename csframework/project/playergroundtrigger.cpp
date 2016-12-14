#include <project/playergroundtrigger.h>

PlayerGroundTrigger::PlayerGroundTrigger()
{
	this->color = Color(0, 0, 0, 0);
	this->addSprite("assets/square.png");
	groundedCollisions = 0;
	this->setScale(Vector2(1.8f, 1.0));
	getPhysicsBody()->setDrawColliders(true);
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