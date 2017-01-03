#pragma once

#include <engine/scene.h>
#include <engine/textmesh.h>
#include <engine/scenemanager.h>
#include <engine/resourcemanager.h>

#include <project/player.h>
#include <project/playergroundtrigger.h>
#include <project/mapeditor.h>
#include <project/bullet.h>
#include <project/zombie.h>
#include <project/audioparticle.h>
#include <project/crate.h>
#include <project/cratepart.h>

class Level1 : public Scene
{
public:
	Level1();
	virtual ~Level1();
	virtual void update(float deltaTime);
	virtual void fixedUpdate();

private:
	void preloadImages();
	void handleInput();
	void createMap();
	void checkBullets();
	void checkAudioParticles();
	void handleZombies();
	void checkCrates();

	AudioParticle* createAudioParticle(std::string filePath);

	int mapWidth;
	int mapHeight;

	float shootDelay;
	float shootDelayCounter;

	Player* player;
	PlayerGroundTrigger* playerGroundTrigger;

	Entity* groundCollider;

	bool playerLeft;
	bool playerRight;
	bool playerJump;

	std::vector<Entity*> loadedEntities;
	std::vector<Zombie*> zombies;
	std::vector<Bullet*> bullets;
	std::vector<AudioParticle*> audioParticles;
	std::vector<CratePart*> crateParts;
	std::vector<Crate*> crates;

	Entity* leftArmPivot;
	Entity* rightArmPivot;
	Entity* leftArm;
	Entity* rightArm;
	Entity* weapon;
	Entity* bulletLaunchPos;

};
