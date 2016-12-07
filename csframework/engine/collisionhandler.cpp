#include <engine/collisionhandler.h>

CollisionHandler::CollisionHandler(std::vector<Entity*>* entities) : b2ContactListener()
{
	this->entities = entities;
}


CollisionHandler::~CollisionHandler()
{

}

void CollisionHandler::BeginContact(b2Contact* contact)
{
	Entity* entityA;
	Entity* entityB;
	
	entityA = getEntityFromBody(contact->GetFixtureA()->GetBody());
	entityB = getEntityFromBody(contact->GetFixtureB()->GetBody());
	
	if (entityA != NULL && entityB != NULL)
	{
		entityA->onCollisionBegin(entityB);
		entityB->onCollisionBegin(entityA);
	}
}

void CollisionHandler::EndContact(b2Contact* contact)
{
	Entity* entityA;
	Entity* entityB;
	entityA = getEntityFromBody(contact->GetFixtureA()->GetBody());
	entityB = getEntityFromBody(contact->GetFixtureB()->GetBody());
	if(entityA != NULL && entityB != NULL)
	{
		entityA->onCollisionEnd(entityB);
		entityB->onCollisionEnd(entityA);
	}
}

Entity* CollisionHandler::getEntityFromBody(b2Body* body)
{
	std::vector< Entity* >::iterator it = entities->begin();
	while (it != entities->end())
	{
		if (body == (*it)->getPhysicsBody()->getBox2dBody()) {
			return (*it);
		}
		it++;
	}
	return NULL;
}
