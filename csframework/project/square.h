#pragma once
#include <engine/entity.h>

class Square : public Entity
{
public:
	Square(bool isblue);
	~Square();

	void update(float deltaTime);
	void onCollisionBegin(Entity* other);
	void onCollisionEnd(Entity* other);

	bool killme;

private:
	int collisions;
	bool blue;
};

