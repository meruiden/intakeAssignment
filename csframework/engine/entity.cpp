#include <engine/entity.h>

Entity::Entity()
{
	rotation = 0.0f;
	scale = Vector2(1, 1);
	position = Vector2();
	color = Color();
	sprite = NULL;
	parent = NULL;
	layer = 0;
	name = "No Name";
	physicsBody = new PhysicsBody();
	spritesheet = new SpriteSheet();
}

Entity::~Entity()
{
	delete physicsBody;
	delete spritesheet;
	if (sprite != NULL)
	{
		delete sprite;
	}
}

void Entity::update(float deltaTime) {

}

void Entity::onCollisionBegin(Entity* other)
{

}

void Entity::onCollisionEnd(Entity* other)
{

}

void Entity::setName(std::string newName)
{
	name = newName;
}

void Entity::setLayer(int value)
{
	if (value < 0)
	{
		layer = 0;
	}
	else {
		layer = value;
	}
}

void Entity::setPosition(Vector2 newPos)
{
	if (physicsBody->getBox2dBody() != NULL) 
	{
		physicsBody->getBox2dBody()->SetTransform(b2Vec2(newPos.x * 0.02f, newPos.y * 0.02f), physicsBody->getBox2dBody()->GetAngle());
	}

	position = newPos;

}

Vector2 Entity::getPosition()
{
	if (physicsBody->getBox2dBody() != NULL)
	{
		return Vector2(physicsBody->getBox2dBody()->GetPosition().x * 50.0f, physicsBody->getBox2dBody()->GetPosition().y * 50.0f);
	}

	return position;
}

void Entity::setRotation(float newRotation)
{
	if (physicsBody->getBox2dBody() != NULL)
	{
		physicsBody->getBox2dBody()->SetTransform(physicsBody->getBox2dBody()->GetPosition() , newRotation * DEG_TO_RAD);
		physicsBody->getBox2dBody()->SetAngularVelocity(0);
		physicsBody->getBox2dBody()->SetAngularDamping(0);
	}

	rotation = newRotation;

}

float Entity::getRotation()
{
	if (physicsBody->getBox2dBody() != NULL)
	{
		return physicsBody->getBox2dBody()->GetAngle() * RAD_TO_DEG;
	}

	return rotation;
}


void Entity::setScale(Vector2 newScale)
{
	if (scale == newScale)
	{
		return;
	}
	scale = newScale;
	if (physicsBody->getBox2dBody() == NULL || physicsBody->hasCustomCollider())
	{
		return;
	}

	b2Body* body = physicsBody->getBox2dBody();
	b2Fixture* fixture = body->GetFixtureList();
	body->DestroyFixture(fixture);
	b2PolygonShape shape;
	shape.SetAsBox(0.02f * (abs(getWidth() / 2.0f)), 0.02f * abs((getHeight() / 2.0f)));

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.0f;
	
	fixture = body->CreateFixture(&fixtureDef);
	fixture->SetSensor(getPhysicsBody()->isTrigger());

}


void Entity::addChild(Entity* entity) 
{
	if (entity->parent != NULL) {
		entity->parent->removeChild(entity);
	}
	
	std::vector< Entity* >::iterator it = children.begin();
	while (it != children.end())
	{
		if ((*it) == entity)
		{
			return;
		}
		else {
			++it;
		}
	}

	children.push_back(entity);
	entity->setParent((this));

}
void Entity::removeChild(Entity* entity) 
{
	std::vector< Entity* >::iterator it = children.begin();
	while (it != children.end())
	{
		if ((*it) == entity)
		{
			it = children.erase(it);
			entity->setParent(NULL);
		}
		else
		{
			++it;
		}
	}

	
}

void Entity::setParent(Entity* entity)
{
	
	if (entity == NULL) 
	{
		if (this->parent != NULL)
		{
			this->parent->removeChild(this);
		}

		this->parent = NULL;
		return;
	}

	this->parent = entity;
	/*int fixtureCounter = 0;
	for (b2Fixture* f = physicsBody->getBox2dBody()->GetFixtureList(); f; f = f->GetNext())
	{
		fixtureCounter++;
	}
	fixtureIndex = fixtureCounter;
	*/
	bool alreadyChild = false;
	std::vector<Entity*> children = entity->getChildren();
	std::vector< Entity* >::iterator it = children.begin();
	while (it != children.end())
	{
		if ((*it) == this)
		{
			alreadyChild = true;
		}

		it++;
	}

	if (!alreadyChild)
	{
		entity->addChild(this);
	}

}

float Entity::getWidth()
{
	if (sprite != NULL) 
	{
		return scale.x * sprite->getSpriteSize().x;
	}
	return scale.x;
}

float Entity::getHeight()
{
	if (sprite != NULL)
	{
		return scale.y * sprite->getSpriteSize().y;
	}
	return scale.y;
}

void Entity::addSprite(std::string path)
{
	if (sprite != NULL)
	{
		delete sprite;
	}
	sprite = new Sprite();
	sprite->setUpSprite(path);
}

void Entity::addSpriteAsSpriteSheet(std::string path, int spriteWidth, int spriteHeight, int spritesPerRow, int rows)
{
	spritesheet->setUpSpriteSheet(path, spriteWidth, spriteHeight, spritesPerRow, rows);
	this->sprite = spritesheet->getSprite();
}

