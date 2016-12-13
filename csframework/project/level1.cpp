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
	ground->setPosition(Vector2(0, 400));
	ground->setScale(Vector2(20, 1));

	this->playerJump = false;
	this->playerRight = false;
	this->playerLeft = false;
}

void Level1::update(float deltaTime)
{
	handleInput();
	playerGroundTrigger->setPosition(player->getPosition() + Vector2(0, player->getHeight()/2.0f)-playerGroundTrigger->getWidth()/2.0f);
	
}

void Level1::handleInput()
{
	if (input()->getKeyDown(SDLK_SPACE) && player->isGrounded())
	{
		playerJump = true;
	}

	if (input()->getKeyDown(SDLK_d))
	{
		player->setScale(Vector2(1, 1));
		playerRight = true;
	}

	if (input()->getKeyDown(SDLK_a))
	{
		player->setScale(Vector2(-1, 1));
		playerLeft = true;
	}

	if (input()->getKeyUp(SDLK_d))
	{
		playerRight = false;
	}

	if (input()->getKeyUp(SDLK_a))
	{
		playerLeft = false;
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
	if (!player->isGrounded())
	{
		player->getPhysicsBody()->addForce(Vector2(0, 300.0f));
	}
	if (playerJump)
	{
		playerJump = false;
		player->getPhysicsBody()->addForce(Vector2(0, -14000.0f));
	}

	if (playerRight)
	{
		
		player->getPhysicsBody()->addForce(Vector2(400.0f, 0));
	}

	if (playerLeft)
	{
		
		player->getPhysicsBody()->addForce(Vector2(-400.0f, 0));
	}
}
 
Level1::~Level1()
{
	delete ground;
	delete playerGroundTrigger;
	delete player;
}