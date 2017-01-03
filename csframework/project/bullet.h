#pragma once
#include <engine/entity.h>
#include <engine/vector2.h>

#include <project/zombie.h>
#include <project/crate.h>
class Bullet : public Entity
{
public:
	Bullet(Vector2 dir);
	virtual ~Bullet();
	virtual void update(float deltaTime); 
	virtual void onCollisionBegin(Entity* other);
	virtual void onCollisionEnd(Entity* other);
	
	bool mustDestroy() { return destroyme; }
private:
	float speed;
	float lifeTime;
	float lifeTimeCounter;

	bool destroyme;
	Vector2 direction;

};

