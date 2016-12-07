#include <engine/renderer.h>
#include <engine/scenemanager.h>
#include <project/mainscene.h>
#include <project/menuscene.h>
#include <project/settingscene.h>

Renderer* renderer;
MainScene* mainscene = NULL;
MenuScene* menuscene = NULL;
SettingScene* settingscene = NULL;

int main(int argc, char* argv[]) {
	renderer = new Renderer();
	mainscene = new MainScene();
	menuscene = new MenuScene();
	settingscene = new SettingScene();

	SceneManager::addScene("mainScene", mainscene);
	SceneManager::addScene("settings", settingscene);
	SceneManager::addScene("menu", menuscene);
	SceneManager::loadScene("menu");

	while (!renderer->mustQuit()) {
		renderer->renderScene();
	}

	delete renderer;
	delete mainscene;
	delete menuscene;
	delete settingscene;
	return 0;
}