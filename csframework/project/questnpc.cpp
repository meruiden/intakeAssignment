#include <project/questnpc.h>

QuestNpc::QuestNpc(QuestLog* questLog) : Entity()
{
	this->addSprite("assets/images/npc.png");
	this->setLayer(2);
	this->quest = "";
	this->chatComplete = false;
	this->chatStarted = false;
	this->questLog = questLog;
	this->chatBar = new NpcChatBar();
	this->chatBar->setPosition(Vector2(120, -100));
	this->chatBar->addMessage("Hey you!");
	this->chatBar->addMessage("Can you please\nhelp me with\nsomething?");
	this->isDisabled = false;
	this->getPhysicsBody()->setPhysicsActive(true);
	this->getPhysicsBody()->setPhysicsMode(PhysicsBody::STATIC);
}

QuestNpc::~QuestNpc()
{
	this->removeChild(chatBar);
	delete chatBar;
}

void QuestNpc::update(float deltaTime)
{
	if (chatStarted && !chatComplete) 
	{
		if (input()->getMouseButtonUp(1))
		{
			if (!chatBar->nextMessage())
			{
				chatComplete = true;
				removeChild(chatBar);
				pushQuest();
			}
		}
	}
}

void QuestNpc::startChat()
{ 
	chatStarted = true;
	this->addChild(chatBar);

}

void QuestNpc::pushQuest()
{
	
	if (this->quest == "Hospital needed!")
	{
		this->questLog->addQuest("Hospital needed!", "Clear the path to\nthe Hospital so Jason\ncan get the medical suplies.");
	}else if (this->quest == "Supplies needed!")
	{
		this->questLog->addQuest("Supplies needed!", "Clear the path to\nthe Groceries store\nso Steve can get food and water.");
	}else if (this->quest == "Thirsty!")
	{
		this->questLog->addQuest("Thirsty!", "Find a bottle of water\nbefore Steves father dies\nfrom dehydration.");
	}
}

void QuestNpc::setQuest(std::string questName)
{
	this->quest = questName;
}

void QuestNpc::disable()
{
	this->isDisabled = true;
	this->getPhysicsBody()->setPhysicsActive(false);
}
