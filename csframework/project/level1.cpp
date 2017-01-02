#include <project/level1.h>
 
Level1::Level1() : Scene()
{
	playerGroundTrigger = new PlayerGroundTrigger();
	player = new Player(playerGroundTrigger);
	addEntity(player);

	player->setPosition(Vector2(0, 100));

	addEntity(playerGroundTrigger);

	playerGroundTrigger->getPhysicsBody()->setPhysicsMode(PhysicsBody::DYNAMIC);
	playerGroundTrigger->getPhysicsBody()->setTrigger(true);

	this->playerJump = false;
	this->playerRight = false;
	this->playerLeft = false;

	player->getPhysicsBody()->setFixedRotation(true);
	
	player->getPhysicsBody()->setBoxCollider(100, 256);
	groundCollider = new Entity();
	mapWidth = 70;
	mapHeight = 2;
	createMap();

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

	leftArmPivot->setLayer(4);
	rightArmPivot->setLayer(2);

	bulletLaunchPos = new Entity();
	weapon->addChild(bulletLaunchPos);
	bulletLaunchPos->setPosition(Vector2(-150, -25));

	shootDelay = 0.1f;
	shootDelayCounter = 0.0f;

	
}

Level1::~Level1()
{

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

	for (int i = 0; i < bullets.size(); i++)
	{
		removeEntity(bullets[i]);
		delete bullets[i];
	}
	bullets.clear();

	for (int i = 0; i < groundTiles.size(); i++)
	{
		removeEntity(groundTiles[i]);
		delete groundTiles[i];
	}
	groundTiles.clear();

	removeEntity(groundCollider);
	delete groundCollider;

	removeEntity(playerGroundTrigger);
	delete playerGroundTrigger;

	removeEntity(player);
	delete player;

}

void Level1::update(float deltaTime)
{
	shootDelayCounter += deltaTime;
	handleInput();
	playerGroundTrigger->setPosition(player->getPosition() + Vector2(0, player->getHeight()/2.0f - playerGroundTrigger->getHeight()/2.0f));
	
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

	lookAtRotation = Vector2(getCamera()->screenToWorldSpace(input()->getMousePosition()), player->getPosition()).getAngle();
	
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
}

void Level1::handleInput()
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
		
	}
}


void Level1::fixedUpdate()
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
}

void Level1::createMap()
{
	int groundCollSize = 0;
	for (int w = 0; w < mapWidth; w++)
	{
		//Ground* groundTile = new Ground();
		//groundTile->setPosition(Vector2(-200, 300));
		//groundTile->getPhysicsBody()->setPhysicsActive(false);
		//groundTile->setPosition(groundTile->getPosition() + Vector2(w * 60, 0));
		//groundTiles.push_back(groundTile);
		groundCollSize += 60;
		//addEntity(groundTile);
		
		for (int h = 1; h < mapHeight; h++)
		{
			//Ground* groundTile = new Ground();
			//groundTile->setPosition(Vector2(-200, 300));
			//groundTile->getPhysicsBody()->setPhysicsActive(false);
			//groundTile->setPosition(groundTile->getPosition() + Vector2(w * 60, h * 60));
			//groundTiles.push_back(groundTile);
			//addEntity(groundTile);
		}
	}
	
	MapEditor::loadMap(loadedEntities);
	for (int i = 0; i < loadedEntities.size(); i++)
	{
		addEntity(loadedEntities[i]);
	}
	addEntity(groundCollider);
	
	groundCollider->getPhysicsBody()->setPhysicsActive(true);
	groundCollider->getPhysicsBody()->setPhysicsMode(PhysicsBody::STATIC);
	std::vector<Vector2> verts;
	verts.push_back(Vector2(0, 0));
	verts.push_back(Vector2(groundCollSize/2, 0));
	verts.push_back(Vector2(groundCollSize / 2 + 200, -100));
	verts.push_back(Vector2(groundCollSize / 2 + 400, -100));
	verts.push_back(Vector2(groundCollSize / 2 + 600, 0));
	verts.push_back(Vector2(groundCollSize, 0));

	groundCollider->getPhysicsBody()->setEdgeCollider(verts);
	groundCollider->setPosition(Vector2(-230, 300));
	groundCollider->getPhysicsBody()->setDrawColliders(true);
}

void Level1::checkBullets()
{
	std::vector<Bullet* >::iterator it = bullets.begin();
	while (it != bullets.end())
	{
		Bullet* b = (*it);
		if (b->mustDestroy())
		{
			removeEntity(b);
			delete b;
			bullets.erase(it);
		}
		else
		{
			++it;
		}
	}
}
