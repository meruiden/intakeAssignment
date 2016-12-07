#pragma once

#include <engine/scene.h>
#include <engine/entity.h>
#include <engine/input.h>
#include <engine/sound.h>
#include <engine/textmesh.h>
#include <engine/physicsbody.h>
#include <engine/camera.h>
#include <engine/hudtext.h>
#include <project/square.h>
#include <engine/textmesh.h>
#include <engine/scenemanager.h>
#include <sstream>

class MainScene : public Scene
{
public:
	MainScene();
	virtual ~MainScene();
	virtual void update(float deltaTime);
	virtual void physicsUpdate();

private:
	
	Entity* floor;
	Entity* parent;
	Entity* child;
	float counter;
	Sound* geluidje;
	TextMesh* textje;


	HudText* counterText;
	std::vector<Square*> cubes;
};

