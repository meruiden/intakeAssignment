#pragma once

#include <engine/scene.h>
#include <engine/textmesh.h>
#include <engine/scenemanager.h>

#include <project/player.h>
#include <project/playergroundtrigger.h>
#include <project/ground.h>
#include <project/mapeditor.h>
#include <project/bullet.h>
#include <project/zombie.h>

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
	void checkBullets();

	int mapWidth;
	int mapHeight;

	float shootDelay;
	float shootDelayCounter;

	Player* player;
	PlayerGroundTrigger* playerGroundTrigger;

	std::vector<Ground*> groundTiles;
	Entity* groundCollider;

	bool playerLeft;
	bool playerRight;
	bool playerJump;

	std::vector<Entity*> loadedEntities;
	std::vector<Zombie*> zombies;
	std::vector<Bullet*> bullets;
	Entity* leftArmPivot;
	Entity* rightArmPivot;
	Entity* leftArm;
	Entity* rightArm;
	Entity* weapon;
	Entity* bulletLaunchPos;


};
