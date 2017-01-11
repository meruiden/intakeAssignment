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
#include <engine/hudtext.h>
#include <engine/sprite.h>

#include <project/textinputfield.h>

class MapEditor : public Scene
{
public:
	MapEditor();
	virtual ~MapEditor();
	virtual void update(float deltaTime);
	
	static void loadMap(std::vector<Entity*> &entities, std::string filePath);
private:
	int spritesHudSize;

	float campSpeed;

	void loadAvailableSprites();
	void fitSprites();
	void saveMapFile();
	void snap(bool smartSnap);
	void openMapFile(std::string filePath);
	void updateObjectMode();
	void updateColliderMode();
	void switchColliderMode(bool value);
	
	bool hoversOverLine(Vector2 linePos, std::vector<Vector2> lineVerts, float detectionRange);

	static std::vector<std::string> splitString(std::string str, std::string splitter);
	std::vector<HudElement*> availableSprites;
	std::vector<Entity*> mapObjects;
	std::vector<Entity*> edgeColliders;
	std::vector<std::vector<Vector2>> edgeColliderVerts;

	Entity* selected;
	Entity* hoveringVertIndicator;

	HudElement* draggingImage;
	HudElement* infoButton;
	HudText* curModeText;

	TextInputField* renamer;
	TextInputField* fileNameHandler;
	
	bool spritesChanged;
	bool draggingSelected;

	Vector2 scrollOffset;
	Vector2 curSelectedVertOffset;

	std::string selectedName;

	bool colliderMode;
	int drawingCollider;
	int selectedCollider;
	int lockedVert;
};

