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
	void startAttack();
	void stopAttack();
	void onIdle();
	void onWalk();
	void disableAttack() { canattack = false; }

	bool canAttack() { return canattack; }
	bool isAttacking() { return attacking; }
	int getHealth() { return health; }
private:
	int health;
	bool attacking;
	bool walking;
	bool canattack;
	TextMesh* healthText;

};

