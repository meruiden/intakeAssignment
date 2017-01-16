#pragma once
#include <engine/entity.h>
#include <project/questlog.h>
class QuestNpc : public Entity
{
public:
	QuestNpc(QuestLog* questLog);
	virtual ~QuestNpc();
	void setQuest(std::string questName);
private:
	QuestLog* questLog;
};

