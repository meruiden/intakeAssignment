#include <project/level1.h>

Level1::Level1() : BaseLevel()
{

}


Level1::~Level1()
{

}

void Level1::update(float deltaTime)
{
	BaseLevel::update(deltaTime);
	if (questLog->hasQuest(questNpc->getQuest()) && getEntitiesByName("zombie").size() == 0)
	{
		questLog->removeQuest(questNpc->getQuest());
	}

	if (hospital != NULL && !questLog->hasQuest(questNpc->getQuest()))
	{
		if (doneFadingIn())
		{
			stopMusic();
			SceneManager::loadScene("level2");
		}
		if (player->getPosition().x >= hospital->getPosition().x - 100 && !isFading())
		{
			fadeIn();
		}
		
	}
}

void Level1::onLoad()
{
	clearMap();
	loadMap("level1");
	BaseLevel::onLoad();
	reset();
	
}

void Level1::reset()
{
	questNpc = (QuestNpc*)getEntityByName("npc");
	if (questNpc != NULL)
	{
		questNpc->setQuest("Hospital needed!");
	}

	hospital = getEntityByName("hospital");
}

