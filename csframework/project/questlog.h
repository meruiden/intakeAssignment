#pragma once

#include <string>
#include <map>

#include <engine/hudelement.h>
#include <engine/hudtext.h>
#include <engine/mesh.h>
#include <engine/texture.h>
#include <engine/sprite.h>

class QuestLog : public HudElement
{
public:
	QuestLog();
	virtual ~QuestLog();
	void addQuest(std::string questName, std::string questInfo);
	void removeQuest(std::string questName);

private:
	void reformatText();
	std::map<std::string, std::string> quests;

	HudText* infoText;
	Mesh* surrounding;

	int padding;
};

