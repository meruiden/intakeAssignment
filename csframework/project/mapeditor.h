#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <fstream>

#include <engine/scene.h>
#include <engine/hudelement.h>
#include <engine/entity.h>
#include <engine/scenemanager.h>

class MapEditor : public Scene
{
public:
	MapEditor();
	virtual ~MapEditor();
	virtual void update(float deltaTime);
	
	static void loadMap(std::vector<Entity*> &entities);
private:
	int spritesHudSize;

	float campSpeed;

	void loadAvailableSprites();
	void fitSprites();
	void saveMapFile();
	void snap(bool smartSnap);

	static std::vector<std::string> splitString(std::string str, std::string splitter);
	std::vector<HudElement*> availableSprites;
	std::vector<Entity*> mapObjects;
	std::vector<Entity*> edgeColliders;

	HudElement* draggingImage;
	
	bool spritesChanged;
	
};

