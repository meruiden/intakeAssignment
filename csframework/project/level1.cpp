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

	ground = new Ground();
	addEntity(ground);
	ground->getPhysicsBody()->setPhysicsActive(true);
	ground->getPhysicsBody()->setPhysicsMode(PhysicsBody::STATIC);
	ground->setPosition(Vector2(0, 200));
	ground->setScale(Vector2(20, 1));

	this->playerJump = false;
	this->playerRight = false;
	this->playerLeft = false;

	camAcc = Vector2();
	camVel = Vector2();
	
	test = new Entity();
	addEntity(test);
	test->getPhysicsBody()->setPhysicsActive(true);
	test->setPosition(Vector2(100, 100));
	test->getPhysicsBody()->setCircleCollider(50);
	test->getPhysicsBody()->setDrawColliders(true);

	player->getPhysicsBody()->setBoxCollider(100, 256);
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
	
	
}


void Level1::fixedUpdate()
{

	Vector2 playerVelocity = player->getPhysicsBody()->getVelocity();
	if (playerVelocity.x > 10.0f)
	{
		player->getPhysicsBody()->setVelocity(Vector2(10.0f, playerVelocity.y));
	}

	if (playerVelocity.x < -10.0f)
	{
		player->getPhysicsBody()->setVelocity(Vector2(-10.0f, playerVelocity.y));
	}
	
	if (!playerLeft && !playerRight && player->isGrounded())
	{
		playerVelocity = player->getPhysicsBody()->getVelocity();
		player->getPhysicsBody()->setVelocity(Vector2(playerVelocity.x * 0.97f, playerVelocity.y));

	}
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

	player->setRotation(0);
}

 
Level1::~Level1()
{
	delete test;
	delete ground;
	delete playerGroundTrigger;
	delete player;
}