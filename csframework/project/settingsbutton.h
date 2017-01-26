#pragma once

#include<string>

#include <engine/vector2.h>
#include <engine/hudelement.h>
#include <engine/hudtext.h>

class SettingsButton : public HudElement
{
public:
	SettingsButton(Vector2 res);
	SettingsButton(std::string text);
	virtual ~SettingsButton();

	void setText(std::string newText);
	void refitText();

	Vector2 getResolution() { return resolution; }
	std::string getText() { return infoText->getText(); }
private:
	void setUpText(std::string text);
	Vector2 resolution;
	HudText* infoText;
};

