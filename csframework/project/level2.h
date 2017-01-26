#pragma once

#include <project/baselevel.h>

class Level2 : public BaseLevel
{
public:
	Level2();
	virtual ~Level2();
	virtual void update(float deltaTime);
	virtual void onLoad();
private:
	QuestNpc* questNpc;
	Entity* water;
	bool hasWater;
	void reset();
	std::vector<QuestNpc*> npcs;
};

