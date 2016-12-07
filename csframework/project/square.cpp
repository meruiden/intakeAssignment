#include <project/square.h>

Square::Square(bool isBlue)
{
	collisions = 0;
	killme = false;
	blue = isBlue;
	std::cout << "New cube" << std::endl;
	addSprite("assets/square.png");
}

void Square::update(float deltaTime)
{
	if (collisions > 0)
	{
		if (blue)
		{
			color = DARK_BLUE;
		}
		else
		{
			color = DARK_RED;
		}
	}
	else {
		if (blue)
		{
			color = LIGHT_BLUE;
		}
		else
		{
			color = LIGHT_RED;
		}
	}
}
void Square::onCollisionBegin(Entity* other)
{
	collisions++;

	if (blue)
	{
		if (other->getName() == "redBlokje")
		{
			killme = true;
		}
	}
	else 
	{
		if (other->getName() == "blueBlokje")
		{
			killme = true;
		}
	}
}
void Square::onCollisionEnd(Entity* other)
{
	collisions--;
}



Square::~Square()
{

}
