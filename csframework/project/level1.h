#pragma once

#include <engine/scene.h>
#include <project/player.h>

class Level1 : public Scene
{
public:
	Level1();
	virtual ~Level1();
	virtual void update(float deltaTime);
	

private:
	Player* player;

};
