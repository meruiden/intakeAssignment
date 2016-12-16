#include <project/menuscene.h>

MenuScene::MenuScene() : Scene()
{
	startButton = new HudElement();
	settingsButton = new HudElement();
	quitButton = new HudElement();

	startButton->addSprite("assets/start.jpg");
	settingsButton->addSprite("assets/settings.jpg");
	quitButton->addSprite("assets/quit.jpg");
	addHudElement(quitButton);
	addHudElement(startButton);
	addHudElement(settingsButton); 
	
	startButton->setPosition(Vector2(0, -60));
	settingsButton->setPosition(Vector2(0, 40));
	quitButton->setPosition(Vector2(0, 140));

}

void MenuScene::update(float deltaTime)
{
	Vector2 mousePos = Vector2();
	mousePos = Input::getInstance()->getMousePosition();

	if (startButton->overLapsWithPoint(mousePos))
	{
		startButton->color = GREY;
		if (Input::getInstance()->getMouseButtonUp(1))
		{
			SceneManager::loadScene("level1");
		}
	}
	else
	{
		startButton->color = WHITE;
	}


	if (settingsButton->overLapsWithPoint(mousePos))
	{
		settingsButton->color = GREY;
		if (Input::getInstance()->getMouseButtonUp(1))
		{
			SceneManager::loadScene("settings");
		}
	}
	else
	{
		settingsButton->color = WHITE;
	}


	if (quitButton->overLapsWithPoint(mousePos))
	{
		quitButton->color = GREY;
		if (Input::getInstance()->getMouseButtonUp(1))
		{
			Input::getInstance()->quit();
		}
	}
	else
	{
		quitButton->color = WHITE;
	}
	
}


MenuScene::~MenuScene()
{
	delete startButton;
	delete settingsButton;
	delete quitButton;
}
