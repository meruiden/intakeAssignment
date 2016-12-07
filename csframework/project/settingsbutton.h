#pragma once

#include <engine/vector2.h>
#include <engine/hudelement.h>

class SettingsButton : public HudElement
{
public:
	SettingsButton(Vector2 res);
	virtual ~SettingsButton();

	Vector2 getResolution() { return resolution; }
private:
	Vector2 resolution;
};

