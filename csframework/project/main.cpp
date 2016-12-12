#include <engine/renderer.h>
#include <engine/scenemanager.h>
#include <project/level1.h>
#include <project/menuscene.h>
#include <project/settingscene.h>

Renderer* renderer;
Level1* level1 = NULL;
MenuScene* menuscene = NULL;
SettingScene* settingscene = NULL;

int main(int argc, char* argv[]) {
	renderer = new Renderer();
	level1 = new Level1();
	menuscene = new MenuScene();
	settingscene = new SettingScene();

	SceneManager::addScene("level1", level1);
	SceneManager::addScene("settings", settingscene);
	SceneManager::addScene("menu", menuscene);
	SceneManager::loadScene("menu");

	while (!renderer->mustQuit()) {
		renderer->renderScene();
	}

	delete renderer;
	delete level1;
	delete menuscene;
	delete settingscene;
	return 0;
}