#include <project/level1.h>
 
Level1::Level1() : Scene()
{
	playerGroundTrigger = new PlayerGroundTrigger();
	player = new Player(playerGroundTrigger);
	addEntity(player);

	player->setPosition(Vector2(0, -200));

	addEntity(playerGroundTrigger);

	playerGroundTrigger->getPhysicsBody()->setPhysicsMode(PhysicsBody::DYNAMIC);
	playerGroundTrigger->getPhysicsBody()->setTrigger(true);

	this->playerJump = false;
	this->playerRight = false;
	this->playerLeft = false;

	camAcc = Vector2();
	camVel = Vector2();
	player->getPhysicsBody()->setFixedRotation(true);
	
	player->getPhysicsBody()->setBoxCollider(100, 256);
	groundCollider = new Entity();
	mapWidth = 200;
	mapHeight = 8;
	createMap();
	
}

Level1::~Level1()
{
	for (int i = 0; i < smilies.size(); i++)
	{
		removeEntity(smilies[i]);
		delete smilies[i];
	}
	smilies.clear();

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
	handleInput();
	playerGroundTrigger->setPosition(player->getPosition() + Vector2(0, player->getHeight()/2.0f - playerGroundTrigger->getHeight()/2.0f));
	
	if (playerRight || playerLeft)
	{
		if (player->getSprite()->getFileName() != "assets/character_walk.png" )
		{
			player->onWalk();
		}
	}
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
	
	if (input()->getMouseButtonDown(1))
	{
		Entity* smiley = new Entity();;
		addEntity(smiley);
		smiley->getPhysicsBody()->setPhysicsActive(true);
		smiley->setPosition(getCamera()->screenToWorldSpace(input()->getMousePosition()));

		smiley->getPhysicsBody()->setCircleCollider(64);
		smiley->addSprite("assets/smiley.png");
		smiley->setScale(Vector2(0.5f, 0.5f));
		smiley->getPhysicsBody()->setDrawColliders(true);
		smilies.push_back(smiley);
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
		player->getPhysicsBody()->addForce(Vector2(0, -10000.0f));
	}

	if (playerRight)
	{
		
		player->getPhysicsBody()->addForce(Vector2(400.0f, 0));
		if (player->getScale() != Vector2(1, 1))
		{
			player->setScale(Vector2(1, 1));
		}
	}

	if (playerLeft)
	{
		if (player->getScale() != Vector2(-1, 1))
		{
			player->setScale(Vector2(-1, 1));
		}
		player->getPhysicsBody()->addForce(Vector2(-400.0f, 0));
	}

	Vector2 camDir = Vector2(player->getPosition() + Vector2(250, 0), getCamera()->getPosition());
	if (camDir.magnitude() > 100.0f)
	{
		camAcc += camDir * 0.008f;
		
	}

	camVel += camAcc;
	getCamera()->setPosition(getCamera()->getPosition() + camVel);

	camAcc *= 0;

	camVel *= 0.9f;


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
	for (int i = 0; i < mapWidth; i++)
	{
		Ground* groundTile = new Ground();
		groundTile->setPosition(Vector2(-200, 300));
		groundTile->getPhysicsBody()->setPhysicsActive(false);
		groundTile->setPosition(groundTile->getPosition() + Vector2(i * (60), 0));
		groundTiles.push_back(groundTile);
		groundCollSize += 60;
		addEntity(groundTile);
		

	}

	addEntity(groundCollider);
	
	groundCollider->getPhysicsBody()->setPhysicsActive(true);
	groundCollider->getPhysicsBody()->setPhysicsMode(PhysicsBody::STATIC);
	groundCollider->getPhysicsBody()->setBoxCollider(groundCollSize, 10);
	groundCollider->setPosition(Vector2(groundTiles[mapWidth/2-1]->getPosition().x + 30, 300));
}
