#pragma once

#include <engine/entity.h>
#include <engine/textmesh.h>

class Zombie : public Entity
{
public:
	Zombie();
	virtual ~Zombie();
	virtual void update(float deltaTime);

	void applyDamage(float damage);

	float getHealth() { return health; }
private:
	float health;

	TextMesh* healthText;

};

