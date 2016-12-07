#include <engine/camera.h>

SDL_Window* Camera::window = NULL;
Vector2 Camera::resolution = Vector2(WINDOW_WIDTH, WINDOW_HEIGHT);
bool Camera::fullscreen = false;

Camera::Camera()
{
	position = Vector2();
}

Vector2 Camera::getWindowSize()
{
	int w = 0;
	int h = 0;
	SDL_GetWindowSize(Camera::window, &w, &h);
	return Vector2(w, h);
}

Vector2 Camera::screenToWorldSpace(Vector2 screenSpace)
{
	Vector2 worldSpace = screenSpace;
	worldSpace += getPosition();
	return worldSpace;
}

void Camera::setPosition(Vector2 newPos)
{
	position = newPos;
}

glm::mat4 Camera::getViewMatrix() 
{
	glm::vec3 pos = glm::vec3(position.x, position.y, 5);
	
	glm::vec3 offset = glm::vec3(getWindowSize().x / 2.0f, getWindowSize().y / 2.0f, 0);

	pos -= offset;
	// Return view matrix
	return glm::lookAt(
		pos, // Camera is at (xpos,ypos,5), in World Space
		pos + glm::vec3(0, 0, -5), // and looks towards Z
		glm::vec3(0, 1, 0)  // Head is up
	);
}

void Camera::setWindow(SDL_Window* window)
{
	Camera::window = window;
}

std::vector<Vector2> Camera::getAvailableResolutions()
{
	std::vector<Vector2> resolutions;

	for (unsigned int i = 0; i < SDL_GetNumDisplayModes(0); ++i) {
		SDL_DisplayMode mode;
		int should_be_zero = SDL_GetDisplayMode(0, i, &mode);

		if (should_be_zero != 0)
		{
			// In case of error...
			SDL_Log("Could not get display mode for video display #%d: %s", i, SDL_GetError());
			i++;
		}
		
		resolutions.push_back(Vector2(mode.w, mode.h));

	}

	return resolutions;
}

void Camera::setResolution(Vector2 resolution)
{

	SDL_SetWindowSize(window, resolution.x, resolution.y);
	SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

	Camera::resolution = resolution;
}

void Camera::setFullScreen(bool value)
{
	Camera::fullscreen = value;
}

void Camera::toggleFullScreen()
{
	Camera::setFullScreen(!Camera::getFullScreen());
}

Camera::~Camera()
{

}
