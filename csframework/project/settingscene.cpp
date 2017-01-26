#include <project/settingscene.h>

SettingScene::SettingScene() : Scene()
{
	Camera::setVsync(true);
	std::vector<Vector2> resolutions = Camera::getAvailableResolutions();

	bool nextToPrev = false;
	for (unsigned int i = 0; i < resolutions.size(); i++)
	{
		SettingsButton* button = new SettingsButton(resolutions[i]);
		buttons.push_back(button);
		addHudElement(button);

		HudText* buttonText = new HudText();
		buttonText->loadFont("assets/arial.ttf");
		buttonText->setText(resolutions[i].toString());
		button->setScale(Vector2(4, 1));
		button->setPosition(Vector2(0, -350));
		if (nextToPrev) 
		{
			button->setPosition(button->getPosition() + Vector2(200, (i-1) * 30));
			nextToPrev = false;
		}
		else
		{
			button->setPosition(button->getPosition() + Vector2(0, i * 30));
			nextToPrev = true;

		}
		
		buttonText->setPosition(button->getPosition());
		addHudElement(buttonText);
		buttonText->color = BLACK;
	}

	toggleFullScreenButton = new HudElement();
	toggleFullScreenButton->setAnchorPoint(HudElement::ANCHOR_BOTTOM_LEFT);
	toggleFullScreenButton->addSprite("assets/images/square.png");
	addHudElement(toggleFullScreenButton);
	toggleFullScreenButton->setPosition(Vector2(200, -100));
	toggleFullScreenButton->setScale(Vector2(7, 1));

	toggleFullScreenButtonText = new HudText();
	toggleFullScreenButtonText->setAnchorPoint(HudElement::ANCHOR_BOTTOM_LEFT);
	toggleFullScreenButtonText->loadFont("assets/arial.ttf");
	toggleFullScreenButtonText->setText("Toggle fullscreen");
	addHudElement(toggleFullScreenButtonText);
	toggleFullScreenButtonText->color = BLACK;
	toggleFullScreenButtonText->setPosition(toggleFullScreenButton->getPosition());

	toggleVsync = new HudElement();
	toggleVsync->setAnchorPoint(HudElement::ANCHOR_BOTTOM_LEFT);
	toggleVsync->addSprite("assets/images/square.png");
	addHudElement(toggleVsync);
	toggleVsync->setPosition(Vector2(200, -200));
	toggleVsync->setScale(Vector2(7, 1));

	toggleVsyncText = new HudText();
	toggleVsyncText->setAnchorPoint(HudElement::ANCHOR_BOTTOM_LEFT);
	toggleVsyncText->loadFont("assets/arial.ttf");
	toggleVsyncText->setText("Vsync: on");
	addHudElement(toggleVsyncText);
	toggleVsyncText->color = BLACK;
	toggleVsyncText->setPosition(toggleVsync->getPosition());
}

void SettingScene::update(float deltatime)
{

	Vector2 mousePos = input()->getMousePosition();

	for (unsigned int i = 0; i < buttons.size(); i++)
	{
		if (buttons[i]->overLapsWithPoint(mousePos))
		{
			buttons[i]->color = GREY;
			if (Input::getInstance()->getMouseButtonUp(1))
			{
				Camera::setResolution(buttons[i]->getResolution());
			}
		}
		else
		{
			buttons[i]->color = WHITE;
		}
	}

	if (toggleFullScreenButton->overLapsWithPoint(mousePos))
	{
		toggleFullScreenButton->color = GREY;
		if (Input::getInstance()->getMouseButtonUp(1))
		{
			Camera::toggleFullScreen();
		}
	}
	else
	{
		toggleFullScreenButton->color = WHITE;
	}

	if (toggleVsync->overLapsWithPoint(mousePos))
	{
		toggleVsync->color = GREY;
		if (Input::getInstance()->getMouseButtonUp(1))
		{
			Camera::setVsync(!Camera::getVsync());
			if (Camera::getVsync())
			{
				toggleVsyncText->setText("Vsync: on");
			}
			else
			{
				toggleVsyncText->setText("Vsync: off");
			}
		}
	}
	else
	{
		toggleVsync->color = WHITE;
	}

	if (input()->getKeyDown(SDLK_ESCAPE))
	{
		SceneManager::loadScene("menu");
	}
}


SettingScene::~SettingScene()
{
	for (unsigned int i = 0; i < buttons.size(); i++)
	{
		delete buttons[i];
	}

	buttons.clear();

	for (unsigned int i = 0; i < buttonTexts.size(); i++)
	{
		delete buttonTexts[i];
	}

	buttonTexts.clear();

	removeHudElement(toggleFullScreenButton);
	delete toggleFullScreenButton;
	removeHudElement(toggleFullScreenButtonText);
	delete toggleFullScreenButtonText;

	removeHudElement(toggleVsync);
	delete toggleVsync;
	removeHudElement(toggleVsyncText);
	delete toggleVsyncText;
}
