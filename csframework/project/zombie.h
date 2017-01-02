#pragma once

#include <engine/entity.h>

class Zombie : public Entity
{
public:
	Zombie();
	virtual ~Zombie();
	virtual void update(float deltaTime);

private:

};

