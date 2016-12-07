#pragma once

#include <vector>

#include <engine/scene.h>
#include <project/settingsbutton.h>
#include <engine/hudtext.h>
#include <engine/camera.h>
#include <engine/scenemanager.h>

class SettingScene : public Scene
{
public:
	SettingScene();
	virtual ~SettingScene();
	virtual void update(float deltatime);

private:
	std::vector<SettingsButton*> buttons;
	std::vector<HudText*> buttonTexts;
	HudElement* toggleFullScreenButton;
	HudElement* backButton;
	HudText* toggleFullScreenButtonText;
};

