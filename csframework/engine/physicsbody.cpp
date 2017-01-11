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
	friction = 0.2f;
	density = 1.0f;
	curColliderType = colliderType::RelativeCollider;
	preSetBoxColliderSize = Vector2();
	preSetCircleColliderRadius = 0.0f;
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
	curColliderType = colliderType::BoxCollider;
	if (box2dBody == NULL)
	{
		preSetBoxColliderSize = Vector2(width, height);
		return;
	}
	b2Body* body = box2dBody;
	destroyCollider();
	b2PolygonShape shape;
	shape.SetAsBox(0.02f * (abs(width / 2.0f)), 0.02f * abs((height / 2.0f)));

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.0f;
	b2Fixture* fixture = body->CreateFixture(&fixtureDef);
	fixture->SetSensor(trigger);
	fixture->SetFriction(friction);

	customCollider = true;

	regenerateColliderMesh();

	
}

void PhysicsBody::setCircleCollider(float radius)
{
	curColliderType = colliderType::CicrleCollider;
	if (box2dBody == NULL)
	{
		preSetCircleColliderRadius = radius;
		return;
	}
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
	fixture->SetFriction(friction);
	customCollider = true;
	regenerateColliderMesh();
}


void PhysicsBody::setCollider(std::vector<Vector2> vertices)
{
	if (vertices.size() == 0)
	{
		return;
	}
	curColliderType = colliderType::CustomCollider;
	preSetVerts = vertices;
	if (box2dBody == NULL)
	{
		return;
	}
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
	fixture->SetFriction(friction);
	
	customCollider = true;

	regenerateColliderMesh();
	
}

void PhysicsBody::setEdgeCollider(std::vector<Vector2> vertices)
{
	if (vertices.size() == 0)
	{
		return;
	}
	curColliderType = colliderType::EdgeCollider;
	preSetVerts = vertices;
	if (box2dBody == NULL)
	{	
		return;
	}
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
		fixture->SetFriction(friction);

	}

	customCollider = true;
	
	regenerateColliderMesh();
}

void PhysicsBody::setFriction(float newFriction)
{
	friction = newFriction;
	if (box2dBody != NULL)
	{
		for (b2Fixture* f = box2dBody->GetFixtureList(); f; f = f->GetNext())
		{
			f->SetFriction(newFriction);
		}
	}
}

void PhysicsBody::setDensity(float newDensity)
{
	density = newDensity;
	if (box2dBody != NULL)
	{
		for (b2Fixture* f = box2dBody->GetFixtureList(); f; f = f->GetNext())
		{
			f->SetDensity(density);
		}
	}
}

void PhysicsBody::setBox2dBody(b2Body* body) 
{
	box2dBody = body;

	if (body != NULL)
	{
		switch (curColliderType)
		{
		case PhysicsBody::CicrleCollider:
			setCircleCollider(preSetCircleColliderRadius);
			break;
		case PhysicsBody::BoxCollider:
			setBoxCollider(preSetBoxColliderSize.x, preSetBoxColliderSize.y);
			break;
		case PhysicsBody::CustomCollider:
			setCollider(preSetVerts);
			break;
		case PhysicsBody::EdgeCollider:
			setEdgeCollider(preSetVerts);
			break;
		}
		setPhysicsActive(physicsActive);
		setPhysicsMode(curPhysicsMode);
		setFixedRotation(fixedRotation);
		setFriction(friction);
		setDensity(density);
		setTrigger(trigger);
	}
}

void PhysicsBody::setFixedRotation(bool active)
{
	fixedRotation = active;
	if (box2dBody != NULL)
	{
		box2dBody->SetFixedRotation(active);
	}
}

void PhysicsBody::setDrawColliders(bool active)
{
	drawColliders = active;
}

void PhysicsBody::regenerateColliderMesh()
{
	std::vector<Vector2> vertices;
	std::vector<Vector2> uvs;
	b2Shape* shape = box2dBody->GetFixtureList()->GetShape();

	if (shape->GetType() == b2Shape::Type::e_polygon)
	{
		b2PolygonShape *poly = (b2PolygonShape*)shape;

		for (unsigned int i = 0; i < poly->GetVertexCount(); i++)
		{
			b2Vec2 pos = poly->GetVertex(i);
			pos *= 50.0f;
			vertices.push_back(Vector2(pos.x, pos.y));
			uvs.push_back(Vector2(0.5f, 0.5f));
		}

		
	}else if (shape->GetType() == b2Shape::Type::e_circle)
	{

		b2CircleShape *poly = (b2CircleShape*)shape;
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
			vertices.push_back(Vector2(cosf(angle)*radius, sinf(angle)*radius));
			uvs.push_back(Vector2(0.5f, 0.5f));
		}
	}
	else if (shape->GetType() == b2Shape::Type::e_edge)
	{
		for (b2Fixture* f = box2dBody->GetFixtureList(); f; f = f->GetNext())
		{
			b2Shape* s = f->GetShape();
			b2EdgeShape *edge = (b2EdgeShape*)s;
			b2Vec2 pos1 = edge->m_vertex1;
			b2Vec2 pos2 = edge->m_vertex2;
			pos1 *= 50.0f;
			pos2 *= 50.0f;
			vertices.push_back(Vector2(pos1.x, pos1.y));
			vertices.push_back(Vector2(pos2.x, pos2.y));
			uvs.push_back(Vector2(0.5f, 0.5f));
			uvs.push_back(Vector2(0.5f, 0.5f));
			
		}

		vertices.push_back(vertices[vertices.size()-1]);
		uvs.push_back(Vector2(0.5f, 0.5f));
	}

	bool collidersSame = true;
	if (lastColliderVertices.size() == vertices.size())
	{
		for (int i = 0; i < lastColliderVertices.size(); i++)
		{
			if(lastColliderVertices[i] != vertices[i])
			{
				collidersSame = false;
				break;
			}
		}
	}
	else
	{
		collidersSame = false;
	}

	if (collidersSame && curColliderType != colliderType::CicrleCollider)
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
	if (curColliderType == colliderType::EdgeCollider)
	{
		drawColliderMesh->setDrawMode(Mesh::drawModeSettings::lines);
	}
	else
	{
		drawColliderMesh->setDrawMode(Mesh::drawModeSettings::polygons);
	}
	
	
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
	trigger = active;
	if (box2dBody == NULL)
	{
		return;
	}
	for (b2Fixture* f = box2dBody->GetFixtureList(); f; f = f->GetNext())
	{
		f->SetSensor(active);
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

