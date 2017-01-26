#pragma once
#include <engine/entity.h>
#include <engine/scene.h>
#include <project/questlog.h>
#include <project/npcchatbar.h>
class QuestNpc : public Entity
{
public:
	QuestNpc(QuestLog* questLog);
	virtual ~QuestNpc();
	virtual void update(float deltaTime);

	void startChat();
	void setQuest(std::string questName);
	void disable();

	bool disabled() { return isDisabled; }
	bool getChatComplete() { return chatComplete; }
	bool canStartChat() { return (!chatStarted && !chatComplete); }

	std::string getQuest() { return quest; }
private:
	void pushQuest();
	QuestLog* questLog;
	bool chatComplete;
	bool chatStarted;
	bool isDisabled;
	std::string quest;
	NpcChatBar* chatBar;
};

