#include <project/mapeditorhelp.h>

MapEditorHelp::MapEditorHelp() : Scene()
{
	infoText = new HudText();
	infoText->setMultiLine(true);
	addHudElement(infoText);
	infoText->loadFont("assets/arial.ttf");
	infoText->setText("Press 'o' To open an already made map\nPress 'n' to safe the file.\nPress 'c' to duplicate the selected object.\n\n\nPress 'escape' to go back.\nPress 'p' to enter ColliderMode.\nPress 'e' to start drawing an edge collider.");
}


MapEditorHelp::~MapEditorHelp()
{
	removeHudElement(infoText);
	delete infoText;
}

void MapEditorHelp::update(float deltaTime)
{
	if (input()->getKeyUp(SDLK_ESCAPE))
	{
		SceneManager::loadScene("mapedit");
	}
	infoText->setPosition(Vector2(infoText->getWidth()/2.0f - 300, infoText->getHeight() / 2.0f - 300));
}
