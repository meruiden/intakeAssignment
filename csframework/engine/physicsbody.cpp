#include <engine/physicsbody.h>

PhysicsBody::PhysicsBody()
{
	box2dBody = NULL;
	drawColliderMesh = NULL;
	drawColliders = false;
}

PhysicsBody::~PhysicsBody()
{
	if (drawColliderMesh != NULL)
	{
		delete drawColliderMesh;
	}
}


void PhysicsBody::setBox2dBody(b2Body* body, b2FixtureDef fixtureDef) 
{
	box2dBody = body;
	this->fixtureDef = fixtureDef;

	if (body != NULL)
	{
		setPhysicsActive(false);
	}
}

void PhysicsBody::setDrawColliders(bool active)
{
	drawColliders = active;
}

void PhysicsBody::regenerateColliderMesh()
{

	std::vector<glm::vec3> vertices;
	b2Shape* shape = box2dBody->GetFixtureList()->GetShape();

	if (shape->GetType() == b2Shape::Type::e_polygon)
	{
		b2PolygonShape *poly = (b2PolygonShape*)shape;

		for (unsigned int i = 0; i < poly->GetVertexCount(); i++)
		{
			b2Vec2 pos = poly->GetVertex(i);

			pos *= 50.0f;
			vertices.push_back(glm::vec3(pos.x, pos.y, 0.0f));
		}

	}

	if (lastColliderVertices == vertices)
	{
		return;
	}
	if (drawColliderMesh != NULL)
	{
		delete drawColliderMesh;
	}

	lastColliderVertices = vertices;

	drawColliderMesh = new Mesh();
	drawColliderMesh->setFromVertices(vertices);
	drawColliderMesh->setDrawMode(Mesh::drawModeSettings::polygons);
	
}
void PhysicsBody::setPhysicsMode(int mode)
{
	if (box2dBody == NULL) {
		return;
	}
	b2BodyType type = b2_staticBody;
	switch (mode)
	{
	case 0:
		type = b2_kinematicBody;
		break;
		
	case 1:
		type = b2_staticBody;
		break;

	case 2:
		type = b2_dynamicBody;
		break;
	}
	box2dBody->SetType(type);

}

void PhysicsBody::addForce(Vector2 force)
{
	if (box2dBody == NULL) {
		return;
	}
	box2dBody->ApplyForceToCenter(b2Vec2(force.x, force.y), true);
}

void PhysicsBody::lockRotation(bool value)
{
	if (box2dBody == NULL) {
		return;
	}
	box2dBody->SetFixedRotation(value);
}
Vector2 PhysicsBody::getVelocity()
{
	if (box2dBody == NULL) {
		return Vector2();
	}

	b2Vec2 vel = box2dBody->GetLinearVelocity();
	return Vector2(vel.x, vel.y);
}

void PhysicsBody::setVelocity(Vector2 newVelocity)
{
	if (box2dBody == NULL) {
		return;
	}
	
	box2dBody->SetLinearVelocity(b2Vec2(newVelocity.x, newVelocity.y));
}

int PhysicsBody::getPhysicsMode()
{
	if (box2dBody == NULL) {
		return STATIC;
	}
	int mode = STATIC;
	b2BodyType type = box2dBody->GetType();
	switch (type)
	{
	case b2_staticBody:
		mode = STATIC;
		break;
	case b2_kinematicBody:
		mode = KINIMATIC;
		break;
	case b2_dynamicBody:
		mode = DYNAMIC;
		break;
	}

	return mode;
}

void PhysicsBody::setPhysicsActive(bool active)
{
	
	for (b2Fixture* f = box2dBody->GetFixtureList(); f; f = f->GetNext())
	{
		f->SetSensor(!active);
	}

	if (active)
	{
		setPhysicsMode(PhysicsBody::DYNAMIC);
	}
	else 
	{
		setPhysicsMode(PhysicsBody::STATIC);
	}
	physicsActive = active;
}

