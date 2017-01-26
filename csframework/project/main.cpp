#include <engine/renderer.h>
#include <engine/scenemanager.h>
#include <project/level1.h>
#include <project/level2.h>
#include <project/menuscene.h>
#include <project/settingscene.h>
#include <project/mapeditor.h>
#include <project/mapeditorhelp.h>

Renderer* renderer;
Level1* level1 = NULL;
Level2* level2 = NULL;
MenuScene* menuscene = NULL;
SettingScene* settingscene = NULL;
MapEditor* mapedit = NULL;;
MapEditorHelp* mapEditHelp = NULL;


int main(int argc, char* argv[]) {
	renderer = new Renderer();
	level1 = new Level1();
	level2 = new Level2();
	menuscene = new MenuScene();
	settingscene = new SettingScene();
	mapedit = new MapEditor();
	mapEditHelp = new MapEditorHelp();

	SceneManager::addScene("level1", level1);
	SceneManager::addScene("level2", level2);
	SceneManager::addScene("settings", settingscene);
	SceneManager::addScene("menu", menuscene);
	SceneManager::addScene("mapedit", mapedit);
	SceneManager::addScene("mapEditHelp", mapEditHelp);
	SceneManager::loadScene("menu");

	while (!renderer->mustQuit()) {
		renderer->renderScene();
	}

	delete level1;
	delete level2;
	delete menuscene;
	delete settingscene;
	delete mapedit;
	delete mapEditHelp;

	delete renderer;

	return 0;
}