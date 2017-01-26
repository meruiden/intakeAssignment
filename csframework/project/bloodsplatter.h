#pragma once
#include <engine/entity.h>
class BloodSplatter : public Entity
{
public:
	BloodSplatter(float impactAngle);
	virtual ~BloodSplatter();
	virtual void update(float deltaTime);

private:
	Vector2 direction;
	float speed;
	float alpha;
};

