#pragma once

#include <engine/entity.h>

class PlayerGroundTrigger :
	public Entity
{
public:
	PlayerGroundTrigger();
	virtual ~PlayerGroundTrigger();
	virtual void onCollisionBegin(Entity* other);
	virtual void onCollisionEnd(Entity* other);

	int getGroundedCollisions() { return groundedCollisions; }
private:
	int groundedCollisions;
};

