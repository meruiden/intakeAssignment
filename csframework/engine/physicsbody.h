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

	void setBox2dBody(b2Body* body);
	void setPhysicsMode(int mode);
	void addForce(Vector2 force);
	void lockRotation(bool value);
	void setVelocity(Vector2 newVelocity);
	void setPhysicsActive(bool active);
	void setDrawColliders(bool active);
	void setTrigger(bool active);
	void setCollider(std::vector<Vector2> vertices);
	void setBoxCollider(float width, float height);
	void setCircleCollider(float radius);
	void setFixedRotation(bool active);
	void setEdgeCollider(std::vector<Vector2> vertices);
	void setFriction(float newFriction);
	void setDensity(float newDensity);
	
	Vector2 getVelocity();
	Mesh* getColliderDrawMesh() { return drawColliderMesh; }

	static const int KINIMATIC = 0;
	static const int STATIC = 1;
	static const int DYNAMIC = 2;
	bool getPhysicsActive() { return physicsActive; }
	bool getDrawColliders() { return drawColliders; }
	bool isTrigger() { return trigger; }
	bool hasCustomCollider() { return customCollider; }
	bool isRotationFixed() { return fixedRotation; }

	int getPhysicsMode();
	float getFriction() { return friction; }
	float getDensity() { return density; }

	void regenerateColliderMesh();
	std::vector<Vector2> getColliderVertices() { return preSetVerts; }

	enum colliderType
	{
		RelativeCollider,
		CicrleCollider,
		BoxCollider,
		CustomCollider,
		EdgeCollider
	};

	colliderType getColliderType() { return curColliderType; }

private:
	void destroyCollider();

	std::vector<Vector2> lastColliderVertices;
	std::vector<Vector2> preSetVerts;

	Vector2 preSetBoxColliderSize;
	float preSetCircleColliderRadius;

	b2Body* box2dBody;

	bool drawColliders;
	bool physicsActive;
	Mesh* drawColliderMesh;
	bool trigger;
	bool customCollider;
	bool fixedRotation;
	
	float lastCircleRadius;
	float friction;
	float density;

	int curPhysicsMode;
	colliderType curColliderType;
};

