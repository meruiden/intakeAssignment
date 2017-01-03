#pragma once

#include <engine/entity.h>

#include <project/cratepart.h>
class Crate : public Entity
{
public:
	Crate();
	virtual ~Crate();

	void applyDamage(float damage);
	
	bool destroyMe() { return health == 0; }
	std::vector<CratePart*> getParts();
private:
	float health;
};
