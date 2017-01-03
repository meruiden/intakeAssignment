#include <engine/scene.h>

Scene::Scene()
{
	physicsWorld = new b2World(b2Vec2(0, GRAVITY));
	collisionHandler = new CollisionHandler(&entities);
	physicsWorld->SetContactListener(collisionHandler);
	physicsWorld->SetAllowSleeping(false);
	camera = new Camera();
}

void Scene::update(float deltaTime) 
{

}

void Scene::createPhysicsFor(Entity* entity) 
{
	float rotation = entity->getRotation();
	b2BodyDef bodydef;
	bodydef.position.Set(entity->getPosition().x * 0.02f, entity->getPosition().y * 0.02f);
	bodydef.type = b2_dynamicBody;
	b2Body* body = physicsWorld->CreateBody(&bodydef);
	b2PolygonShape shape;

	float width = 1.0f;
	float height = 1.0f;

	if (entity->getWidth() != 0 && entity->getHeight() != 0)
	{
		width = 0.02f * (entity->getWidth() / 2.0f);
		height = 0.02f * (entity->getHeight() / 2.0f);
	}
	shape.SetAsBox(width, height);

	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;

	body->CreateFixture(&fixture);
	body->SetUserData(body);
	entity->getPhysicsBody()->setBox2dBody(body);
	
	physicBodies.push_back(body);
	entity->setRotation(rotation);
}

void Scene::fixedUpdate()
{

}

void Scene::removePhysicsFor(Entity* entity)
{
	if (entity->getPhysicsBody()->getBox2dBody() == NULL)
	{
		return;
	}
	std::vector< b2Body* >::iterator it = physicBodies.begin();
	while (it != physicBodies.end())
	{
		if ((*it) == entity->getPhysicsBody()->getBox2dBody())
		{
			Vector2 entityPos = entity->getPosition();
			physicsWorld->DestroyBody((*it));	
			b2FixtureDef emptyFixture;
			entity->getPhysicsBody()->setBox2dBody(NULL);
			entity->setPosition(entityPos);
			it = physicBodies.erase(it);

		}
		else
		{
			
			++it;
		}
	}
}


void Scene::addEntity(Entity* entity) 
{
	std::vector< Entity* >::iterator it = entities.begin();
	while (it != entities.end())
	{
		if ((*it) == entity) 
		{
			return;
		}
		else {
			++it;
		}
	}
	createPhysicsFor(entity);
	entities.push_back(entity);
}

void Scene::removeEntity(Entity* entity) 
{
	std::vector< Entity* >::iterator it = entities.begin();
	while (it != entities.end())
	{
		if ((*it) == entity)
		{
			removePhysicsFor((*it));
			it = entities.erase(it);
		}
		else 
		{
			++it;
		}
	}
}

void Scene::addHudElement(HudElement* element)
{
	std::vector< HudElement* >::iterator it = hudElements.begin();
	while (it != hudElements.end())
	{
		if ((*it) == element)
		{
			return;
		}
		else {
			++it;
		}
	}

	hudElements.push_back(element);
}

void Scene::removeHudElement(HudElement* element)
{
	std::vector< HudElement* >::iterator it = hudElements.begin();
	while (it != hudElements.end())
	{
		if ((*it) == element)
		{
			it = hudElements.erase(it);
		}
		else
		{
			++it;
		}
	}
}

Scene::~Scene()
{
	delete collisionHandler;
	physicsWorld->SetContactListener(NULL);
	for(unsigned int i = 0; i < physicBodies.size(); i++)
	{
		physicsWorld->DestroyBody(physicBodies[i]);
	}
	physicBodies.clear();
	
	delete physicsWorld;
	
	delete camera;
}
