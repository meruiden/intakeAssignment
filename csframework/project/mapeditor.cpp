#include <project/mapeditor.h>

MapEditor::MapEditor() : Scene()
{
	spritesHudSize = 128;
	draggingImage = NULL;

	loadAvailableSprites();
	
	spritesChanged = true;
	campSpeed = 40.0f;
}


MapEditor::~MapEditor()
{
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
	
}

void MapEditor::update(float deltaTime)
{
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

	if (input()->getMouseButtonUp(1) && draggingImage != NULL)
	{
		Entity* entity = new Entity();
		mapObjects.push_back(entity);
		entity->addSprite(draggingImage->getSprite()->getFileName());
		addEntity(entity);
		entity->setPosition(getCamera()->screenToWorldSpace(draggingImage->getGlobalPosition()));

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
				break;
			}
		}
	}
	if (draggingImage != NULL)
	{
		draggingImage->setPosition(input()->getMousePosition());
	}

	bool useSmart = false;
	if (input()->getKey(SDLK_LSHIFT))
	{
		useSmart = input()->getKey(SDLK_LCTRL);
		snap(useSmart);
	}

	if (input()->getKeyUp(SDLK_n))
	{
		saveMapFile();
	}

	
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

	if (input()->getKeyUp(SDLK_ESCAPE))
	{
		SceneManager::loadScene("menu");
	}

	
}

void MapEditor::loadMap(std::vector<Entity*>& entities)
{
	std::ifstream mapFile;
	mapFile.open("assets/level1.map");
	std::string output;
	if (mapFile.is_open()) {
		for (std::string line; std::getline(mapFile, line); )
		{
			output += line + "\n";
		}
	}
	mapFile.close();

	std::vector<std::string> lines = MapEditor::splitString(output, "\n");

	for (int i = 0; i < lines.size(); i++)
	{
		
		if (lines[i] == "[EntityList]")
		{
			while (lines[i] != "[/EntityList]" && i < lines.size())
			{
				Vector2 position = Vector2();
				Vector2 scale = Vector2();
				float rotation = 0.0f;
				std::string filePath = "";
				std::string name = "";

				std::vector<std::string> data = MapEditor::splitString(lines[i], ",");

				if (data.size() == 7)
				{
					std::string::size_type sz;

					name = data[0];
					position = Vector2(std::stof(data[1], &sz), std::stof(data[2], &sz));
					scale = Vector2(std::stof(data[3], &sz), std::stof(data[4], &sz));
					rotation = std::stof(data[5], &sz);
					filePath = data[6];
				}
				
				Entity* entity = new Entity();
				if (filePath != "")
				{
					entity->addSprite(filePath);
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
	}
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

	std::vector<std::string> imagePaths = MapEditor::splitString(output, "\n");
	for (int i = 0; i < imagePaths.size(); i++)
	{
		HudElement* img = new HudElement();
		img->addSprite("assets/" + imagePaths[i]);
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

void MapEditor::saveMapFile()
{
	std::stringstream mapData;

	if (mapObjects.size() > 0)
	{
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
			mapData << edgeColliders[i]->getSprite()->getFileName();
			mapData << ",";
			std::vector<Vector2>verts = edgeColliders[i]->getPhysicsBody()->getColliderVertices();
			for (int ii = 0; ii < verts.size(); i++)
			{
				mapData << verts[i].x << "|" << verts[i].y << "/";
			}

			mapData << "\n";

		}
		mapData << "[/EdgeColliderList]\n";

	}

	std::ofstream myfile("assets/level1.map");
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

std::vector<std::string> MapEditor::splitString(std::string str, std::string splitter)
{
	std::vector<std::string> tokens;
	std::string::size_type lastPos = str.find_first_not_of(splitter, 0);
	std::string::size_type pos = str.find_first_of(splitter, lastPos);

	while (std::string::npos != pos || std::string::npos != lastPos)
	{
		tokens.push_back(str.substr(lastPos, pos - lastPos));
		lastPos = str.find_first_not_of(splitter, pos);
		pos = str.find_first_of(splitter, lastPos);
	}

	return tokens;
}