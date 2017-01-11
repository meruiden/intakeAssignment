#pragma once
#include <vector>

#include <engine/color.h>
#include <engine/vector2.h>
#include <engine/sprite.h>
#include <engine/camera.h>
#include <engine/input.h>
class HudElement
{
public:
	HudElement();
	virtual ~HudElement();

	virtual void update(float deltaTime);
	void addChild(HudElement* entity);
	void removeChild(HudElement* entity);
	void setParent(HudElement* entity);
	void setLayer(int value);
	void setPosition(Vector2 newPosition);
	void setScale(Vector2 newScale);
	void setRotation(float newRotation);
	void addSprite(std::string path);
	void addSprite(Sprite* sprite);
	void setGlobals(Vector2 pos, Vector2 scal, float rot);

	Vector2 getPosition() { return position; }
	Vector2 getScale() { return scale; }
	Vector2 getAnchorPosition();
	Vector2 getAnchoredPosition();
	Vector2 getGlobalPosition() { return globalPosition; }
	Vector2 getGlobalScale() { return globalScale; }
	float getGlobalRotation() { return globalRotation; }

	float getRotation() { return rotation; }
	int getLayer() { return layer; }
	float getWidth();
	float getHeight();

	Sprite* getSprite() { return sprite; }

	Color color;

	static const int ANCHOR_TOP_CENTER = 0;
	static const int ANCHOR_TOP_LEFT = 1;
	static const int ANCHOR_TOP_RIGHT = 2;
	static const int ANCHOR_CENTER_CENTER = 3;
	static const int ANCHOR_CENTER_LEFT = 4;
	static const int ANCHOR_CENTER_RIGHT = 5;
	static const int ANCHOR_BOTTOM_CENTER = 6;
	static const int ANCHOR_BOTTOM_LEFT = 7;
	static const int ANCHOR_BOTTOM_RIGHT = 8;
	
	int getAnchorPoint() { return anchorPoint; }
	void setAnchorPoint(int point);

	bool overLapsWithPoint(Vector2 point);

	Input* input() { return Input::getInstance(); }

	std::vector<HudElement*> getChildren() { return children; }
	HudElement* getParent() { return parent; }
private:
	int layer;
	Vector2 position;
	Vector2 scale;
	Vector2 globalPosition;
	Vector2 globalScale;
	float globalRotation;
	float rotation;
	int anchorPoint;

	std::vector<HudElement*> children;
	HudElement* parent;
protected:
	Sprite* sprite;
};
