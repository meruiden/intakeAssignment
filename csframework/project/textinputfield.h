#pragma once

#include <engine/hudelement.h>
#include <engine/hudtext.h>

class TextInputField : public HudElement
{
public:
	TextInputField();
	virtual ~TextInputField();
	virtual void update(float deltaTime);

	void setSelected(bool value);
	void setSelectAble(bool value);
	void setText(std::string text);

	bool isSelected() { return selected; }
	bool isSelectAble() { return selectable; }
	bool isVisable() { return backGround->color.a != 0; }
	bool beingHovered();

	std::string getText() { return inputText->getText(); }

	void setVisable(bool value);
private:
	bool selected;
	bool selectable;
	bool pipeVisable;

	float pipeDelay;
	float pipeDelayCounter;

	HudElement* backGround;
	HudText* inputText;
};
