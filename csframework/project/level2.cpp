#include <project/level2.h>

Level2::Level2() : BaseLevel()
{

}


Level2::~Level2()
{
}

void Level2::update(float deltaTime)
{
	BaseLevel::update(deltaTime);
	if (npcs.size() == 2)
	{
		if (questLog->hasQuest(npcs[0]->getQuest()) && getEntitiesByName("zombie").size() == 0)
		{
			questLog->removeQuest(npcs[0]->getQuest());
		}
		if (water != NULL)
		{
			if ( (std::abs(player->getPosition().x - water->getPosition().x) * 2 < (101 + 69)) &&
				(std::abs(player->getPosition().y - water->getPosition().y) * 2 < (player->getHeight() + water->getHeight())))
			{

				removeEntity(water); // don't need to delete the water because baselevel deletes it.
				water = NULL;
				hasWater = true;
			}
		}
		if(hasWater && questLog->hasQuest(npcs[1]->getQuest()))
		{
			questLog->removeQuest(npcs[1]->getQuest());
		}
		if (hasWater && doneFadingIn())
		{
			stopMusic();
			SceneManager::loadScene("menu");
		}
		else
		{
			if (hasWater && getEntitiesByName("zombie").size() == 0 && !isFading())
			{
				fadeIn();
			}
		}
	}
}

void Level2::onLoad()
{
	clearMap();
	loadMap("level2");
	BaseLevel::onLoad();
	reset();
}

void Level2::reset()
{
	npcs.clear();
	hasWater = false;
	water = getEntityByName("water");
	std::vector<Entity*> foundNpcs = getEntitiesByName("npc");
    for(Entity* npc : foundNpcs)
	{
		npcs.push_back((QuestNpc*)npc);
	}
	if (npcs.size() == 2)
	{

		if (npcs[0]->getPosition().x > npcs[1]->getPosition().x)
		{
			QuestNpc* tmp = npcs[0];
			npcs[0] = npcs[1];
			npcs[1] = tmp;
		}
		npcs[0]->setQuest("Supplies needed!");
		npcs[1]->setQuest("Thirsty!");
	}
}
