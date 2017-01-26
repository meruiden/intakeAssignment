#pragma once
#include <engine/entity.h>

#include <project/explosionparticle.h>
class ExplosiveBarrel : public Entity
{
public:
	ExplosiveBarrel();
	virtual ~ExplosiveBarrel();
	virtual void update(float deltaTime);
	void applyDamage(int damage);

	std::vector<ExplosionParticle*> explode();
	bool destroyMe(){ return destroyme; }
	bool explodeMe() { return (health == 0 && !exploding); }

private:
	int health;
	bool destroyme;
	bool exploding;
};

