#include <project/baselevel.h>
 
BaseLevel::BaseLevel() : Scene()
{
	background = new Background(getCamera());
	addEntity(background);

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

	player->getPhysicsBody()->setFixedRotation(true);
	
	player->getPhysicsBody()->setBoxCollider(100, 256);
	leftArmPivot = new Entity();
	rightArmPivot = new Entity();
	addEntity(leftArmPivot);
	addEntity(rightArmPivot);

	weapon = new Entity();
	weapon->addSprite("assets/mp5.png");
	leftArmPivot->addChild(weapon);
	leftArm = new Entity();
	leftArmPivot->addChild(leftArm);
	leftArm->addSprite("assets/character_arm_left.png");
	rightArm = new Entity();
	rightArm->addSprite("assets/character_arm_right.png");
	rightArmPivot->addChild(rightArm);

	leftArmPivot->setLayer(5);
	rightArmPivot->setLayer(3);

	bulletLaunchPos = new Entity();
	weapon->addChild(bulletLaunchPos);
	bulletLaunchPos->setPosition(Vector2(-150, -25));

	shootDelay = 0.1f;
	shootDelayCounter = 0.0f;

	player->setScale(Vector2(-1, 1));
	leftArm->setScale(Vector2(-1, 1));
	rightArm->setScale(Vector2(-1, 1));
	preloadImages();

	questLog = new QuestLog();
	addHudElement(questLog);
	questLog->setAnchorPoint(HudElement::ANCHOR_TOP_RIGHT);
	questLog->addQuest("Hospital needed!", "Clear the path to\nthe Hostpital so Jason\ncan get the medical suplies.");

}

BaseLevel::~BaseLevel()
{
	removeEntity(background);
	delete background;

	removeHudElement(questLog);
	delete questLog;

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

	removeEntity(playerGroundTrigger);
	delete playerGroundTrigger;

	removeEntity(player);
	delete player;
}

void BaseLevel::update(float deltaTime)
{
	questLog->setPosition(Vector2(-questLog->getWidth() / 2.0f, questLog->getHeight() / 2.0f) + Vector2(-10, 10));

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
	
	
	leftArmPivot->setRotation(lookAtRotation);
	rightArmPivot->setRotation(lookAtRotation);

	checkBullets();
	checkAudioParticles();
	checkCrates();
}

void BaseLevel::preloadImages()
{
	bool succes = false;
	ResourceManager::getInstance()->getTexture("assets/character_idle.png", succes);
	ResourceManager::getInstance()->getTexture("assets/character_arm_left.png", succes);
	ResourceManager::getInstance()->getTexture("assets/character_arm_right.png", succes);
	ResourceManager::getInstance()->getTexture("assets/mp5.png", succes);
	ResourceManager::getInstance()->getTexture("assets/zombie.png", succes);
	ResourceManager::getInstance()->getTexture("assets/crate/crate_full.png", succes);
	ResourceManager::getInstance()->getTexture("assets/background.png", succes);
	for (int i = 1; i <= 9; i++)
	{
		std::stringstream path;
		path << "assets/crate/crate_slice_";
		path << i;
		path << ".png";
		ResourceManager::getInstance()->getTexture(path.str() , succes);
	}
}

void BaseLevel::handleInput()
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
	if (input()->getMouseButton(1) && shootDelayCounter >= shootDelay)
	{
		shootDelayCounter = 0.0f;
		Bullet* b = player->shoot(leftArmPivot->getRotation());
		addEntity(b);
		b->setPosition(bulletLaunchPos->getGlobalPosition());
		bullets.push_back(b);
		createAudioParticle("assets/gunShot.wav")->start();
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

	if (playerRight)
	{
		player->getPhysicsBody()->addForce(Vector2(400.0f, 0));
		if (player->getScale() != Vector2(-1, 1))
		{
			player->setScale(Vector2(-1, 1));
			leftArm->setScale(Vector2(-1, 1));
			rightArm->setScale(Vector2(-1, 1));
		}
	}
	
	if (playerLeft)
	{
		if (player->getScale() != Vector2(1, 1))
		{
			player->setScale(Vector2(1, 1));
			leftArm->setScale(Vector2(1, 1));
			rightArm->setScale(Vector2(1, 1));
		}
		player->getPhysicsBody()->addForce(Vector2(-400.0f, 0));
	}

	getCamera()->setPosition(player->getPosition() + Vector2(250, 0));

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

void BaseLevel::loadMap(std::string filePath)
{
	MapEditor::loadMap(loadedEntities, filePath);
	for (int i = 0; i < loadedEntities.size(); i++)
	{
		
		if (loadedEntities[i]->getName() == "Player")
		{
			player->setPosition(loadedEntities[i]->getPosition());
			delete loadedEntities[i];
		}
		else if (loadedEntities[i]->getName() == "crate")
		{
			Crate* c = new Crate();
			c->setPosition(loadedEntities[i]->getPosition());
			c->setScale(loadedEntities[i]->getScale());
			c->setRotation(loadedEntities[i]->getRotation());
			c->setName(loadedEntities[i]->getName());
			delete loadedEntities[i];
			loadedEntities[i] = c;
			addEntity(c);
			crates.push_back(c);
			c->setLayer(2);
			
		}
		else if (loadedEntities[i]->getName() == "zombie")
		{
			Zombie* z = new Zombie();
			z->setPosition(loadedEntities[i]->getPosition());
			z->setName(loadedEntities[i]->getName());
			delete loadedEntities[i];
			loadedEntities[i] = z;
			addEntity(z);
			zombies.push_back(z);
			z->setLayer(2);
		}
		else
		{
			addEntity(loadedEntities[i]);
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
		if (Vector2(zombies[i]->getPosition(), player->getPosition()).magnitude() <= 500 && Vector2(zombies[i]->getPosition(), player->getPosition()).magnitude() >= 200)
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

AudioParticle * BaseLevel::createAudioParticle(std::string filePath)
{
	AudioParticle* particle = new AudioParticle(filePath);
	audioParticles.push_back(particle);
	return particle;
}
