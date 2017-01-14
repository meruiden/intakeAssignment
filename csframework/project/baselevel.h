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
#include <project/questlog.h>

class BaseLevel : public Scene
{
public:
	BaseLevel();
	virtual ~BaseLevel();
	virtual void update(float deltaTime);
	virtual void fixedUpdate();

protected:
	void loadMap(std::string filePath);
private:
	void preloadImages();
	void handleInput();
	void checkBullets();
	void checkAudioParticles();
	void handleZombies();
	void checkCrates();

	AudioParticle* createAudioParticle(std::string filePath);
	float shootDelay;
	float shootDelayCounter;

	Player* player;
	PlayerGroundTrigger* playerGroundTrigger;

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

	QuestLog* questLog;
};
