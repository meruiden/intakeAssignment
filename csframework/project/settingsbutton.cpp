#include <project/settingsbutton.h>

SettingsButton::SettingsButton(Vector2 res) : HudElement()
{
	this->setUpText(res.toString());
	this->resolution = res;
}

SettingsButton::SettingsButton(std::string text) : HudElement()
{
	this->setUpText(text);
}


SettingsButton::~SettingsButton()
{
	removeChild(infoText);
	delete infoText;
}

void SettingsButton::setText(std::string newText)
{
	this->infoText->setText(newText);
}

void SettingsButton::refitText()
{
	this->infoText->setScale(Vector2(1, 1) / this->getScale());
}

void SettingsButton::setUpText(std::string text)
{
	this->addSprite("assets/images/square.png");
	this->infoText = new HudText();
	this->infoText->loadFont("assets/arial.ttf");
	this->setText(text);
	this->infoText->color = BLACK;
	this->addChild(infoText);
}
