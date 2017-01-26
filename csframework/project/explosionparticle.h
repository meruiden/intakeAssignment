#pragma once
#include <engine/entity.h>

class ExplosionParticle : public Entity
{
public:
	ExplosionParticle(Vector2 direction);
	virtual ~ExplosionParticle();
	virtual void update(float deltaTime);

	bool destroyMe() { return this->color.a <= 1; }

private:
	bool destroyme;
	float rotationSpeed;
	Vector2 direction;
	float speed;
	float alpha;
};

