#pragma once

#include <engine/entity.h>
#include <project/playergroundtrigger.h>

class Player : public Entity
{
public:
	Player(PlayerGroundTrigger* groundTrigger);
	virtual ~Player();
	virtual void update(float deltaTime);
	virtual void onCollisionBegin(Entity* other);
	virtual void onCollisionEnd(Entity* other);


	bool isGrounded() { return groundTrigger->getGroundedCollisions() > 0; }

	void onWalk();
	void onIdle();
private:
	PlayerGroundTrigger* groundTrigger;

	float walkAnimationFps;
	float idleAnimationFps;
	float animationFps;
};

