#pragma once

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
	void setLayer(int value);
	void setPosition(Vector2 newPosition);
	void setScale(Vector2 newScale);
	void setRotation(float newRotation);
	void addSprite(std::string path);

	Vector2 getPosition() { return position; }
	Vector2 getScale() { return scale; }
	Vector2 getAnchoredPosition();
	Vector2 getGlobalPosition();

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
private:
	int layer;
	Vector2 position;
	Vector2 scale;
	float rotation;
	int anchorPoint;
protected:
	Sprite* sprite;
};
