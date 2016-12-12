#pragma once

#include <engine/entity.h>

class Player : public Entity
{
public:
	Player();
	virtual ~Player();
	virtual void update(float deltaTime);
	virtual void onCollisionBegin(Entity* other);
	virtual void onCollisionExit(Entity* other);
};

