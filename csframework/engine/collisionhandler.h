#pragma once

#include <Box2D/Dynamics/b2WorldCallbacks.h>
#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/Contacts/b2Contact.h>
#include <iostream>
#include <vector>
#include <engine/entity.h>

class CollisionHandler : public b2ContactListener
{
public:
	CollisionHandler(std::vector<Entity*>* entities);
	~CollisionHandler();

	void BeginContact(b2Contact* contact);

	void EndContact(b2Contact* contact);

private:
	std::vector<Entity*>* entities;
	Entity* getEntityFromBody(b2Body* body);
};

