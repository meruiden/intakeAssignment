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
	b2Fixture* fixture = body->GetFixtureList();
	body->DestroyFixture(fixture);
	b2PolygonShape shape;
	shape.SetAsBox(0.02f * (abs(width / 2.0f)), 0.02f * abs((height / 2.0f)));

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.0f;

	b2Filter filter;
	filter.maskBits = 0x0001;
	filter.categoryBits = 0x0001;
	fixtureDef.filter = filter;
	fixture = body->CreateFixture(&fixtureDef);
	fixture->SetSensor(trigger);

	customCollider = true;

	regenerateColliderMesh();

	
}

void PhysicsBody::setCircleCollider(float radius, int segments)
{
	b2Body* body = box2dBody;
	b2Fixture* fixture = body->GetFixtureList();
	body->DestroyFixture(fixture);
	b2CircleShape shape;

	shape.m_radius = radius;

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.0f;

	b2Filter filter;
	filter.maskBits = 0x0001;
	filter.categoryBits = 0x0001;
	fixtureDef.filter = filter;
	fixture = body->CreateFixture(&fixtureDef);
	fixture->SetSensor(trigger);

	customCollider = true;
	regenerateColliderMesh();
	
}


void PhysicsBody::setCollider(std::vector<Vector2> vertices)
{
	std::vector<b2Vec2> b2verts;
	for (unsigned int i = 0; i < vertices.size(); i++)
	{
		b2verts.push_back(b2Vec2(vertices[i].x, vertices[i].y));
	}
	b2Body* body = box2dBody;
	b2Fixture* fixture = body->GetFixtureList();
	body->DestroyFixture(fixture);
	b2PolygonShape shape;

	shape.Set(b2verts.data(), b2verts.size());

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.0f;

	b2Filter filter;
	filter.maskBits = 0x0001;
	filter.categoryBits = 0x0001;
	fixtureDef.filter = filter;
	fixture = body->CreateFixture(&fixtureDef);
	fixture->SetSensor(trigger);
	
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
	}
}

void PhysicsBody::setDrawColliders(bool active)
{
	drawColliders = active;
}

void PhysicsBody::regenerateColliderMesh()
{
	bool isCircle = false;

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
		}
	}
	
	if (lastColliderVertices == vertices && !isCircle)
	{
		return;
	}
	if (vertices.size() == 0)
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
	for (b2Fixture* f = box2dBody->GetFixtureList(); f; f = f->GetNext())
	{
		b2Filter filter = f->GetFilterData();
		if (active)
		{
			filter.categoryBits = 0x0001;
			setPhysicsMode(PhysicsBody::DYNAMIC);
		}
		else
		{
			filter.categoryBits = 0x0002;
			setPhysicsMode(PhysicsBody::STATIC);
		}
		
		f->SetFilterData(filter);
	}
}

