#include <project/level1.h>

Level1::Level1() : BaseLevel()
{
	loadMap("assets/level1.map");
}


Level1::~Level1()
{

}

void Level1::update(float deltaTime)
{
	BaseLevel::update(deltaTime);
}
