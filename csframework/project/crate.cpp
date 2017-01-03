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
		part->setPosition(this->getPosition() + Vector2((rand() % 200) - 100, (rand() % 200) - 100));
		part->setScale(getScale());
		
	}
	return parts;
}
