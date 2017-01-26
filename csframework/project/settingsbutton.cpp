#include <project/settingsbutton.h>

SettingsButton::SettingsButton(Vector2 res) : HudElement()
{
	this->resolution = res;
	this->addSprite("assets/images/square.png");
}


SettingsButton::~SettingsButton()
{

}
