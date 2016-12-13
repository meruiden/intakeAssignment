#pragma once

#include <engine/scene.h>
#include <project/player.h>
#include <project/playergroundtrigger.h>
#include <project/ground.h>

class Level1 : public Scene
{
public:
	Level1();
	virtual ~Level1();
	virtual void update(float deltaTime);
	virtual void fixedUpdate();

private:
	void handleInput();

	Player* player;
	PlayerGroundTrigger* playerGroundTrigger;
	Ground* ground;

	bool playerLeft;
	bool playerRight;
	bool playerJump;
};
