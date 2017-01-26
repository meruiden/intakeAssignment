#include <project/mapeditor.h>

MapEditor::MapEditor() : Scene()
{
	spritesHudSize = 128;
	draggingImage = NULL;

	loadAvailableSprites();
	
	spritesChanged = true;
	campSpeed = 300.0f;

	selected = NULL;

	selectedName = "No Name";
	draggingSelected = false;

	renamer = new TextInputField();
	addHudElement(renamer);
	renamer->setVisable(false);

	renamer->setAnchorPoint(HudElement::ANCHOR_BOTTOM_LEFT);
	renamer->setPosition(Vector2(200, -70));

	fileNameHandler = new TextInputField();
	addHudElement(fileNameHandler);
	fileNameHandler->setVisable(false);
	fileNameHandler->setText("LevelName");

	infoButton = new HudElement();
	infoButton->addSprite("assets/images/questionMark.png");
	addHudElement(infoButton);
	infoButton->setAnchorPoint(HudElement::ANCHOR_TOP_RIGHT);
	infoButton->setPosition(Vector2(-40, 40));
	infoButton->setScale(Vector2(0.5f, 0.5f));

	colliderMode = false;
	drawingCollider = -1;
	selectedCollider = -1;
	lockedVert = -1;

	curModeText = new HudText();
	addHudElement(curModeText);
	curModeText->loadFont("assets/arial.ttf");
	curModeText->setAnchorPoint(HudElement::ANCHOR_TOP_CENTER);
	curModeText->setText("Object Mode");
	curModeText->setPosition(Vector2(0, 30));

	hoveringVertIndicator = new Entity();
	Mesh* mesh = new Mesh();
	Texture* texture = new Texture();
	Sprite* sprite = new Sprite();
	sprite->setDynamics(mesh, texture);
	hoveringVertIndicator->addSprite(sprite);
	hoveringVertIndicator->setLayer(2);
	mesh->setDrawMode(Mesh::drawModeSettings::polygons);
	std::vector<Vector2> indicatorVerts;
	float radius = 10.0f;
	int segments = 16;
	for (unsigned int i = 0; i < segments; i++)
	{
		float angle = 2.0f * PI * float(i) / float(segments);
		indicatorVerts.push_back(Vector2(cosf(angle)*radius, sinf(angle)*radius));
	}
	mesh->setFromVertices(indicatorVerts);
	addEntity(hoveringVertIndicator);
	hoveringVertIndicator->color.a = 0;

	savingFile = false;
}


MapEditor::~MapEditor()
{
	removeEntity(hoveringVertIndicator);
	delete hoveringVertIndicator;

	removeHudElement(fileNameHandler);
	delete fileNameHandler;
	removeHudElement(renamer);
	delete renamer;

	if (draggingImage != NULL)
	{
		removeHudElement(draggingImage);
		delete draggingImage;
	}
	for (int i = 0; i < availableSprites.size(); i++)
	{
		removeHudElement(availableSprites[i]);
		delete availableSprites[i];
	}
	availableSprites.clear();


	for (int i = 0; i < mapObjects.size(); i++)
	{
		removeEntity(mapObjects[i]);
		delete mapObjects[i];
	}
	mapObjects.clear();

	for (int i = 0; i < edgeColliders.size(); i++)
	{
		removeEntity(edgeColliders[i]);
		delete edgeColliders[i];
	}
	edgeColliders.clear();

	for (int i = 0; i < edgeColliderVerts.size(); i++)
	{
		edgeColliderVerts[i].clear();
	}
	edgeColliderVerts.clear();
}

void MapEditor::update(float deltaTime)
{
	if (!fileNameHandler->isVisable() && !renamer->isSelected())
	{
		if (input()->getKey(SDLK_d))
		{
			getCamera()->setPosition(getCamera()->getPosition() + Vector2(campSpeed, 0) * deltaTime);
		}

		if (input()->getKey(SDLK_a))
		{
			getCamera()->setPosition(getCamera()->getPosition() - Vector2(campSpeed, 0) * deltaTime);
		}

		if (input()->getKey(SDLK_w))
		{
			getCamera()->setPosition(getCamera()->getPosition() - Vector2(0, campSpeed) * deltaTime);
		}

		if (input()->getKey(SDLK_s))
		{
			getCamera()->setPosition(getCamera()->getPosition() + Vector2(0, campSpeed) * deltaTime);
		}

		if (input()->getKeyUp(SDLK_o) && !renamer->isVisable() && !fileNameHandler->isVisable())
		{
			fileNameHandler->setVisable(true);
			savingFile = false;
		}

		if (input()->getKeyUp(SDLK_n) && !renamer->isVisable() && !fileNameHandler->isVisable())
		{
			fileNameHandler->setVisable(true);
			savingFile = true;
		}

		if (input()->getKeyUp(SDLK_p))
		{
			switchColliderMode(!colliderMode);
		}

		if (input()->getKeyUp(SDLK_ESCAPE))
		{
			SceneManager::loadScene("menu");
		}

		if (infoButton->overLapsWithPoint(input()->getMousePosition()))
		{
			if (input()->getMouseButtonUp(1))
			{
				SceneManager::loadScene("mapEditHelp");
				return;
			}
			infoButton->color = GREY;
		}
		else
		{
			infoButton->color = WHITE;
		}
	}

	if (fileNameHandler->isVisable())
	{
		if (input()->getKeyDown(SDLK_RETURN))
		{
			fileNameHandler->setVisable(false);
			if (savingFile)
			{
				saveMapFile(fileNameHandler->getText());
				savingFile = false;
			}
			else
			{
				openMapFile(fileNameHandler->getText());
			}
			
		}

		if (input()->getKeyDown(SDLK_ESCAPE))
		{
			fileNameHandler->setVisable(false);
			savingFile = false;
		}
	}
	if (colliderMode)
	{
		updateColliderMode();
	}
	else
	{
		updateObjectMode();
	}
	
}

void MapEditor::updateObjectMode()
{
	if (selected == NULL)
	{
		if (renamer->isVisable())
		{
			renamer->setVisable(false);
		}
	}
	else
	{

		if (selected->getSprite()->getFileName() == "assets/images/player_icon.png")
		{
			if (!renamer->isVisable())
			{
				renamer->setVisable(true);
				renamer->setSelectAble(false);
			}

		}
		else
		{
			if (!renamer->isVisable())
			{
				renamer->setVisable(true);
			}
		}
		if (input()->getKeyUp(SDLK_RETURN))
		{
			renamer->setSelected(false);
			selected->setName(renamer->getText());
		}

		if (input()->getKeyUp(SDLK_c) && !renamer->isSelected())
		{
			draggingImage = new HudElement();
			addHudElement(draggingImage);
			draggingImage->addSprite(selected->getSprite()->getFileName());
			selectedName = selected->getName();

			selected->color.a = 255;
			selected = NULL;
		}
	}

	if (spritesChanged)
	{
		spritesChanged = false;
		fitSprites();
	}

	if (input()->getMouseButtonUp(3))
	{
		if (draggingImage == NULL)
		{
			std::vector< Entity* >::iterator it = mapObjects.begin();
			while (it != mapObjects.end())
			{
				if ((*it)->overLapsWithPoint(getCamera()->screenToWorldSpace(input()->getMousePosition())))
				{
					removeEntity((*it));
					delete (*it);
					it = mapObjects.erase(it);
					selected = NULL;
					selectedName = "No Name";
				}
				else
				{
					++it;
				}
			}
		}
		else
		{
			removeHudElement(draggingImage);
			delete draggingImage;
			draggingImage = NULL;
		}
	}

	if (input()->getMouseButtonUp(1) && !(renamer->beingHovered() && selected != NULL))
	{
		if (draggingImage == NULL)
		{
			if (selected == NULL)
			{
				for (int i = mapObjects.size()-1; i >= 0; i--)
				{
					if (mapObjects[i]->overLapsWithPoint(getCamera()->screenToWorldSpace(input()->getMousePosition())))
					{
						selected = mapObjects[i];
						selected->color.a = 155;
						renamer->setText(selected->getName());
						break;
					}

				}
			}
			else
			{
				for (int i = 0; i < mapObjects.size(); i++)
				{
					if (mapObjects[i]->overLapsWithPoint(getCamera()->screenToWorldSpace(input()->getMousePosition())))
					{
						if (selected == mapObjects[i])
						{
							draggingImage = new HudElement();
							addHudElement(draggingImage);
							draggingImage->addSprite(selected->getSprite()->getFileName());
							selectedName = selected->getName();
							draggingSelected = true;
							std::vector< Entity* >::iterator it = mapObjects.begin();
							while (it != mapObjects.end())
							{
								if ((*it) == selected)
								{
									removeEntity((*it));
									delete (*it);
									it = mapObjects.erase(it);

								}
								else
								{
									++it;
								}
							}
						}
					}
				}
				selected->color.a = 255;
				selected = NULL;
			}
		}
		else
		{
			bool isPlayer = draggingImage->getSprite()->getFileName() == "assets/images/player_icon.png";
			if (isPlayer)
			{
				std::vector< Entity* >::iterator it = mapObjects.begin();
				while (it != mapObjects.end())
				{
					if ((*it)->getSprite() != NULL && (*it)->getSprite()->getFileName() == "assets/images/player_icon.png")
					{
						removeEntity((*it));
						delete (*it);
						it = mapObjects.erase(it);
					}
					else
					{
						++it;
					}
				}
			}

			Entity* entity = new Entity();
			mapObjects.push_back(entity);
			entity->addSprite(draggingImage->getSprite()->getFileName());
			addEntity(entity);
			entity->setPosition(getCamera()->screenToWorldSpace(draggingImage->getPosition()));
			if (entity->getSprite()->getFileName() == "assets/images/crate/crate_full.png")
			{
				entity->setScale(Vector2(0.5f, 0.5f));
			}
			if (isPlayer)
			{
				selectedName = "Player";
			}
			entity->setName(selectedName);
			if (draggingSelected)
			{
				selectedName = "No Name";
				removeHudElement(draggingImage);
				delete draggingImage;
				draggingImage = NULL;
				draggingSelected = false;
			}
		}
	}

	if (input()->getMouseButtonUp(1))
	{
		for (int i = 0; i < availableSprites.size(); i++)
		{
			if (availableSprites[i]->overLapsWithPoint(input()->getMousePosition()))
			{
				if (draggingImage != NULL)
				{
					removeHudElement(draggingImage);
					delete draggingImage;
				}
				draggingImage = new HudElement();
				addHudElement(draggingImage);
				draggingImage->addSprite(availableSprites[i]->getSprite()->getFileName());
				selectedName = "No Name";
				break;
			}
		}
	}
	if (draggingImage != NULL)
	{
		draggingImage->setPosition(input()->getMousePosition());
		if (draggingImage->getSprite()->getFileName() == "assets/images/crate/crate_full.png")
		{
			draggingImage->setScale(Vector2(0.5f, 0.5f));
		}
	}

	bool useSmart = false;
	if (input()->getKey(SDLK_LSHIFT))
	{
		useSmart = input()->getKey(SDLK_LCTRL);
		snap(useSmart);
	}


	if (input()->scrollUp())
	{
		scrollOffset.y += 20;
	}
	if (input()->scrollDown())
	{
		scrollOffset.y -= 20;
	}
	for (int i = 0; i < availableSprites.size(); i++)
	{
		availableSprites[i]->setPosition(Vector2(spritesHudSize / 2 + 10, spritesHudSize / 2 + 10 + i* (spritesHudSize + 10)) + scrollOffset);
	}
}

void MapEditor::loadMap(std::vector<Entity*>& entities, std::string filePath)
{
	filePath = "assets/maps/" + filePath + ".map";
	std::ifstream mapFile;
	mapFile.open(filePath);
	std::string output;
	if (mapFile.is_open()) {
		for (std::string line; std::getline(mapFile, line); )
		{
			output += line + "\n";
		}
	}
	mapFile.close();

	std::vector<std::string> lines = csframework::splitString(output, "\n");

	for (int i = 0; i < lines.size(); i++)
	{
		if (lines[i] == "[EntityList]")
		{
			i++;
			while (lines[i] != "[/EntityList]" && i < lines.size())
			{
				Vector2 position = Vector2();
				Vector2 scale = Vector2();
				float rotation = 0.0f;
				std::string imagePath = "";
				std::string name = "";

				std::vector<std::string> data = csframework::splitString(lines[i], ",");

				if (data.size() == 7)
				{
					name = data[0];
					position = Vector2(std::stof(data[1]), std::stof(data[2]));
					scale = Vector2(std::stof(data[3]), std::stof(data[4]));
					rotation = std::stof(data[5]);
					imagePath = data[6];
				}

				Entity* entity = new Entity();
				if (imagePath != "")
				{
					entity->addSprite(imagePath);
				}

				if (name != "")
				{
					entity->setName(name);
				}
				entity->setPosition(position);
				entity->setScale(scale);
				entity->setRotation(rotation);
				entities.push_back(entity);
				i++;
			}
		}

		if (lines[i] == "[EdgeColliderList]")
		{
			i++;
			while (lines[i] != "[/EdgeColliderList]" && i < lines.size())
			{
				Vector2 position = Vector2();
				Vector2 scale = Vector2();
				float rotation = 0.0f;
				std::string name = "";
				std::string colliderVertsString = "";
				std::vector<std::string> data = csframework::splitString(lines[i], ",");

				if (data.size() == 7)
				{
					name = data[0];
					position = Vector2(std::stof(data[1]), std::stof(data[2]));
					scale = Vector2(std::stof(data[3]), std::stof(data[4]));
					rotation = std::stof(data[5]);
					colliderVertsString = data[6];
				}

				Entity* entity = new Entity();
				std::vector<Vector2> colliderVerts;
				if (colliderVertsString != "")
				{
					std::vector<std::string> colliderVectors = csframework::splitString(colliderVertsString, "/");
					for (int v = 0; v < colliderVectors.size(); v++)
					{

						std::vector<std::string> vectorData = csframework::splitString(colliderVectors[v], "|");
						if (vectorData.size() == 2)
						{
							Vector2 curVec = Vector2(std::stof(vectorData[0]), std::stof(vectorData[1]));
							colliderVerts.push_back(curVec);
						}
					}
				}
				entity->getPhysicsBody()->setPhysicsActive(true);
				entity->getPhysicsBody()->setPhysicsMode(PhysicsBody::STATIC);
				entity->getPhysicsBody()->setEdgeCollider(colliderVerts);
				if (name != "")
				{
					entity->setName(name);
				}
				entity->setPosition(position);
				entity->setScale(scale);
				entity->setRotation(rotation);
				entities.push_back(entity);
				i++;
			}
		}
	}
}

void MapEditor::updateColliderMode()
{
	if (input()->getKeyUp(SDLK_e) && selectedCollider == -1)
	{
		drawingCollider = edgeColliders.size();
		Entity* edgeCollider = new Entity();
		Mesh* colliderMesh = new Mesh();
		Texture* texture = new Texture();
		Sprite* sprite = new Sprite();
		colliderMesh->setDrawMode(Mesh::drawModeSettings::lines);
		sprite->setDynamics(colliderMesh, texture);
		edgeCollider->addSprite(sprite);
		std::vector<Vector2> firstVerts;
		firstVerts.push_back(Vector2());
		firstVerts.push_back(Vector2());
		edgeColliderVerts.push_back(firstVerts);
		edgeColliders.push_back(edgeCollider);
		edgeCollider->setPosition(getCamera()->screenToWorldSpace(input()->getMousePosition()));
		addEntity(edgeCollider);
		edgeCollider->setLayer(2);
	}

	if (drawingCollider != -1)
	{
		Vector2 vertPos = getCamera()->screenToWorldSpace(input()->getMousePosition()) - edgeColliders[drawingCollider]->getPosition();
		edgeColliderVerts[drawingCollider][edgeColliderVerts[drawingCollider].size()-1] = vertPos;

		edgeColliders[drawingCollider]->getSprite()->getDynamicMesh()->setFromVertices(edgeColliderVerts[drawingCollider]);

		if (input()->getMouseButtonUp(1))
		{
			edgeColliderVerts[drawingCollider].push_back(vertPos);
			edgeColliderVerts[drawingCollider].push_back(vertPos);
		}

		if (input()->getMouseButtonUp(3))
		{
			drawingCollider = -1;
		}
	}

	if (selectedCollider == -1)
	{
		for (int i = 0; i < edgeColliders.size(); i++)
		{
			std::vector<Vector2> curEdgeVerts = edgeColliderVerts[i];
			bool alreadyHovering = false;
			if (hoversOverLine(edgeColliders[i]->getPosition(), curEdgeVerts, 5.0f) && !alreadyHovering && drawingCollider == -1)
			{
				edgeColliders[i]->getSprite()->getDynamicMesh()->setLineThickness(5.0f);
				alreadyHovering = true;
				if (input()->getMouseButtonUp(1))
				{
					selectedCollider = i;
					edgeColliders[i]->color = GREY;
				}
			}
			else
			{
				edgeColliders[i]->getSprite()->getDynamicMesh()->setLineThickness(3.0f);
			}
		}
	}
	else
	{	
		Vector2 mousePos = getCamera()->screenToWorldSpace(input()->getMousePosition());
		std::vector<Vector2> verts = edgeColliderVerts[selectedCollider];

		if (lockedVert != -1)
		{
			Vector2 selectedVertPos = edgeColliders[selectedCollider]->getPosition() + verts[lockedVert];
			

			if (input()->getMouseButton(1))
			{
				verts[lockedVert] = mousePos + curSelectedVertOffset - edgeColliders[selectedCollider]->getPosition();
				if (lockedVert != 0 && lockedVert != verts.size() - 1)
				{
					verts[lockedVert + 1] = mousePos + curSelectedVertOffset - edgeColliders[selectedCollider]->getPosition();
				}
				edgeColliderVerts[selectedCollider] = verts;
				edgeColliders[selectedCollider]->getSprite()->getDynamicMesh()->setFromVertices(verts);
			}

			if (input()->getMouseButtonUp(1))
			{
				lockedVert = -1;
			}
			
		}
	
		if (verts.size() > 0)
		{
			if (hoversOverLine(edgeColliders[selectedCollider]->getPosition(), verts, 20.0f))
			{
				int closest = 0;
				bool inRange = false;
				for (int i = 0; i < verts.size(); i++)
				{
					Vector2 curVertPos = edgeColliders[selectedCollider]->getPosition() + verts[i];
					Vector2 closestVertPos = edgeColliders[selectedCollider]->getPosition() + verts[closest];
					
					if (Vector2(mousePos, curVertPos).magnitude() <= 15)
					{
						if (Vector2(mousePos, curVertPos).magnitude() < Vector2(mousePos, closestVertPos).magnitude())
						{
							closest = i;
						}
						inRange = true;
					}
				}

				if (inRange)
				{
					Vector2 selectedVertPos = edgeColliders[selectedCollider]->getPosition() + verts[closest];
					if (input()->getMouseButtonDown(1))
					{
						curSelectedVertOffset = mousePos - selectedVertPos;
						lockedVert = closest;
					}

					if (input()->getMouseButton(1))
					{
						verts[closest] = mousePos + curSelectedVertOffset - edgeColliders[selectedCollider]->getPosition();
						if (closest != 0 && closest != verts.size() - 1)
						{
							verts[closest+1] = mousePos + curSelectedVertOffset - edgeColliders[selectedCollider]->getPosition();
						}
						edgeColliderVerts[selectedCollider] = verts;
						edgeColliders[selectedCollider]->getSprite()->getDynamicMesh()->setFromVertices(verts);
					}
					if (lockedVert == -1)
					{
						hoveringVertIndicator->color.a = 100;
						hoveringVertIndicator->setPosition(selectedVertPos);
					}
					else
					{
						hoveringVertIndicator->color.a = 0;
					}
					

				}
				else
				{
					hoveringVertIndicator->color.a = 0;
				}
			}
			else
			{
				hoveringVertIndicator->color.a = 0;
				if (input()->getMouseButtonUp(1))
				{
					edgeColliders[selectedCollider]->color = WHITE;
					selectedCollider = -1;
				}
			}
		}
		if (input()->getMouseButtonUp(3))
		{
			removeEntity(edgeColliders[selectedCollider]);
			delete edgeColliders[selectedCollider];
			edgeColliders.erase(edgeColliders.begin() + selectedCollider);
			edgeColliderVerts.erase(edgeColliderVerts.begin() + selectedCollider);
			lockedVert = -1;
			selectedCollider = -1;
			hoveringVertIndicator->color.a = 0;
		}
	}
}

void MapEditor::switchColliderMode(bool value)
{
	colliderMode = value;

	if (value)
	{
		for (int i = 0; i < mapObjects.size(); i++)
		{
			mapObjects[i]->color.a = 100;
		}
		for (int i = 0; i < availableSprites.size(); i++)
		{
			availableSprites[i]->color.a = 0;
		}
		if (draggingImage != NULL)
		{
			removeHudElement(draggingImage);
			delete draggingImage;
			draggingImage = NULL;
		}
		if (selected != NULL)
		{
			selected = NULL;
		}
		if (renamer->isVisable())
		{
			renamer->setVisable(false);
		}

		for (int i = 0; i < edgeColliders.size(); i++)
		{
			edgeColliders[i]->color.a = 255;
			edgeColliders[i]->getSprite()->getDynamicMesh()->setLineThickness(3.0f);
		}

		curModeText->setText("Collider Mode");
	}
	else
	{
		for (int i = 0; i < mapObjects.size(); i++)
		{
			mapObjects[i]->color.a = 255;
		}
		for (int i = 0; i < availableSprites.size(); i++)
		{
			availableSprites[i]->color.a = 255;
		}
		for (int i = 0; i < edgeColliders.size(); i++)
		{
			edgeColliders[i]->color.a = 100;
		}
		curModeText->setText("Object Mode");
	}
}

bool MapEditor::hoversOverLine(Vector2 linePos, std::vector<Vector2> lineVerts, float detectionRange)
{
	int numVerts = lineVerts.size();
	if (numVerts == 0)
	{
		return false;
	}
	for (unsigned int i = 0; i < numVerts - 1; i++) {
		bool inRange = true;
		Vector2 a = lineVerts[i] + linePos;
		Vector2 b = lineVerts[i + 1] + linePos;

		Vector2 mousePos = getCamera()->screenToWorldSpace(input()->getMousePosition());
		Vector2 normal = Vector2::getNormalPoint(mousePos, a, b);
		float distance = Vector2(mousePos, normal).magnitude();

		if (a.x < b.x && normal.x < a.x) {
			inRange = false;
		}

		if (a.y < b.y && normal.y < a.y) {
			inRange = false;
		}

		if (a.x < b.x && normal.x > b.x) {
			inRange = false;
		}

		if (a.y < b.y && normal.y > b.y) {
			inRange = false;
		}

		if (b.x < a.x && normal.x < b.x) {
			inRange = false;
		}

		if (b.y < a.y && normal.y < b.y) {
			inRange = false;
		}

		if (b.x < a.x && normal.x > a.x) {
			inRange = false;
		}

		if (b.y < a.y && normal.y > a.y) {
			inRange = false;
		}

		if (distance <= detectionRange && inRange) {
			return true;
		}

	}
	return false;
}

void MapEditor::loadAvailableSprites()
{
	std::ifstream myReadFile;
	myReadFile.open("assets/.availableImages");
	std::string output;
	if (myReadFile.is_open()) {
		while (!myReadFile.eof()) {
			std::string line;
			myReadFile >> line;
			output += line + "\n";
		}
	}
	myReadFile.close();

	std::vector<std::string> imagePaths = csframework::splitString(output, "\n");
	for (int i = 0; i < imagePaths.size(); i++)
	{
		HudElement* img = new HudElement();
		img->addSprite("assets/images/" + imagePaths[i]);
		availableSprites.push_back(img);
		addHudElement(img);
		img->setAnchorPoint(HudElement::ANCHOR_TOP_LEFT);
		img->setPosition(Vector2(spritesHudSize / 2 + 10, spritesHudSize / 2 + 10 + i* (spritesHudSize + 10)));
	}
}

void MapEditor::fitSprites()
{
	for (int i = 0; i < availableSprites.size(); i++)
	{
		HudElement* img = availableSprites[i];
		float biggestSide = std::max(img->getSprite()->getSpriteSize().x, img->getSprite()->getSpriteSize().y);
		if (biggestSide == 0)
		{
			spritesChanged = true;
			return;
		}
		float scalar = (1.0f / biggestSide) * (float)spritesHudSize;
		img->setScale(Vector2(scalar, scalar));
		
	}
}

void MapEditor::saveMapFile(std::string filePath)
{
	filePath = "assets/maps/" + filePath + ".map";
	std::stringstream mapData;

	mapData << "[EntityList]\n";
	for(int i = 0; i < mapObjects.size(); i++)
	{
		mapData << mapObjects[i]->getName();
		mapData << ",";
		mapData << mapObjects[i]->getGlobalPosition().x;
		mapData << ",";
		mapData << mapObjects[i]->getGlobalPosition().y;
		mapData << ",";
		mapData << mapObjects[i]->getGlobalScale().x;
		mapData << ",";
		mapData << mapObjects[i]->getGlobalScale().y;
		mapData << ",";
		mapData << mapObjects[i]->getGlobalRotation();
		mapData << ",";
		mapData << mapObjects[i]->getSprite()->getFileName();

		mapData << "\n";
			
	}
	mapData << "[/EntityList]\n";
	mapData << "[EdgeColliderList]\n";
	for (int i = 0; i < edgeColliders.size(); i++)
	{
		mapData << edgeColliders[i]->getName();
		mapData << ",";
		mapData << edgeColliders[i]->getGlobalPosition().x;
		mapData << ",";
		mapData << edgeColliders[i]->getGlobalPosition().y;
		mapData << ",";
		mapData << edgeColliders[i]->getGlobalScale().x;
		mapData << ",";
		mapData << edgeColliders[i]->getGlobalScale().y;
		mapData << ",";
		mapData << edgeColliders[i]->getGlobalRotation();
		mapData << ",";
		std::vector<Vector2>verts = edgeColliderVerts[i];
		for (int ii = 0; ii < verts.size(); ii++)
		{
			mapData << verts[ii].x << "|" << verts[ii].y;
			if (ii + 1 < verts.size())
			{
				mapData << "/";
			}
		}

		mapData << "\n";

	}
	mapData << "[/EdgeColliderList]\n";

	

	std::ofstream myfile(filePath);
	if (myfile.is_open())
	{
		myfile << mapData.str();
		myfile.close();
	}

}

void MapEditor::snap(bool smartSnap)
{
	if (draggingImage == NULL || mapObjects.size() == 0)
	{
		return;
	}

	Entity* closest = mapObjects[0];

	Vector2 draggingWorldPos = getCamera()->screenToWorldSpace(input()->getMousePosition());

	for (int i = 0; i < mapObjects.size(); i++)
	{
		if (Vector2(mapObjects[i]->getPosition(), draggingWorldPos).magnitude() <
			Vector2(closest->getPosition(), draggingWorldPos).magnitude())

		{
			closest = mapObjects[i];
		}
	}

	Vector2 closestPos = closest->getPosition();

	bool top = false;
	bool bottom = false;
	bool left = false;
	bool right = false;

	if (draggingWorldPos.x > closestPos.x)
	{
		right = true;
	}

	if (draggingWorldPos.x < closestPos.x)
	{
		left = true;
	}

	if (draggingWorldPos.y > closestPos.y)
	{
		bottom = true;
	}

	if (draggingWorldPos.y < closestPos.y)
	{
		top = true;
	}

	
	if (std::abs(draggingWorldPos.y - closestPos.y) > std::abs(draggingWorldPos.x - closestPos.x))
	{
		left = false;
		right = false;
	}
	else 
	{
		bottom = false;
		top = false;
	}
	
	Vector2 closestPosToScreen = closestPos - getCamera()->getPosition();
	if (smartSnap)
	{
		if (left)
		{
			draggingImage->setPosition(closestPosToScreen + Vector2(-(draggingImage->getWidth()/2.0f + closest->getWidth()/2.0f) , 0));
		}

		if (right)
		{
			draggingImage->setPosition(closestPosToScreen + Vector2((draggingImage->getWidth() / 2.0f + closest->getWidth() / 2.0f), 0));
		}

		if (top)
		{
			draggingImage->setPosition(closestPosToScreen + Vector2(0, -(draggingImage->getWidth() / 2.0f + closest->getWidth() / 2.0f)));
		}

		if (bottom)
		{
			draggingImage->setPosition(closestPosToScreen + Vector2(0, (draggingImage->getWidth() / 2.0f + closest->getWidth() / 2.0f)));
		}
	}
	else 
	{
		if (right || left)
		{
			draggingImage->setPosition(Vector2(draggingImage->getPosition().x, closestPosToScreen.y));
		}

		if (top || bottom)
		{
			draggingImage->setPosition(Vector2(closestPosToScreen.x, draggingImage->getPosition().y));
		}
	}

}

void MapEditor::openMapFile(std::string filePath)
{

	for (int i = 0; i < mapObjects.size(); i++)
	{
		removeEntity(mapObjects[i]);
		delete mapObjects[i];
	}
	mapObjects.clear();

	for (int i = 0; i < edgeColliders.size(); i++)
	{
		removeEntity(edgeColliders[i]);
		delete edgeColliders[i];
	}
	edgeColliders.clear();

	for (int i = 0; i < edgeColliderVerts.size(); i++)
	{
		edgeColliderVerts[i].clear();
	}
	edgeColliderVerts.clear();

	std::vector<Entity*> loadedEntities;
	MapEditor::loadMap(loadedEntities, filePath);
	for (int i = 0; i < loadedEntities.size(); i++)
	{
		addEntity(loadedEntities[i]);
		if (loadedEntities[i]->getPhysicsBody()->getPhysicsActive())
		{
			Entity* edgeCollider = loadedEntities[i];
			std::vector<Vector2> colliderVerts = loadedEntities[i]->getPhysicsBody()->getColliderVertices();
			Mesh* colliderMesh = new Mesh();
			Texture* texture = new Texture();
			Sprite* sprite = new Sprite();
			colliderMesh->setDrawMode(Mesh::drawModeSettings::lines);
			sprite->setDynamics(colliderMesh, texture);
			edgeCollider->addSprite(sprite);
			std::vector<Vector2> fixedVerts;
			float lineCounter = 0.0f;;

			for (int v = 0; v < colliderVerts.size(); v++)
			{
				fixedVerts.push_back(colliderVerts[v]);
			}
			colliderMesh->setFromVertices(fixedVerts);
			edgeColliderVerts.push_back(fixedVerts);
			edgeColliders.push_back(edgeCollider);
			edgeCollider->setLayer(2);
		}
		else
		{
			mapObjects.push_back(loadedEntities[i]);
		}
		
	}

	switchColliderMode(colliderMode);
}



