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
	sprite = new Entity();
	//sprite->addSpriteAsSpriteSheet("assets/walk.png", 130, 150, 7, 4);
	addEntity(sprite);
	//sprite->addSpriteAsSpriteSheet("assets/bird.png", 240, 314, 5, 5);
	sprite->addSpriteAsSpriteSheet("assets/explosion.png", 128, 128, 4, 4);
	sprite->getSpriteSheet()->setAnimateFrames(0, 11);
	sprite->getSpriteSheet()->play();
	sprite->getSpriteSheet()->setLoop(true);
	sprite->setPosition(Vector2(300, 0));
	sprite->setScale(Vector2(-1, 1));
	
	startButton->setPosition(Vector2(0, -60));
	settingsButton->setPosition(Vector2(0, 40));
	quitButton->setPosition(Vector2(0, 140));

}

void MenuScene::update(float deletaTime)
{

	Vector2 mousePos = Vector2();
	mousePos = Input::getInstance()->getMousePosition();

	sprite->getSpriteSheet()->animate(deletaTime, 1.0f / 20.0f);

	if (input()->getKeyUp(SDLK_SPACE)) {
		sprite->getSpriteSheet()->stop();
		sprite->getSpriteSheet()->play();
	}
	
	if(input()->getKey(SDLK_a))
	{
		sprite->setPosition(sprite->getPosition() - Vector2(200, 0) * deletaTime);
		sprite->setScale(Vector2(-1, 1));
	}

	if (input()->getKey(SDLK_d))
	{
		sprite->setPosition(sprite->getPosition() + Vector2(200, 0) * deletaTime);
		sprite->setScale( Vector2(1, 1));
	}

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
	delete sprite;
	delete startButton;
	delete settingsButton;
	delete quitButton;
}
