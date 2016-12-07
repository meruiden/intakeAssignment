#pragma once

#include <engine/scene.h>
#include <map>
class SceneManager
{
public:
	static void loadScene(std::string sceneName);
	static void addScene(std::string sceneName, Scene* scene);
	static Scene* getCurrentScene() { return SceneManager::currentScene; }
private:
	static std::map<std::string, Scene*> scenes;
	static Scene* currentScene;

};

