#pragma once
#include <engine/entity.h>
#include <engine/vector2.h>
#include <engine/sound.h>
class Bullet : public Entity
{
public:
	Bullet(Vector2 dir);
	virtual ~Bullet();
	virtual void update(float deltaTime);
	bool mustDestroy() { return destroyme; }
private:
	float speed;
	float lifeTime;
	float lifeTimeCounter;

	bool destroyme;
	Vector2 direction;

	Sound* gunShot;
};

