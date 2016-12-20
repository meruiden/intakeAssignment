#pragma once

#include <engine/scene.h>
#include <engine/textmesh.h>
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
	void createMap();

	int mapWidth;
	int mapHeight;

	Player* player;
	PlayerGroundTrigger* playerGroundTrigger;
	TextMesh* info;

	std::vector<Ground*> groundTiles;
	Entity* groundCollider;

	bool playerLeft;
	bool playerRight;
	bool playerJump;

	std::vector<Vector2> clickColVerts;
	Entity* clickCollider;
	bool mustSecondClick;

	float updateColCounter;

	bool leftClicked;
	bool rightClicked;
};
