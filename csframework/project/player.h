#pragma once

#include <engine/entity.h>

class Player :
	public Entity
{
public:
	Player();
	virtual ~Player();
	virtual void update(float deltaTime);
};

