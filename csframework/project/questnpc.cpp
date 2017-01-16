#include <project/questnpc.h>

QuestNpc::QuestNpc(QuestLog* questLog)
{
	this->addSprite("assets/npc.png");
	
}


QuestNpc::~QuestNpc()
{

}

void QuestNpc::setQuest(std::string questName)
{
	if (questName == "Hospital needed!") {
		questLog->addQuest("Hospital needed!", "Clear the path to\nthe Hostpital so Jason\ncan get the medical suplies.");
	}
}
