#pragma once

#include <stdio.h>
#include <time.h>

// Include GLEW
#include <GL/glew.h>

// Include SDL2
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_thread.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/matrix_decompose.hpp>

//include classes from engine
#include <engine/shader.h>
#include <engine/input.h>
#include <engine/entity.h>
#include <engine/camera.h>
#include <engine/resourcemanager.h>
#include <engine/scene.h>
#include <engine/sound.h>
#include <engine/config.h>
#include <engine/scenemanager.h>
#include <engine/texture.h>
#include <engine/mesh.h>
#include <engine/hudelement.h>

class Renderer
{

public:
	Renderer();

	virtual ~Renderer();

	void renderScene();

	void initGL();

	bool mustQuit();
private:
	//The window
	SDL_Window* window;
	//OpenGL context
	SDL_GLContext gContext;

	GLuint programID;
	GLuint vertexPosition_modelspaceID;
	GLuint vertexUVID;
	GLuint alphafloat;
	GLuint rgbVec;
	GLuint matrixID;
	GLuint textureID;
	GLuint uvoffset;

	static Input* input;

	glm::mat4 ProjectionMatrix;
	glm::mat4 getModelMatrix(Vector2 pos, Vector2 scal, float rot);

	//Declare variables for window size
	int window_width;
	int window_height;

	void renderEntity(glm::mat4 &modelmatrix, Entity* entity, Camera* camera);
	void renderHudElement(HudElement* hudelement);
	void updateDeltaTime();
	void showFps();
	void renderMesh(glm::mat4 matrix, Mesh* mesh, GLuint textureBuffer, Vector2 uvOffset, Color color);

	double currentTime;
	double dt;
	double fixedUpdateCounter;

	bool setFullScreenFix;
	bool isMeshVisable(glm::mat4 modelMatrix, Vector2 size, Vector2 cameraPos);
	float fpsCounter;
	int fps;

	Vector2 lastWindowSize;
	Vector2 originalResolution;

	int fullScreenFlag;

	GLuint lastRenderedTexture;

};
