#include <project/mainscene.h>

MainScene::MainScene()
{

	counter = 0.0f;
	
	geluidje = new Sound("assets/soundtest.wav");

	floor = new  Entity();
	floor->addSprite("assets/square.png"); 
	floor->setScale(Vector2(25, 1));
	floor->setPosition(Vector2(0, 330));

	textje = new TextMesh();
	textje->loadFont("assets/arial.ttf");
	textje->setText("hoi daar");
	addEntity(textje);
	textje->setPosition(Vector2(100, 100));
	textje->getPhysicsBody()->setPhysicsActive(true);

	addEntity(floor);
	floor->getPhysicsBody()->setPhysicsActive(true);
	floor->getPhysicsBody()->setPhysicsMode(PhysicsBody::STATIC);

	counterText = new HudText();
	counterText->loadFont("assets/arial.ttf");
	addHudElement(counterText);
	counterText->setAnchorPoint(HudElement::ANCHOR_TOP_LEFT);
	
	parent = new Entity();
	parent->addSprite("assets/square.png");
	
	child = new Entity();
	child->addSprite("assets/square.png");
	addEntity(parent);
	parent->getPhysicsBody()->setPhysicsActive(true);
	parent->getPhysicsBody()->setPhysicsMode(PhysicsBody::STATIC);
	parent->setLayer(2);
	textje->getPhysicsBody()->setDrawColliders(true);
	//parent->addChild(child);
	child->setPosition(Vector2(100, 0));

}

void MainScene::update(float deltaTime) 
{
	
	Vector2 mousePos = Input::getInstance()->getMousePosition();
	mousePos = getCamera()->screenToWorldSpace(mousePos);
	parent->setPosition(mousePos);
	//parent->setRotation(parent->getRotation() + deltaTime * 40);

	counter += deltaTime;
	std::stringstream ss;
	ss << "Counter: ";
	ss << (int)counter;
	counterText->setText(ss.str());
	counterText->setPosition(Vector2(10, 20) + Vector2(counterText->getWidth()/2, 0));
	textje->setText(ss.str());

	if (input()->getKeyDown(SDLK_SPACE))
	{
		geluidje->play();
	}

	if (input()->getKey(SDLK_UP))
	{
		getCamera()->setPosition(getCamera()->getPosition() + Vector2(0, -100) * deltaTime);
	}

	if (input()->getKey(SDLK_DOWN))
	{
		getCamera()->setPosition(getCamera()->getPosition() + Vector2(0, 100) * deltaTime);
	}

	if (input()->getKey(SDLK_LEFT))
	{
		getCamera()->setPosition(getCamera()->getPosition() + Vector2(-100, 0) * deltaTime);
	}

	if (input()->getKey(SDLK_RIGHT))
	{
		getCamera()->setPosition(getCamera()->getPosition() + Vector2(100, 0) * deltaTime);
	}

	if (input()->getKeyDown(SDLK_k))
	{
		if (cubes.size() >= 1)
		{
			if (cubes[0]->getPhysicsBody()->getPhysicsMode() == PhysicsBody::STATIC) 
			{
				cubes[0]->getPhysicsBody()->setPhysicsMode(PhysicsBody::DYNAMIC);
			}else if(cubes[0]->getPhysicsBody()->getPhysicsMode() == PhysicsBody::DYNAMIC)
			{
				cubes[0]->getPhysicsBody()->setPhysicsMode(PhysicsBody::STATIC);
			}
			
		}
	}

	if (input()->getMouseButtonDown(1))
	{
		
		Square* newcube = new Square(true);
		
		addEntity(newcube);
		newcube->setPosition(mousePos);
		newcube->getPhysicsBody()->setPhysicsActive(true);
		cubes.push_back(newcube);
		newcube->setName("blueBlokje");
	}

	if (input()->getMouseButtonDown(3))
	{
		Square* newcube = new Square(false);
		newcube->setPosition(mousePos);
		addEntity(newcube);
		newcube->setName("redBlokje");
		newcube->getPhysicsBody()->setPhysicsActive(true);
		cubes.push_back(newcube);
	}

	std::vector< Square* >::iterator it = cubes.begin();
	while (it != cubes.end())
	{
		if ((*it)->killme)
		{
			removeEntity(*it);
			delete (*it);
			it = cubes.erase(it);
		}
		else
		{
			++it;
		}
	}

	if (input()->getKeyDown(SDLK_ESCAPE))
	{
		SceneManager::loadScene("menu");
	}
} 

void MainScene::physicsUpdate()
{

}

MainScene::~MainScene()
{
	for (unsigned int i = 0; i < cubes.size(); i++)
	{
		delete cubes[i];
	}
	cubes.clear();

	delete floor;
	delete counterText;
	delete parent;
	delete child;
	delete geluidje;
	delete textje;
}
