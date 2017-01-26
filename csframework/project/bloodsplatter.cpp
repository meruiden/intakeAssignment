#include <project/bloodsplatter.h>

BloodSplatter::BloodSplatter(float impactAngle) : Entity()
{
	this->addSpriteAsSpriteSheet("assets/images/blood_splatters.png", 32, 32, 2, 2);
	this->getSpriteSheet()->setSprite(rand() % 4);
	impactAngle -= 90;
	impactAngle += (rand() % 45) - 45;
	this->direction = Vector2(std::sin(impactAngle * DEG_TO_RAD), std::cos(impactAngle * DEG_TO_RAD));
	this->speed = 10;
	this->setRotation(impactAngle);
	alpha = 255;
}


BloodSplatter::~BloodSplatter()
{

}

void BloodSplatter::update(float deltaTime)
{
	this->setPosition(this->getPosition() + direction * speed);
	if (this->color.a > 1)
	{
		alpha -= deltaTime * 1000;
		if (alpha < 1)
		{
			alpha = 1;
		}
	}

	this->color.a = alpha;
}
