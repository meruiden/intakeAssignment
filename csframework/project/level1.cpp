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
	mapWidth = 50;
	mapHeight = 80;
	createMap();
	
	info = new TextMesh();
	info->loadFont("assets/arial.ttf");
	info->setText("Jump down to see how many blocks\nthere actualy are in the scene :)");
	addEntity(info);
	info->setPosition(Vector2(2400, 0));
	mustSecondClick = false;
	clickCollider = new Entity();
	addEntity(clickCollider);
	clickCollider->getPhysicsBody()->setPhysicsMode(PhysicsBody::STATIC);
	updateColCounter = 0;
	rightClicked = false;
}

Level1::~Level1()
{
	delete clickCollider;
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

	removeEntity(info);
	delete info;
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
		leftClicked = true;
	}

	if (input()->getMouseButtonDown(3))
	{
		rightClicked = true;
	}
}


void Level1::fixedUpdate()
{

	updateColCounter += 1.0f / 60.0f;
	static bool initClickCollider = true;
	if (leftClicked)
	{
		leftClicked = false;
		if (initClickCollider)
		{
			initClickCollider = false;
			clickCollider->getPhysicsBody()->setPhysicsActive(true);
			clickCollider->getPhysicsBody()->setDrawColliders(true);
			clickColVerts.push_back(getCamera()->screenToWorldSpace(input()->getMousePosition()));
		}
		
		clickColVerts.push_back(getCamera()->screenToWorldSpace(input()->getMousePosition()));

		mustSecondClick = true;
	}

	if (mustSecondClick && updateColCounter >= 0.01f)
	{
		clickColVerts[clickColVerts.size() - 1] = getCamera()->screenToWorldSpace(input()->getMousePosition());
		clickCollider->getPhysicsBody()->setEdgeCollider(clickColVerts);

		updateColCounter = 0;
	}
	if (rightClicked)
	{
		rightClicked = false;
		mustSecondClick = false;
	}
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
		Ground* groundTile = new Ground();
		groundTile->setPosition(Vector2(-200, 300));
		groundTile->getPhysicsBody()->setPhysicsActive(false);
		groundTile->setPosition(groundTile->getPosition() + Vector2(w * 60, 0));
		groundTiles.push_back(groundTile);
		groundCollSize += 60;
		addEntity(groundTile);
		
		for (int h = 1; h < mapHeight; h++)
		{
			Ground* groundTile = new Ground();
			groundTile->setPosition(Vector2(-200, 300));
			groundTile->getPhysicsBody()->setPhysicsActive(false);
			groundTile->setPosition(groundTile->getPosition() + Vector2(w * 60, h * 60));
			groundTiles.push_back(groundTile);
			addEntity(groundTile);
		}
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
