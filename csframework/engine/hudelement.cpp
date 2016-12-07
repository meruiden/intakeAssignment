#include <engine/hudelement.h>

HudElement::HudElement()
{
	sprite = NULL;
	layer = 0;
	position = Vector2();
	scale = Vector2(1, 1);
	rotation = 0.0f;
	color = Color();
	setAnchorPoint(HudElement::ANCHOR_CENTER_CENTER);
}

HudElement::~HudElement()
{
	if (sprite != NULL)
	{
		delete sprite;
	}
}

void HudElement::setAnchorPoint(int point)
{
	if (point >= 0 && point <= 8)
	{
		anchorPoint = point;
	}
}
void HudElement::setLayer(int value)
{
	if (value < 0)
	{
		layer = 0;
	}
	else {
		layer = value;
	}
}

void HudElement::setPosition(Vector2 newPosition)
{
	position = newPosition;
}

void HudElement::setScale(Vector2 newScale)
{
	scale = newScale;
}

void HudElement::setRotation(float newRotation)
{
	rotation = rotation;
}

void HudElement::update(float deltaTime)
{

}

Vector2 HudElement::getAnchoredPosition()
{
	Vector2 anchoredPosition = Vector2();
	Vector2 windowSize = Camera::getWindowSize();

	switch (getAnchorPoint())
	{
	case HudElement::ANCHOR_TOP_CENTER:
		anchoredPosition -= Vector2(0, windowSize.y / 2.0f);
		break;

	case HudElement::ANCHOR_TOP_LEFT:
		anchoredPosition -= Vector2(windowSize.x / 2.0f, windowSize.y / 2.0f);
		break;

	case HudElement::ANCHOR_TOP_RIGHT:
		anchoredPosition -= Vector2(-windowSize.x / 2.0f, windowSize.y / 2.0f);
		break;

	case HudElement::ANCHOR_CENTER_CENTER:
		anchoredPosition = Vector2();
		break;

	case HudElement::ANCHOR_CENTER_LEFT:
		anchoredPosition -= Vector2(windowSize.x / 2.0f, 0);
		break;

	case HudElement::ANCHOR_CENTER_RIGHT:
		anchoredPosition -= Vector2(-windowSize.x / 2.0f, 0);
		break;

	case HudElement::ANCHOR_BOTTOM_CENTER:
		anchoredPosition -= Vector2(0, -windowSize.y / 2.0f);
		break;

	case HudElement::ANCHOR_BOTTOM_LEFT:
		anchoredPosition -= Vector2(windowSize.x / 2.0f, -windowSize.y / 2.0f);
		break;

	case HudElement::ANCHOR_BOTTOM_RIGHT:
		anchoredPosition -= Vector2(-windowSize.x / 2.0f, -windowSize.y / 2.0f);
		break;
	}

	return anchoredPosition;
}

Vector2 HudElement::getGlobalPosition()
{
	return getPosition() + getAnchoredPosition();
}

float HudElement::getWidth()
{
	if (sprite != NULL)
	{
		return scale.x * sprite->getSpriteSize().x;
	}
	return scale.x;
}

float HudElement::getHeight()
{
	if (sprite != NULL)
	{
		return scale.y * sprite->getSpriteSize().y;
	}
	return scale.y;
}

bool HudElement::overLapsWithPoint(Vector2 point)
{
	bool overlaps = false;
	if (point.x < (getGlobalPosition().x + getWidth() / 2.0f)
		&& point.x >(getGlobalPosition().x - getWidth() / 2.0f)
		&& point.y < (getGlobalPosition().y + getHeight() / 2.0f)
		&& point.y > (getGlobalPosition().y - getHeight() / 2.0f)
		)
	{
		overlaps = true;
	}

	return overlaps;
}

void HudElement::addSprite(std::string path)
{
	if (sprite != NULL)
	{
		delete sprite;
	}
	sprite = new Sprite();
	sprite->setUpSprite(path);
}


