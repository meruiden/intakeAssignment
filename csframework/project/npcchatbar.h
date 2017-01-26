#pragma once

#include <string>
#include <vector>

#include <engine/entity.h>
#include <engine/textmesh.h>
class NpcChatBar : public Entity
{
public:
	NpcChatBar();
	virtual ~NpcChatBar();

	void addMessage(std::string message);
	bool nextMessage();

private:
	TextMesh* brievingText;
	Entity* mouseIcon;
	std::vector <std::string> messages;
	void fitText();
	int curMessage;
};

