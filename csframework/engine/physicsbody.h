#pragma once

#include <engine/vector2.h>
#include <Box2D/Box2D.h>
#include <iostream>
#include <engine/mesh.h>
#include <engine/resourcemanager.h>

class PhysicsBody
{
public:
	PhysicsBody();
	virtual ~PhysicsBody();

	b2Body* getBox2dBody() { return box2dBody; }

	void setBox2dBody(b2Body* body, b2FixtureDef fixtureDef);
	void setPhysicsMode(int mode);
	void addForce(Vector2 force);
	void lockRotation(bool value);
	void setVelocity(Vector2 newVelocity);
	void setPhysicsActive(bool active);
	void setDrawColliders(bool active);
	

	b2FixtureDef getFixtureDef(){return fixtureDef;}
	Vector2 getVelocity();
	Mesh* getColliderDrawMesh() { return drawColliderMesh; }

	static const int KINIMATIC = 0;
	static const int STATIC = 1;
	static const int DYNAMIC = 2;
	bool getPhysicsActive() { return physicsActive; }
	bool getDrawColliders() { return drawColliders; }
	int getPhysicsMode();

	void regenerateColliderMesh();
private:
	std::vector<glm::vec3>lastColliderVertices;

	b2Body* box2dBody;
	b2FixtureDef fixtureDef;

	bool drawColliders;
	bool physicsActive;
	Mesh* drawColliderMesh;
};

