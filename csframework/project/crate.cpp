#include <project/crate.h>

Crate::Crate() : Entity()
{
	addSprite("assets/crate/crate_full.png");
	getPhysicsBody()->setPhysicsActive(true);
	setName("crate");
	health = 50;
}


Crate::~Crate()
{

}

void Crate::applyDamage(float damage)
{
	health -= damage;
	if (health < 0)
	{
		health = 0;
	}
}

std::vector<CratePart*> Crate::getParts()
{
	std::vector<CratePart*> parts;
	for (int i = 1; i <= 9; i++)
	{
		CratePart* part = new CratePart(i);
		parts.push_back(part);
		Vector2 partPosition;
		float partRotation = 0;
		switch (i)
		{
		case 1:
			partPosition = Vector2(43, 34);
			break;
		case 2:
			partPosition = Vector2(-7, 48);
			break;
		case 3:
			partPosition = Vector2(-42, 37);
			break;
		case 4:
			partPosition = Vector2(13, 23);
			partRotation = 15;
			break;
		case 5:
			partPosition = Vector2(-39, -10);
			partRotation = 7;
			break;
		case 6:
			partPosition = Vector2(-34, -34);
			break;
		case 7:
			partPosition = Vector2(11, -30);
			break;
		case 8:
			partPosition = Vector2(45, -40);
			break;
		case 9:
			partPosition = Vector2(42, -17);
			break;
		}
		partPosition += getPosition();
		part->setPosition(partPosition);
		part->setRotation(partRotation);
		part->setScale(getScale());
		
	}
	return parts;
}
