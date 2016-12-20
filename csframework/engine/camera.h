#pragma once

#include <iostream>
#include <vector>

#include <SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <engine/vector2.h>
#include <engine/config.h>


class Camera
{
public:
	Camera();
	virtual ~Camera();
	
	glm::mat4 getViewMatrix();
	static void setWindow(SDL_Window* window);
	static void setResolution(Vector2 resolution);
	static void setFullScreen(bool value);
	static void toggleFullScreen();

	void setPosition(Vector2 newPos);
	
	Vector2 screenToWorldSpace(Vector2 screenSpace);
	Vector2 worldToScreenSpace(Vector2 worldSpace);
	Vector2 getPosition() { return position; }
	
	static std::vector<Vector2> getAvailableResolutions();

	static Vector2 getWindowSize();
	static Vector2 getResolution() { return resolution; }

	static bool getFullScreen() { return fullscreen; }
private:
	static SDL_Window* window;

	static Vector2 resolution;
	static bool fullscreen;
	Vector2 position;
	
};

