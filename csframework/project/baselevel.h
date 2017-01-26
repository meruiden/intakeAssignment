#pragma once

#include <engine/scene.h>
#include <engine/textmesh.h>
#include <engine/scenemanager.h>
#include <engine/resourcemanager.h>
#include <engine/sound.h>

#include <project/player.h>
#include <project/playergroundtrigger.h>
#include <project/mapeditor.h>
#include <project/bullet.h>
#include <project/zombie.h>
#include <project/audioparticle.h>
#include <project/crate.h>
#include <project/cratepart.h>
#include <project/questlog.h>
#include <project/background.h>
#include <project/questnpc.h>
#include <project/bloodsplatter.h>
#include <project/explosivebarrel.h>
#include <project/explosionparticle.h>
#include <project/ammoclip.h>

class BaseLevel : public Scene
{
public:
	BaseLevel();
	virtual ~BaseLevel();
	virtual void update(float deltaTime);
	virtual void fixedUpdate();
	virtual void onLoad();

protected:
	void loadMap(std::string filePath);

private:
	void preloadImages();
	void handleInput();
	void checkBullets();
	void checkAudioParticles();
	void handleZombies();
	void checkCrates();
	void checkNpcs();
	void checkBloodSplatters();
	void checkExplosiveBarrels();
	void checkExplosionParticles();
	void checkDroppedAmmoClips();
	void handleReloading(float deltaTime);

	AudioParticle* createAudioParticle(std::string filePath);
	float shootDelay;
	float shootDelayCounter;
	float cameraOffset;
	float playerReloadStartAngle;
	float blackFadeAlpha;

	PlayerGroundTrigger* playerGroundTrigger;

	bool playerLeft;
	bool playerRight;
	bool playerJump;
	bool playerCanWalk;
	bool playerReloading;
	bool armReturning;
	bool fadingIn;
	bool fadingOut;

	std::vector<Entity*> loadedEntities;
	std::vector<Zombie*> zombies;
	std::vector<Bullet*> bullets;
	std::vector<AudioParticle*> audioParticles;
	std::vector<CratePart*> crateParts;
	std::vector<Crate*> crates;
	std::vector<QuestNpc*> npcs;
	std::vector<BloodSplatter*> bloodSplatters;
	std::vector<ExplosiveBarrel*> explosiveBarrels;
	std::vector<ExplosionParticle*> explosionParticles;
	std::vector<AmmoClip*> droppedAmmoClips;

	Entity* leftArmPivot;
	Entity* rightArmPivot;
	Entity* leftArm;
	Entity* rightArm;
	Entity* weapon;
	Entity* bulletLaunchPos;

	Background* background;

	Sound* nightTimeAmbientSound;
	Sound* ambientSound;

	HudText* playerHealthText;
	HudText* ammoText;
	HudText* blackFade;
protected:
	QuestLog* questLog;
	Player* player;

	void fadeIn();
	void fadeOut();
	void clearMap();
	void playMusic();
	void stopMusic();

	bool doneFadingIn() { return blackFadeAlpha == 255; }
	bool doneFadingOut() { return blackFadeAlpha == 0; }
	bool isFading() { return fadingIn || fadingOut; }
};
