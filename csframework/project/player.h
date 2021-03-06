#pragma once

#include <engine/entity.h>
#include <engine/sound.h>

#include <project/playergroundtrigger.h>
#include <project/bullet.h>

class Player : public Entity
{
public:
	Player(PlayerGroundTrigger* groundTrigger);
	virtual ~Player();
	virtual void update(float deltaTime);
	virtual void onCollisionBegin(Entity* other);
	virtual void onCollisionEnd(Entity* other);

	bool isGrounded() { return groundTrigger->getGroundedCollisions() > 0; }
	bool hasAmmo() { return ammoInMag > 0; }

	void onWalk();
	void onIdle();
	void applyDamage(int damage);
	void setWalk(bool value);
	void reload() { ammoInMag = maxAmmoInMag; }
	void resetStats();
	
	int getHealth() { return health; }
	int getMaxAmmo() { return maxAmmoInMag; }
	int getAmmoLeft() { return ammoInMag; }
	Bullet* shoot(float dir);
private:
	PlayerGroundTrigger* groundTrigger;
	Sound* landsound;

	void onIsGrounded();

	bool wasGrounded;
	bool walking;

	int health;

	int ammoInMag;
	int maxAmmoInMag;
};

