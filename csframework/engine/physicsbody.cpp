#include <engine/physicsbody.h>

PhysicsBody::PhysicsBody()
{
	box2dBody = NULL;
	drawColliderMesh = NULL;
	drawColliders = false;
	trigger = false;
	customCollider = false;
	physicsActive = false;
	lastCircleRadius = 0.0f;
	fixedRotation = false;
	curPhysicsMode = PhysicsBody::DYNAMIC;
}

PhysicsBody::~PhysicsBody()
{
	if (drawColliderMesh != NULL)
	{
		delete drawColliderMesh;
	}
}

void PhysicsBody::setBoxCollider(float width, float height)
{

	b2Body* body = box2dBody;
	destroyCollider();
	b2PolygonShape shape;
	shape.SetAsBox(0.02f * (abs(width / 2.0f)), 0.02f * abs((height / 2.0f)));

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.0f;

	b2Fixture* fixture = body->CreateFixture(&fixtureDef);
	fixture->SetSensor(trigger);

	customCollider = true;

	regenerateColliderMesh();

	
}

void PhysicsBody::setCircleCollider(float radius)
{
	radius *= 0.02f;
	b2Body* body = box2dBody;
	destroyCollider();
	b2CircleShape shape;

	shape.m_radius = radius;

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.0f;

	b2Fixture* fixture = body->CreateFixture(&fixtureDef);
	fixture->SetSensor(trigger);

	customCollider = true;
	regenerateColliderMesh();
	
}


void PhysicsBody::setCollider(std::vector<Vector2> vertices)
{
	std::vector<b2Vec2> b2verts;
	for (unsigned int i = 0; i < vertices.size(); i++)
	{
		b2verts.push_back(b2Vec2(vertices[i].x * 0.02f, vertices[i].y * 0.02f));
	}
	b2Body* body = box2dBody;
	destroyCollider();
	b2PolygonShape shape;

	shape.Set(b2verts.data(), b2verts.size());

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.0f;

	b2Fixture* fixture = body->CreateFixture(&fixtureDef);
	fixture->SetSensor(trigger);
	
	customCollider = true;

	regenerateColliderMesh();
	
}

void PhysicsBody::setEdgeCollider(std::vector<Vector2> vertices)
{
	b2Body* body = box2dBody;
	destroyCollider();
	b2EdgeShape shape;
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.0f;

	
	for (unsigned int i = 0; i < vertices.size()-1; i++)
	{
		shape.Set(b2Vec2(vertices[i].x * 0.02f, vertices[i].y * 0.02f), b2Vec2(vertices[i+1].x * 0.02f, vertices[i+1].y * 0.02f));
		b2Fixture* fixture = body->CreateFixture(&fixtureDef);
		fixture->SetSensor(trigger);

	}

	customCollider = true;
	
	regenerateColliderMesh();
}

void PhysicsBody::setBox2dBody(b2Body* body, b2FixtureDef fixtureDef) 
{
	box2dBody = body;
	this->fixtureDef = fixtureDef;

	if (body != NULL)
	{
		setPhysicsActive(physicsActive);
		setPhysicsMode(curPhysicsMode);
	}
}

void PhysicsBody::setFixedRotation(bool active)
{
	fixedRotation = active;
	box2dBody->SetFixedRotation(active);
}

void PhysicsBody::setDrawColliders(bool active)
{
	drawColliders = active;
}

void PhysicsBody::regenerateColliderMesh()
{
	bool isCircle = false;
	bool isEdge = false;
	

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	b2Shape* shape = box2dBody->GetFixtureList()->GetShape();

	if (shape->GetType() == b2Shape::Type::e_polygon)
	{
		b2PolygonShape *poly = (b2PolygonShape*)shape;

		for (unsigned int i = 0; i < poly->GetVertexCount(); i++)
		{
			b2Vec2 pos = poly->GetVertex(i);
			pos *= 50.0f;
			vertices.push_back(glm::vec3(pos.x, pos.y, 0.0f));
			uvs.push_back(glm::vec2(0.5f, 0.5f));
		}

		
	}else if (shape->GetType() == b2Shape::Type::e_circle)
	{

		b2CircleShape *poly = (b2CircleShape*)shape;
		isCircle = true;
		float radius = poly->m_radius;
		radius *= 50.0f;
		if (lastCircleRadius == radius)
		{
			return;
		}
		lastCircleRadius = radius;
		int segments = CIRCLE_COLLIDER_DRAW_SEGMENTS;
		for (unsigned int i = 0; i < segments; i++)
		{
			float angle = 2.0f * PI * float(i) / float(segments);
			vertices.push_back(glm::vec3(cosf(angle)*radius, sinf(angle)*radius, 0.0f));
			uvs.push_back(glm::vec2(0.5f, 0.5f));
		}
	}
	else if (shape->GetType() == b2Shape::Type::e_edge)
	{
		int ii = 0;
		for (b2Fixture* f = box2dBody->GetFixtureList(); f; f = f->GetNext())
		{
			b2Shape* s = f->GetShape();
			b2EdgeShape *edge = (b2EdgeShape*)s;
			b2Vec2 pos1 = edge->m_vertex1;
			b2Vec2 pos2 = edge->m_vertex2;
			pos1 *= 50.0f;
			pos2 *= 50.0f;
			vertices.push_back(glm::vec3(pos1.x, pos1.y, 0.0f));
			vertices.push_back(glm::vec3(pos2.x, pos2.y, 0.0f));
			uvs.push_back(glm::vec2(0.5f, 0.5f));
			uvs.push_back(glm::vec2(0.5f, 0.5f));
			ii++;
			
		}

		vertices.push_back(vertices[vertices.size()-1]);
		uvs.push_back(glm::vec2(0.5f, 0.5f));
		isEdge = true;
	}

	if (lastColliderVertices == vertices && !isCircle)
	{
		return;
	}
	if (vertices.size() == 0)
	{
		if (drawColliderMesh != NULL)
		{
			delete drawColliderMesh;
			drawColliderMesh = NULL;
		}
		return;
	}
	
	if (drawColliderMesh != NULL)
	{
		delete drawColliderMesh;
		drawColliderMesh = NULL;
	}

	lastColliderVertices = vertices;



	drawColliderMesh = new Mesh();
	drawColliderMesh->setFromVerticesAndUvs(vertices, uvs);
	if (isEdge)
	{
		drawColliderMesh->setDrawMode(Mesh::drawModeSettings::lines);
	}
	else
	{
		drawColliderMesh->setDrawMode(Mesh::drawModeSettings::polygons);
	}
	
	
}
std::vector<Vector2> PhysicsBody::getColliderVertices()
{
	std::vector<Vector2> v2Verts;
	for (int i = 0; i < lastColliderVertices.size(); i++)
	{
		v2Verts.push_back(Vector2(lastColliderVertices[i].x, lastColliderVertices[i].y));
	}
	return v2Verts;
}
void PhysicsBody::destroyCollider()
{
	std::vector<b2Fixture*> toDestroy;
	for (b2Fixture* f = box2dBody->GetFixtureList(); f; f = f->GetNext())
	{
		toDestroy.push_back(f);
	}
	for (int i = 0; i < toDestroy.size(); i++)
	{
		box2dBody->DestroyFixture(toDestroy[i]);
	}
	toDestroy.clear();
}
void PhysicsBody::setPhysicsMode(int mode)
{
	curPhysicsMode = mode;
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
		return curPhysicsMode;
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


void PhysicsBody::setTrigger(bool active)
{
	for (b2Fixture* f = box2dBody->GetFixtureList(); f; f = f->GetNext())
	{
		f->SetSensor(active);
		trigger = active;
	}
}

void PhysicsBody::setPhysicsActive(bool active)
{
	physicsActive = active;

	if (box2dBody == NULL)
	{
		return;
	}
	box2dBody->SetActive(active);
}

