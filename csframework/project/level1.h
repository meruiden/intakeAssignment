#pragma once

#include <project/baselevel.h>

class Level1 : public BaseLevel
{
public:
	Level1();
	virtual ~Level1();
	virtual void update(float deltaTime);
	virtual void onLoad();

private:
	QuestNpc* questNpc;
	Entity* hospital;
	void reset();

};

