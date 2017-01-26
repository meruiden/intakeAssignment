#include <project/mapeditorhelp.h>

MapEditorHelp::MapEditorHelp() : Scene()
{
	infoText = new HudText();
	infoText->setMultiLine(true);
	addHudElement(infoText);
	infoText->setAnchorPoint(HudElement::ANCHOR_TOP_LEFT);
	infoText->setScale(Vector2(0.8f, 0.8f));
	infoText->loadFont("assets/arial.ttf");
	infoText->setText("Click on an item from the menu and click again to place it\nHold Left Shift while moving an image close to another image for relative axis snapping\nHold Left Shift + Left CTRL while moving an image close to another image for relative side snapping\nScroll up or down to scroll trough the menu\nClick on an object in the world to select it\nClick on an selected object to replace it\nSelected objects can be renamed with the text area in the top left bottom once an object is selected\nPress 'o' To open an already made map\nPress 'n' to safe the file.\nPress 'c' to duplicate the selected object.\nUse 'W' 'A' 'S' 'D' to move the camera\nPress 'escape' to go back.\n\nPress 'p' to enter ColliderMode.\nPress 'e' to start drawing an edge collider.\nClick on your left mouse button while drawing to add an corner point\nPress right button to place the last vertice\nClick on an collider to select it.\nSelected colliders can be deleted with the right mouse button\nHover over the corners of an selected collider to edit the vertices by dragging the circle that pops up.      ");
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
	infoText->setPosition(Vector2(infoText->getWidth()/2.0f + 10 , infoText->getHeight() / 2.0f + 20));
}
