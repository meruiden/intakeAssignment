#pragma once

#include <engine/scene.h>
#include <engine/entity.h>
#include <engine/input.h>
#include <engine/vector2.h>
#include <engine/scenemanager.h>
#include <engine/hudelement.h>

class MenuScene : public Scene
{
public:
	MenuScene();
	~MenuScene();
	virtual void update(float deltaTime);

private:
	HudElement* startButton;
	HudElement* settingsButton;
	HudElement* quitButton;
};
