#pragma once

#include <vector>

#include <engine/sprite.h>
#include <engine/color.h>
#include <engine/vector2.h>
#include <engine/input.h>
#include <engine/spritesheet.h>
#include <engine/physicsbody.h>

class Entity
{
public:
	Entity();
	virtual ~Entity();
	virtual void update(float deltaTime);
	virtual void onCollisionBegin(Entity* other);
	virtual void onCollisionEnd(Entity* other);

	void addChild(Entity* entity);
	void removeChild(Entity* entity);
	void setParent(Entity* entity);
	void setPosition(Vector2 newPos);
	void setScale(Vector2 newScale);
	void setRotation(float newRotation);
	void setLayer(int value);
	void setName(std::string newName);
	void addSprite(std::string path);
	void addSpriteAsSpriteSheet(std::string path, int spriteWidth, int spriteHeight, int spritesPerRow, int rows);

	
	float getWidth();
	float getHeight();
	float getRotation();
	int getLayer() { return layer; }

	Color color;
	Sprite* getSprite() { return sprite; }

	SpriteSheet* getSpriteSheet() { return spritesheet; }

	Vector2 getPosition();
	Vector2 getScale() { return scale; }

	std::vector<Entity*> getChildren() { return children; }
	Entity* getParent() { return parent; }
	PhysicsBody* getPhysicsBody() { return physicsBody; }

	Input* input() { return Input::getInstance(); }

	std::string getName() { return name; }
	
	
private:
	int layer;

	std::vector<Entity*> children;
	Entity* parent;

	PhysicsBody* physicsBody;

	Vector2 scale;
	Vector2 position;

	float rotation;

	std::string name;

	SpriteSheet* spritesheet;
	
	
protected:
	Sprite* sprite;

};

