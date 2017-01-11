#pragma once

#include <engine/scene.h>
#include <engine/hudtext.h>
#include <engine/scenemanager.h>

class MapEditorHelp : public Scene
{
public:
	MapEditorHelp();
	virtual ~MapEditorHelp();
	virtual void update(float deltaTime);

private:
	HudText* infoText;
};

