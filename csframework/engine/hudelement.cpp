#include <engine/hudelement.h>

HudElement::HudElement()
{
	parent = NULL;
	sprite = NULL;
	layer = 0;
	position = Vector2();
	scale = Vector2(1, 1);
	globalPosition = Vector2();
	globalScale = Vector2(1, 1);
	globalRotation = 0.0f;
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
	rotation = newRotation;
}

void HudElement::update(float deltaTime)
{

}

Vector2 HudElement::getAnchorPosition()
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

Vector2 HudElement::getAnchoredPosition()
{
	return getPosition() + getAnchorPosition();
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

void HudElement::addSprite(Sprite * sprite)
{
	if (this->sprite != NULL)
	{
		delete this->sprite;
	}
	this->sprite = sprite;
}

void HudElement::addChild(HudElement* element)
{
	if (element->parent != NULL) {
		element->parent->removeChild(element);
	}

	std::vector< HudElement* >::iterator it = children.begin();
	while (it != children.end())
	{
		if ((*it) == element)
		{
			return;
		}
		else {
			++it;
		}
	}

	children.push_back(element);
	element->setParent((this));

}
void HudElement::removeChild(HudElement* element)
{
	std::vector< HudElement* >::iterator it = children.begin();
	while (it != children.end())
	{
		if ((*it) == element)
		{
			it = children.erase(it);
			element->setParent(NULL);
		}
		else
		{
			++it;
		}
	}


}

void HudElement::setParent(HudElement* element)
{

	if (element == NULL)
	{
		if (this->parent != NULL)
		{
			this->parent->removeChild(this);
		}

		this->parent = NULL;
		return;
	}

	this->parent = element;
	bool alreadyChild = false;
	std::vector<HudElement*> children = element->getChildren();
	std::vector< HudElement* >::iterator it = children.begin();
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
		element->addChild(this);
	}

}

void HudElement::setGlobals(Vector2 pos, Vector2 scal, float rot)
{
	this->globalPosition = pos - Camera::getWindowSize() / 2.0f;
	this->globalScale = scal;
	this->globalRotation = rot;
}


