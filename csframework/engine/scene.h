#pragma once

#include <vector>

#include <Box2D/Box2D.h>

#include <engine/entity.h>
#include <engine/hudelement.h>
#include <engine/config.h>
#include <engine/camera.h>
#include <engine/input.h>
#include <engine/collisionhandler.h>

class Scene
{
public:
	Scene();
	virtual ~Scene();

	virtual void update(float deltaTime);
	virtual void fixedUpdate();

	void addEntity(Entity* entity);
	void removeEntity(Entity* entity);
	
	void addHudElement(HudElement* entity);
	void removeHudElement(HudElement* entity);

	Input* input() { return Input::getInstance(); }
	std::vector <Entity*> getEntities() { return entities; }
	std::vector <HudElement*> getHudElements() { return hudElements; }

	b2World* getPhysicsWorld() { return physicsWorld; }
	Camera* getCamera() { return camera; }
private:
	std::vector<Entity*> entities;
	std::vector<HudElement*> hudElements;
	std::vector<b2Body*> physicBodies;
	b2World* physicsWorld;
	Camera* camera;
	CollisionHandler* collisionHandler;
	void createPhysicsFor(Entity* entity);
	void removePhysicsFor(Entity* entity);
};

