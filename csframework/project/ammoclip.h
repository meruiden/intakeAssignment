#pragma once
#include <engine/entity.h>
class AmmoClip : public Entity
{
public:
	AmmoClip();
	virtual ~AmmoClip();
	virtual void update(float deltaTime);

	bool destroyMe() { return lifeTimeCounter >= lifeTime; }
private:
	float lifeTime;
	float lifeTimeCounter;
};

