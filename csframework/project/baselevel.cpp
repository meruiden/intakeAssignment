#include <project/baselevel.h>
 
BaseLevel::BaseLevel() : Scene()
{

	background = new Background(getCamera());
	addEntity(background);
	background->setName("bg");

	playerGroundTrigger = new PlayerGroundTrigger();
	player = new Player(playerGroundTrigger);
	addEntity(player);

	player->setPosition(Vector2(0, 100));

	addEntity(playerGroundTrigger);
	playerGroundTrigger->getPhysicsBody()->setBoxCollider(90, 20);
	playerGroundTrigger->getPhysicsBody()->setPhysicsMode(PhysicsBody::DYNAMIC);
	playerGroundTrigger->getPhysicsBody()->setTrigger(true);

	this->playerJump = false;
	this->playerRight = false;
	this->playerLeft = false;
	this->playerCanWalk = true;

	player->getPhysicsBody()->setFixedRotation(true);
	
	player->getPhysicsBody()->setBoxCollider(100, 256);
	leftArmPivot = new Entity();
	rightArmPivot = new Entity();
	addEntity(leftArmPivot);
	addEntity(rightArmPivot);

	weapon = new Entity();
	weapon->addSprite("assets/images/mp5.png");
	leftArmPivot->addChild(weapon);
	leftArm = new Entity();
	leftArmPivot->addChild(leftArm);
	leftArm->addSprite("assets/images/character_arm_left.png");
	rightArm = new Entity();
	rightArm->addSprite("assets/images/character_arm_right.png");
	rightArmPivot->addChild(rightArm);

	leftArmPivot->setLayer(5);
	rightArmPivot->setLayer(3);

	bulletLaunchPos = new Entity();
	weapon->addChild(bulletLaunchPos);
	bulletLaunchPos->setPosition(Vector2(-150, -25));

	shootDelay = 0.1f;
	shootDelayCounter = 0.0f;
	cameraOffset = -280;

	player->setScale(Vector2(-1, 1));
	leftArm->setScale(Vector2(-1, 1));
	rightArm->setScale(Vector2(-1, 1));
	preloadImages();

	questLog = new QuestLog();
	addHudElement(questLog);
	questLog->setAnchorPoint(HudElement::ANCHOR_TOP_RIGHT);
	nightTimeAmbientSound = new Sound("assets/audio/nighttime_ambient.wav");
	ambientSound = new Sound("assets/audio/ambient.wav");

	playerHealthText = new HudText();
	playerHealthText->loadFont("assets/arial.ttf");
	playerHealthText->setAnchorPoint(HudElement::ANCHOR_TOP_LEFT);
	addHudElement(playerHealthText);

	ammoText = new HudText();
	ammoText->loadFont("assets/arial.ttf");
	ammoText->setAnchorPoint(HudElement::ANCHOR_TOP_RIGHT);
	addHudElement(ammoText);

	playerReloading = false;
	playerReloadStartAngle = 0.0f;
	armReturning = false;

	blackFade = new HudText();
	blackFade->addSprite("assets/images/whiteSquare.png");
	blackFade->color = BLACK;
	addHudElement(blackFade);
	blackFade->setScale(Vector2(50, 50));

	fadingIn = false;
	fadingOut = false;
	blackFadeAlpha = 255;
	reloadIndicatorAlpha = 255;
	reloadIndicatorAlphaTimer = 0;

	reloadIndicator = new HudText();
	reloadIndicator->loadFont("assets/arial.ttf");
	reloadIndicator->setText("Press 'R' to reload");
	reloadIndicator->setAnchorPoint(HudElement::ANCHOR_BOTTOM_CENTER);
	reloadIndicator->setPosition(Vector2(0, -80));
	addHudElement(reloadIndicator);
}

BaseLevel::~BaseLevel()
{
	removeHudElement(reloadIndicator);
	delete reloadIndicator;

	removeHudElement(ammoText);
	delete ammoText;

	removeHudElement(playerHealthText);
	delete playerHealthText;

	delete ambientSound;
	delete nightTimeAmbientSound;

	removeEntity(background);
	delete background;

	weapon->removeChild(bulletLaunchPos);
	delete bulletLaunchPos;

	leftArmPivot->removeChild(weapon);
	delete weapon;
	leftArmPivot->removeChild(leftArm);
	delete leftArm;
	rightArmPivot->removeChild(rightArm);
	delete rightArm;

	removeEntity(leftArmPivot);
	delete leftArmPivot;
	removeEntity(rightArmPivot);
	delete rightArmPivot;

	clearMap();

	removeHudElement(questLog);
	delete questLog;

	removeEntity(playerGroundTrigger);
	delete playerGroundTrigger;

	removeEntity(player);
	delete player;
}

void BaseLevel::update(float deltaTime)
{
	if (player->getAmmoLeft() == 0)
	{
		reloadIndicatorAlphaTimer += deltaTime*2.5f;
		reloadIndicatorAlpha = (std::sin(reloadIndicatorAlphaTimer) + 1.0f) / 2.0f * 255.0f;
		reloadIndicator->color.a = reloadIndicatorAlpha;
	}
	else
	{
		reloadIndicator->color.a = 0;
		reloadIndicatorAlphaTimer = 0;
	}
	if (player->getAmmoLeft() <= 5)
	{
		ammoText->color = RED;
	}
	else
	{
		ammoText->color = WHITE;
	}

	if (player->getHealth() <= 20)
	{
		playerHealthText->color = RED;
	}
	else
	{
		playerHealthText->color = WHITE;
	}

	blackFade->color.a = blackFadeAlpha;
	if (blackFade->getSprite()->getTextureSize().magnitude() > 0)
	{
		blackFade->setScale(Camera::getWindowSize() / blackFade->getSprite()->getTextureSize());
	}

	if (fadingOut)
	{
		blackFadeAlpha -= deltaTime * 100;
		if (blackFadeAlpha <= 0)
		{
			fadingOut = false;
			blackFadeAlpha = 0;
		}
	}

	if (fadingIn)
	{
		blackFadeAlpha += deltaTime * 100;
		if (blackFadeAlpha >= 255)
		{
			fadingIn = false;
			blackFadeAlpha = 255;
		}
	}

	std::stringstream hs;
	hs << "Health: ";
	hs << player->getHealth();
	hs << " / 100";
	playerHealthText->setText(hs.str());

	std::stringstream as;
	as << "Ammo: ";
	as << player->getAmmoLeft();
	as << " / ";
	as << player->getMaxAmmo();

	ammoText->setText(as.str());

	ammoText->setPosition(Vector2(-(ammoText->getWidth() / 2.0f) - 10, 20));
	playerHealthText->setPosition(Vector2(playerHealthText->getWidth() / 2.0f + 10, 20));
	questLog->setPosition(Vector2(-(questLog->getWidth() / 2.0f), questLog->getHeight() / 2.0f) + Vector2(-10, 10 + ammoText->getHeight()));

	shootDelayCounter += deltaTime;
	handleInput();
	playerGroundTrigger->setPosition(player->getPosition() + Vector2(0, player->getHeight()/2.0f ));
	
	if ((playerRight || playerLeft) && player->isGrounded())
	{
		player->setWalk(true);
	}
	else
	{
		player->setWalk(false);
	}

	if (input()->getKeyUp(SDLK_ESCAPE))
	{
		stopMusic();
		SceneManager::loadScene("menu");
	}

	float lookAtRotation = 0.0f;

	lookAtRotation = Vector2(getCamera()->screenToWorldSpace(input()->getMousePosition()), leftArmPivot->getGlobalPosition()).getAngle();
	
	Vector2 leftArmOffSet;
	Vector2 rightArmOffset;
	
	if (player->getScale() == Vector2(1, 1))
	{
		leftArmOffSet = Vector2(18, -15);
		rightArmOffset = Vector2(18, -15);

		leftArm->setPosition(Vector2(-30, 7));
		rightArm->setPosition(Vector2(-30, 7));

		weapon->setScale(Vector2(0.4f, 0.4f));
		weapon->setPosition(Vector2(-60, 10));

		lookAtRotation += 180;
		if (lookAtRotation > 70 && lookAtRotation < 180)
		{
			lookAtRotation = 70;
		}

		if (lookAtRotation < 300 && lookAtRotation > 180)
		{
			lookAtRotation = 300;
		}
	}

	if (player->getScale() == Vector2(-1, 1))
	{
		leftArmOffSet = Vector2(-18, -15);
		rightArmOffset = Vector2(-18, -15);

		leftArm->setPosition(Vector2(30, 7));
		rightArm->setPosition(Vector2(30, 7));

		weapon->setScale(Vector2(-0.4f, 0.4f));
		weapon->setPosition(Vector2(60, 10));
		if (lookAtRotation > 70)
		{
			lookAtRotation = 70;
		}

		if (lookAtRotation < -70)
		{
			lookAtRotation = -70;
		}
	}
	leftArmPivot->setPosition(player->getPosition() + leftArmOffSet);
	rightArmPivot->setPosition(player->getPosition() + rightArmOffset);	

	if (playerReloading)
	{
		handleReloading(deltaTime);
	}
	else
	{
		rightArmPivot->setRotation(lookAtRotation);
		leftArmPivot->setRotation(lookAtRotation);
	}

	
	checkBullets();
	checkAudioParticles();
	checkCrates();
	checkNpcs();
	checkBloodSplatters();
	checkExplosiveBarrels();
	checkExplosionParticles();
	checkDroppedAmmoClips();

	if (player->getHealth() <= 0 )
	{
		if (playerCanWalk)
		{
			playerCanWalk = false;
			playerLeft = false;
			playerJump = false;
			playerRight = false;
			player->setWalk(false);
			player->onIdle();
			player->getPhysicsBody()->setVelocity(Vector2(0, player->getPhysicsBody()->getVelocity().y));
			fadeIn();
		}
		if (doneFadingIn())
		{
			SceneManager::loadScene("menu");
		}
	}
}

void BaseLevel::preloadImages()
{
	bool succes = false;
	ResourceManager::getInstance()->getTexture("assets/images/character_idle.png", succes);
	ResourceManager::getInstance()->getTexture("assets/images/character_arm_left.png", succes);
	ResourceManager::getInstance()->getTexture("assets/images/character_arm_right.png", succes);
	ResourceManager::getInstance()->getTexture("assets/images/zombie.png", succes);
	ResourceManager::getInstance()->getTexture("assets/images/mp5.png", succes);
	ResourceManager::getInstance()->getTexture("assets/images/crate/crate_full.png", succes);
	ResourceManager::getInstance()->getTexture("assets/images/background.png", succes);
	ResourceManager::getInstance()->getTexture("assets/images/hospital.png", succes);
	ResourceManager::getInstance()->getTexture("assets/images/crate/crate_slices.png", succes);
	ResourceManager::getInstance()->getTexture("assets/images/bullet.png", succes);
}

void BaseLevel::handleInput()
{
	if (playerCanWalk)
	{
		if (input()->getKeyDown(SDLK_SPACE) && player->isGrounded())
		{
			playerJump = true;
		}

		if (input()->getKeyDown(SDLK_d))
		{
			playerRight = true;
			player->onWalk();
		}

		if (input()->getKeyDown(SDLK_a))
		{
			playerLeft = true;
			player->onWalk();
		}

		if (input()->getKeyUp(SDLK_d))
		{
			playerRight = false;
			if (!playerLeft)
			{
				player->onIdle();
			}
		}

		if (input()->getKeyUp(SDLK_a))
		{
			playerLeft = false;
			if (!playerRight)
			{
				player->onIdle();
			}
		}
		if (input()->getMouseButton(1) && shootDelayCounter >= shootDelay && player->hasAmmo() && !playerReloading)
		{
			shootDelayCounter = 0.0f;
			Bullet* b = player->shoot(leftArmPivot->getRotation());
			addEntity(b);
			b->setPosition(bulletLaunchPos->getGlobalPosition());
			bullets.push_back(b);
			createAudioParticle("assets/audio/gunShot.wav")->start();
		}

		if (input()->getKeyDown(SDLK_r) && !playerReloading && player->getAmmoLeft() != player->getMaxAmmo())
		{
			playerReloading = true;
			playerReloadStartAngle = rightArmPivot->getRotation();
			AmmoClip* droppedClip = new AmmoClip();
			droppedClip->setPosition(weapon->getGlobalPosition());
			addEntity(droppedClip);
			droppedAmmoClips.push_back(droppedClip);
			droppedClip->setRotation(rand() % 360);
		}
	}
}


void BaseLevel::fixedUpdate()
{
	if (!player->isGrounded())
	{
		player->getPhysicsBody()->addForce(Vector2(0, 300.0f));
	}

	if (playerJump)
	{
		playerJump = false;
		player->getPhysicsBody()->addForce(Vector2(0, -10000) );
	}

	if (playerRight && !playerReloading)
	{
		player->getPhysicsBody()->addForce(Vector2(400.0f, 0));
		if (player->getScale() != Vector2(-1, 1))
		{
			player->setScale(Vector2(-1, 1));
			leftArm->setScale(Vector2(-1, 1));
			rightArm->setScale(Vector2(-1, 1));
		}
	}
	
	if (playerLeft && !playerReloading)
	{
		if (player->getScale() != Vector2(1, 1))
		{
			player->setScale(Vector2(1, 1));
			leftArm->setScale(Vector2(1, 1));
			rightArm->setScale(Vector2(1, 1));
		}
		player->getPhysicsBody()->addForce(Vector2(-400.0f, 0));
	}

	getCamera()->setPosition(player->getPosition() + Vector2(250, -(Camera::getWindowSize().y/2.0f + cameraOffset)));

	Vector2 playerVelocity = player->getPhysicsBody()->getVelocity();
	if (playerVelocity.x > 10.0f)
	{
		player->getPhysicsBody()->setVelocity(Vector2(10.0f, playerVelocity.y));
	}

	if (playerVelocity.x < -10.0f)
	{
		player->getPhysicsBody()->setVelocity(Vector2(-10.0f, playerVelocity.y));
	}

	if (playerVelocity.y > 50.0f)
	{
		player->getPhysicsBody()->setVelocity(Vector2(playerVelocity.x, 50.0f));
	}

	if (playerVelocity.y < -50.0f)
	{
		player->getPhysicsBody()->setVelocity(Vector2(playerVelocity.x, 50.0f));
	}

	if (!playerLeft && !playerRight && player->isGrounded())
	{
		playerVelocity = player->getPhysicsBody()->getVelocity();
		player->getPhysicsBody()->setVelocity(Vector2(playerVelocity.x * 0.97f, playerVelocity.y));
	}

	handleZombies();
}

void BaseLevel::onLoad()
{
	playMusic();
	fadeOut();
}

void BaseLevel::clearMap()
{
	questLog->clear();

	for (int i = 0; i < loadedEntities.size(); i++)
	{
		removeEntity(loadedEntities[i]);
		delete loadedEntities[i];
	}
	loadedEntities.clear();


	for (int i = 0; i < crates.size(); i++)
	{
		removeEntity(crates[i]);
		delete crates[i];
	}
	crates.clear();

	for (int i = 0; i < crateParts.size(); i++)
	{
		removeEntity(crateParts[i]);
		delete crateParts[i];
	}
	crateParts.clear();

	for (int i = 0; i < bullets.size(); i++)
	{
		removeEntity(bullets[i]);
		delete bullets[i];
	}
	bullets.clear();

	for (int i = 0; i < audioParticles.size(); i++)
	{
		delete audioParticles[i];
	}
	audioParticles.clear();

	for (int i = 0; i < zombies.size(); i++)
	{
		removeEntity(zombies[i]);
		delete zombies[i];
	}
	zombies.clear();

	for (int i = 0; i < npcs.size(); i++)
	{
		removeEntity(npcs[i]);
		delete npcs[i];
	}
	npcs.clear();

	for (int i = 0; i < bloodSplatters.size(); i++)
	{
		removeEntity(bloodSplatters[i]);
		delete bloodSplatters[i];
	}
	bloodSplatters.clear();

	for (int i = 0; i < explosionParticles.size(); i++)
	{
		removeEntity(explosionParticles[i]);
		delete explosionParticles[i];
	}
	explosionParticles.clear();


	for (int i = 0; i < explosiveBarrels.size(); i++)
	{
		removeEntity(explosiveBarrels[i]);
		delete explosiveBarrels[i];
	}
	explosiveBarrels.clear();

	for (int i = 0; i < droppedAmmoClips.size(); i++)
	{
		removeEntity(droppedAmmoClips[i]);
		delete droppedAmmoClips[i];
	}
	droppedAmmoClips.clear();

	player->resetStats();
	playerCanWalk = true;
}

void BaseLevel::playMusic()
{
	nightTimeAmbientSound->play(true);
	ambientSound->play(true);
}

void BaseLevel::stopMusic()
{
	nightTimeAmbientSound->stop();
	ambientSound->stop();
}

void BaseLevel::loadMap(std::string filePath)
{
	MapEditor::loadMap(loadedEntities, filePath);

	for (std::vector<Entity*>::iterator it = loadedEntities.begin(); it != loadedEntities.end();)
	{
		Entity* entity = (*it);
		bool removedSomething = false;
		if (entity->getName() == "Player")
		{
			player->setPosition(entity->getPosition());
			delete entity;
			it = loadedEntities.erase(it);
			removedSomething = true;

		}
		else if (entity->getName() == "crate")
		{
			Crate* c = new Crate();
			c->setPosition(entity->getPosition());
			c->setScale(entity->getScale());
			c->setRotation(entity->getRotation());
			c->setName(entity->getName());
			delete entity;
			it = loadedEntities.erase(it);
			removedSomething = true;
			addEntity(c);
			crates.push_back(c);
			c->setLayer(2);
			
		}
		else if (entity->getName() == "zombie")
		{
			Zombie* z = new Zombie();
			z->setPosition(entity->getPosition());
			z->setName(entity->getName());
			delete entity;
			it = loadedEntities.erase(it);
			removedSomething = true;
			addEntity(z);
			zombies.push_back(z);
		}
		else if (entity->getName() == "npc")
		{
			QuestNpc* npc = new QuestNpc(questLog);
			npc->setPosition(entity->getPosition());
			npc->setName(entity->getName());
			delete entity;
			it = loadedEntities.erase(it);
			removedSomething = true;
			addEntity(npc);
			npcs.push_back(npc);
		}
		else if (entity->getName() == "explosive barrel")
		{
			ExplosiveBarrel* barrel = new ExplosiveBarrel();
			barrel->setPosition(entity->getPosition());
			barrel->setName(entity->getName());
			delete entity;
			it = loadedEntities.erase(it);
			removedSomething = true;
			addEntity(barrel);
			explosiveBarrels.push_back(barrel);
			barrel->setLayer(2);
		}
		else
		{
			if (entity->getName() == "water")
			{
				entity->getPhysicsBody()->setPhysicsActive(true);
				entity->getPhysicsBody()->setBoxCollider(25, 68);
			}
			addEntity(entity);
		}

		if (!removedSomething)
		{
			it++;
		}
		
	}
}

void BaseLevel::checkBullets()
{
	std::vector<Bullet* >::iterator it = bullets.begin();
	while (it != bullets.end())
	{
		Bullet* b = (*it);
		if (b->mustDestroy())
		{
			if (b->spawnBlood())
			{
				BloodSplatter* bs = new BloodSplatter(b->getRotation());
				bs->setPosition(b->getPosition());
				addEntity(bs);
				bloodSplatters.push_back(bs);
			}
			removeEntity(b);
			delete b;
			it = bullets.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void BaseLevel::checkAudioParticles()
{
	std::vector<AudioParticle* >::iterator it = audioParticles.begin();
	while (it != audioParticles.end())
	{
		AudioParticle* a = (*it);
		if (a->destroyMe())
		{
			delete a;
			it = audioParticles.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void BaseLevel::handleZombies()
{

	for (int i = 0; i < zombies.size(); i++)
	{
		float distanceToPlayer = Vector2(zombies[i]->getPosition(), player->getPosition()).magnitude();
		if (distanceToPlayer <= 800 && distanceToPlayer >= 220)
		{
			if (zombies[i]->getPosition().x < player->getPosition().x)
			{
				zombies[i]->getPhysicsBody()->addForce(Vector2(100, 0));
				zombies[i]->setScale(Vector2(std::abs(zombies[i]->getScale().x), zombies[i]->getScale().y));
				
			}
			if (zombies[i]->getPosition().x > player->getPosition().x)
			{
				zombies[i]->getPhysicsBody()->addForce(Vector2(-100, 0));
				zombies[i]->setScale(Vector2(-std::abs(zombies[i]->getScale().x), zombies[i]->getScale().y));
			}
			zombies[i]->stopAttack();
			zombies[i]->onWalk();
		}
		else if(distanceToPlayer <= 220)
		{
			if (!zombies[i]->isAttacking())
			{
				zombies[i]->startAttack();
			}
		}
		else
		{
			zombies[i]->stopAttack();
			zombies[i]->onIdle();
		}

		if (zombies[i]->isAttacking() && zombies[i]->getSpriteSheet()->getCurSpriteId() == 3 && zombies[i]->canAttack())
		{
			zombies[i]->disableAttack();
			player->applyDamage(10);
		}
	}
	std::vector<Zombie* >::iterator it = zombies.begin();
	while (it != zombies.end())
	{
		Zombie* z = (*it);
		if (z->getHealth() == 0)
		{
			removeEntity(z);
			delete z;
			it = zombies.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void BaseLevel::checkCrates()
{

	std::vector<Crate* >::iterator it = crates.begin();
	while (it != crates.end())
	{
		Crate* c = (*it);
		if (c->destroyMe())
		{
			std::vector<CratePart*> parts = c->getParts();
			for (int i = 0; i < parts.size(); i++)
			{
				addEntity(parts[i]);
				parts[i]->getPhysicsBody()->setCollider(parts[i]->getColliderVerts(c->getScale()));
				Vector2 explosionDir = Vector2(parts[i]->getPosition(), c->getPosition()) ;
				explosionDir.normalize();
				parts[i]->getPhysicsBody()->addForce(explosionDir  * 300);
				crateParts.push_back(parts[i]);
			}
			removeEntity(c);
			delete c;
			it = crates.erase(it);
		}
		else
		{
			++it;
		}
	}

	std::vector<CratePart* >::iterator ite = crateParts.begin();
	while (ite != crateParts.end())
	{
		CratePart* cp = (*ite);
		if (cp->destroyMe())
		{
			removeEntity(cp);
			delete cp;
			ite = crateParts.erase(ite);
		}
		else
		{
			++ite;
		}
	}
}

void BaseLevel::checkNpcs()
{
	for(QuestNpc* npc : npcs)
	{
		if (npc->canStartChat() && player->getPosition().x > npc->getPosition().x - 100 - npc->getWidth() / 2.0f &&
			player->getPosition().x < npc->getPosition().x + 100 + npc->getWidth() / 2.0f) 
		{
			npc->startChat();
			playerCanWalk = false;
			playerLeft = false;
			playerJump = false;
			playerRight = false;
			player->setWalk(false);
			player->onIdle();
			player->getPhysicsBody()->setVelocity(Vector2(0, player->getPhysicsBody()->getVelocity().y));
		}
		if (npc->getChatComplete() && !npc->disabled())
		{
			npc->disable();
			playerCanWalk = true;
		}
	}
}

void BaseLevel::checkBloodSplatters()
{
	std::vector<BloodSplatter* >::iterator it = bloodSplatters.begin();
	while (it != bloodSplatters.end())
	{
		if ((*it)->color.a <= 1)
		{
			removeEntity(*it);
			delete (*it);
			it = bloodSplatters.erase(it);
		}
		else
		{
			it++;
		}
	}
}

void BaseLevel::checkExplosiveBarrels()
{
	std::vector<ExplosiveBarrel*>::iterator it = explosiveBarrels.begin();
	while (it != explosiveBarrels.end())
	{
		if ((*it)->explodeMe())
		{
			std::vector<ExplosionParticle*> particles = (*it)->explode();
			for (int i = 0; i < particles.size(); i++)
			{
				addEntity(particles[i]);
				explosionParticles.push_back(particles[i]);
			}
			AudioParticle* explosionSound = createAudioParticle("assets/audio/explosion.wav");
			explosionSound->start();

			std::vector<Entity*> water = getEntitiesByName("water");
			for (int i = 0; i < water.size(); i++)
			{
				if (Vector2(water[i]->getPosition(), (*it)->getPosition()).magnitude() <= 300)
				{
					water[i]->getPhysicsBody()->addForce((water[i]->getPosition() - (*it)->getPosition()) * 10);

				}

			}
			if (Vector2(player->getPosition(), (*it)->getPosition()).magnitude() <= 300)
			{
				float distance = Vector2(player->getPosition(), (*it)->getPosition()).magnitude();
				if (distance <= 300)
				{
					player->applyDamage((100.0f - ((100.0f / 300.0f) * (distance)))/2.0f );
					player->getPhysicsBody()->addForce((player->getPosition() - (*it)->getPosition()) * 300);
				}
			}
			 
			for (Zombie* zombie : zombies)
			{
				float distance = Vector2(zombie->getPosition(), (*it)->getPosition()).magnitude();
				if (distance <= 300)
				{
					zombie->applyDamage(100.0f - ((100.0f / 300.0f) * distance));
					zombie->getPhysicsBody()->addForce((zombie->getPosition() - (*it)->getPosition()) * 300);
				}
			}
		}
		if ((*it)->destroyMe())
		{
			removeEntity(*it);
			delete (*it);
			it = explosiveBarrels.erase(it);
		}
		else
		{
			it++;
		}

	}
}

void BaseLevel::checkExplosionParticles()
{
	std::vector<ExplosionParticle*>::iterator it = explosionParticles.begin();
	while (it != explosionParticles.end())
	{
		if ((*it)->destroyMe())
		{
			removeEntity(*it);
			delete (*it);
			it = explosionParticles.erase(it);
		}
		else
		{
			it++;
		}
	}
}

void BaseLevel::checkDroppedAmmoClips()
{
	std::vector<AmmoClip*>::iterator it = droppedAmmoClips.begin();
	while (it != droppedAmmoClips.end())
	{
		if ((*it)->destroyMe())
		{
			removeEntity(*it);
			delete (*it);
			it = droppedAmmoClips.erase(it);
		}
		else
		{
			it++;
		}
	}
}

void BaseLevel::handleReloading(float deltaTime)
{
	bool inverted = false;
	if (player->getScale().x > 0)
	{
		inverted = true;
	}
	if (inverted)
	{
		if (rightArmPivot->getRotation() > playerReloadStartAngle - 60 && !armReturning)
		{
			rightArmPivot->setRotation(rightArmPivot->getRotation() - deltaTime * 100.0f);
		}
		else
		{
			if (!armReturning)
			{
				armReturning = true;
			}
		}

		if (rightArmPivot->getRotation() < playerReloadStartAngle && armReturning)
		{
			rightArmPivot->setRotation(rightArmPivot->getRotation() + deltaTime * 80.0f);
		}
		else
		{
			if (armReturning)
			{
				armReturning = false;
				playerReloading = false;
				player->reload();
			}
		}
	}
	else
	{
		if (rightArmPivot->getRotation() < playerReloadStartAngle + 60 && !armReturning)
		{
			rightArmPivot->setRotation(rightArmPivot->getRotation() + deltaTime * 100.0f);
		}
		else
		{
			if (!armReturning)
			{
				armReturning = true;
			}
		}

		if (rightArmPivot->getRotation() > playerReloadStartAngle && armReturning)
		{
			rightArmPivot->setRotation(rightArmPivot->getRotation() - deltaTime * 80.0f);
		}
		else
		{
			if (armReturning)
			{
				armReturning = false;
				playerReloading = false;
				player->reload();
			}
		}
	}
}

void BaseLevel::fadeIn()
{
	fadingIn = true;
	blackFadeAlpha = 0;
}

void BaseLevel::fadeOut()
{
	fadingOut = true;
	blackFadeAlpha = 255;
}

AudioParticle * BaseLevel::createAudioParticle(std::string filePath)
{
	AudioParticle* particle = new AudioParticle(filePath);
	audioParticles.push_back(particle);
	return particle;
}
