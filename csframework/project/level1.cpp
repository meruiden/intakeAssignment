#include <project/level1.h>
 
Level1::Level1() : Scene()
{
	player = new Player();
	addEntity(player);

	player->setPosition(Vector2(0, -200));
	player->getPhysicsBody()->setPhysicsActive(true);


}

void Level1::update(float deltaTime)
{

}
 
Level1::~Level1()
{
	delete player;
}