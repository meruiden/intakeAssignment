#include <project/npcchatbar.h>

NpcChatBar::NpcChatBar() : Entity()
{
	this->addSprite("assets/images/chatbar.png");
	this->curMessage = 0;
	this->brievingText = new TextMesh();
	this->brievingText->loadFont("assets/arial.ttf");
	this->brievingText->color = BLACK;
	this->brievingText->setMultiLine(true);
	this->addChild(brievingText);
	this->brievingText->setScale(Vector2(0.7f, 0.7f));
	this->mouseIcon = new Entity();
	this->mouseIcon->addSprite("assets/images/mouse_icon.png");
	this->addChild(mouseIcon);
	this->mouseIcon->setPosition(Vector2(80, 38));
}


NpcChatBar::~NpcChatBar()
{
	this->removeChild(brievingText);
	delete brievingText;

	this->removeChild(mouseIcon);
	delete mouseIcon;
}

void NpcChatBar::addMessage(std::string message)
{
	messages.push_back(message);
	if (messages.size() == 1)
	{
		brievingText->setText(message);
		fitText();
	}
}

bool NpcChatBar::nextMessage()
{
	if (messages.size()-1 > curMessage)
	{
		curMessage++;
		brievingText->setText(messages[curMessage]);
		fitText();
		return true;
	}
	return false;
}

void NpcChatBar::fitText()
{
	brievingText->setPosition(Vector2(
		(brievingText->getSprite()->getDynamicMesh()->getWidth() * brievingText->getScale().x) / 2.0f - 90,
		(brievingText->getSprite()->getDynamicMesh()->getHeight() * brievingText->getScale().y) / 2.0f - 50)
	);
}
