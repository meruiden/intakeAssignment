#include <engine/renderer.h>

Renderer::Renderer()
{
	//Making sure every random value is not the same every time
	srand(time(NULL));

	//Init everything
	dt = 0;
	currentTime = 0;

	window_width = WINDOW_WIDTH;
	window_height = WINDOW_HEIGHT;
	fpsCounter = 0.0f;
	setFullScreenFix = false;
	fps = 0;
	//Init GL
	initGL();
	Camera::setWindow(window);
	Sound::init();

	SDL_DisplayMode current;

	SDL_Init(SDL_INIT_VIDEO);

	SDL_GetCurrentDisplayMode(0, &current);

	originalResolution = Vector2(current.w, current.h);

	lastRenderedTexture = 0;
	fixedUpdateCounter = 0.0;
}

bool Renderer::mustQuit()
{
	return Input::getInstance()->mustQuit();
}

void Renderer::initGL()
{
	// Initialize SDL2
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		return;
	}

	int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_TIF | IMG_INIT_WEBP;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		return;
	}
	// Create an application window with the following settings:
	window = SDL_CreateWindow(
		WINDOWNAME,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		window_width,
		window_height,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
	);

	lastWindowSize = Vector2(window_width, window_height);
	// Check that the window was successfully created
	if (window == NULL)
	{
		// In the case that the window could not be made...
		std::cout << "Could not create window: " << SDL_GetError() << std::endl;
		return;
	}

	//Create context
	gContext = SDL_GL_CreateContext(window);
	if (gContext == NULL)
	{
		printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
		return;
	}

	// Initialize GLEW
	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW\n");
		return;
	}

	SDL_GL_SetSwapInterval(0);

	//Enable blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	// Dark blue background
	Color bgColor = BACKGROUND_COLOR;

	glClearColor((float)bgColor.r / 255.0f, (float)bgColor.g / 255.0f, (float)bgColor.b / 255.0f, 0.0f);

	// Cull triangles which normal is not towards the camera
	glDisable(GL_CULL_FACE);

	// Create and compile GLSL program from the shaders
	programID = loadShaders("shaders/sprite.vert", "shaders/sprite.frag");

	// Get a handle for buffers
	vertexPosition_modelspaceID = glGetAttribLocation(programID, "vertexPosition_modelspace");
	vertexUVID = glGetAttribLocation(programID, "vertexUV");

	// Get a handle for Color uniforms
	rgbVec = glGetUniformLocation(programID, "rgb");
	alphafloat = glGetUniformLocation(programID, "alpha");
	uvoffset = glGetUniformLocation(programID, "uvOffset");

	// Get a handle for "MVP" uniform
	matrixID = glGetUniformLocation(programID, "MVP");
	// Get a handle for "myTextureSampler" uniform
	textureID = glGetUniformLocation(programID, "myTextureSampler");

	ProjectionMatrix = glm::ortho(0.0f, (float)window_width, (float)window_height, 0.0f, 0.1f, 100.0f);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// open 44.1KHz, signed 16bit, system byte order,
	//      stereo audio, using 1024 byte chunks
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
		printf("Mix_OpenAudio: %s\n", Mix_GetError());
	}
	Mix_AllocateChannels(MAX_AUDIO_PLAYING);

	TTF_Init();


	fullScreenFlag = SDL_WINDOW_FULLSCREEN;

	std::stringstream ss;
	ss << SDL_GetPlatform();
	if(ss.str() == "Linux")
	{
		fullScreenFlag = SDL_WINDOW_FULLSCREEN_DESKTOP;
	}

	glLineWidth(3.0f);
}

void Renderer::renderScene()
{
	updateDeltaTime();
	Scene* scene;
	scene = SceneManager::getCurrentScene();
	fixedUpdateCounter += dt;
	if (fixedUpdateCounter >= 1.0 / 60.0)
	{
		scene->fixedUpdate();
		scene->getPhysicsWorld()->Step(1.0 / 60.0, 8, 5);
		fixedUpdateCounter = fmod(fixedUpdateCounter, dt);
	}
	
	

	Input::getInstance()->update();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Vector2 curWindowSize = Camera::getWindowSize();

	if (lastWindowSize != curWindowSize)
	{
		window_width = curWindowSize.x;
		window_height = curWindowSize.y;

		glViewport(0, 0, curWindowSize.x, curWindowSize.y);

		ProjectionMatrix = glm::ortho(0.0f, (float)window_width, (float)window_height, 0.0f, 0.1f, 100.0f);

		if (setFullScreenFix)
		{
			setFullScreenFix = false;
			SDL_SetWindowSize(window, curWindowSize.x-1, curWindowSize.y-1);
			SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
		}

		lastWindowSize = curWindowSize;

	}

	if (SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED && Camera::getFullScreen() && setFullScreenFix)
	{
		SDL_RestoreWindow(window);
	}
	if (Camera::getFullScreen() && !(SDL_GetWindowFlags(window) & fullScreenFlag))
	{
		SDL_SetWindowFullscreen(window, fullScreenFlag);
	}

	if (!Camera::getFullScreen() && (SDL_GetWindowFlags(window) & fullScreenFlag))
	{

		Camera::setResolution(originalResolution);
		SDL_SetWindowFullscreen(window, 0);
		SDL_SetWindowSize(window, curWindowSize.x + 1, curWindowSize.y + 1);
		setFullScreenFix = true;

	}

	if (COUT_FPS) {
		showFps();
	}

	if (scene != NULL) {

		
		scene->update(dt);
		std::vector<Entity*> entities = scene->getEntities();

		unsigned int curLayer = 0;
		bool needHigherLayer = true;

		// Render all entities from the scene. lowest layer renders first highest layer renders last.

		while (needHigherLayer) {
			needHigherLayer = false;
			int numEntities = entities.size();
			for (unsigned int i = 0; i < numEntities; i++)
			{
				if (entities[i]->getLayer() == curLayer) {
					entities[i]->update(dt);
					glm::mat4 modelmatrix = glm::mat4(1.0f);
					renderEntity(modelmatrix, entities[i], scene->getCamera());
				}

				if (entities[i]->getLayer() > curLayer) {
					needHigherLayer = true;
				}
			}
			curLayer++;
		}

		std::vector<HudElement*> hudElements = scene->getHudElements();

		curLayer = 0;
		needHigherLayer = true;

		// Render all hud elements from the scene. lowest layer renders first highest layer renders last.

		while (needHigherLayer) {
			needHigherLayer = false;
			int numHudElements = hudElements.size();
			for (unsigned int i = 0; i < numHudElements; i++)
			{
				if (hudElements[i]->getLayer() == curLayer) {
					hudElements[i]->update(dt);
					renderHudElement(hudElements[i]);
				}

				if (hudElements[i]->getLayer() > curLayer) {
					needHigherLayer = true;
				}
			}
			curLayer++;
		}
	}


	SDL_GL_SwapWindow(window);
}

void Renderer::renderEntity(glm::mat4 &modelmatrix, Entity* entity, Camera* camera)
{
	// Use our shader
	glm::mat4 MVP;
	glUseProgram(programID);
	modelmatrix *= getModelMatrix(entity->getPosition(), entity->getScale(), entity->getRotation());

	MVP = ProjectionMatrix * camera->getViewMatrix() * modelmatrix;


	Texture* texture = NULL;
	Mesh* mesh = NULL;
	bool isVisable = false;

	// Get the globals
	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 position;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(modelmatrix, scale, rotation, position, skew, perspective);

	entity->setGlobals(Vector2(position.x, position.y), Vector2(scale.x, scale.y), rotation.z * RAD_TO_DEG);

	if (entity->getSprite() != NULL)
	{

		if (entity->getSprite()->hasDynamicMesh())
		{
			isVisable = isMeshVisable(Vector2(position.x, position.y), Vector2(scale.x, scale.y), Vector2(entity->getSprite()->getDynamicMesh()->getWidth(), entity->getSprite()->getDynamicMesh()->getHeight()), camera->getPosition());
		}
		else
		{
			isVisable = isMeshVisable(Vector2(position.x, position.y), Vector2(scale.x, scale.y), entity->getSprite()->getSpriteSize(), camera->getPosition());
		}

		if (entity->getSprite()->getSpriteSize().magnitude() == 0)
		{
			isVisable = true;
		}

		if (isVisable)
		{
			if (!entity->getSprite()->hasDynamicTexture())
			{
				if (entity->getSprite()->getFileName() == "")
				{
					return;
				}
				bool succes = false;
				texture = ResourceManager::getInstance()->getTexture(entity->getSprite()->getFileName(), succes);
				if (!succes)
				{
					return;
				}
			}
			else
			{

				texture = entity->getSprite()->getDynamicTexture();
			}

			entity->getSprite()->setTextureSize(Vector2(texture->getWidth(), texture->getHeight()));

			if (entity->getSprite()->getSpriteSize().x == 0 && entity->getSprite()->getSpriteSize().y == 0)
			{
				entity->getSprite()->setSpriteSize(Vector2(texture->getWidth(), texture->getHeight()));
				entity->setScale(entity->getScale());
			}
			if (!entity->getSprite()->hasDynamicMesh())
			{
				Vector2 spriteSize = entity->getSprite()->getSpriteSize();
				Vector2 uvSize = entity->getSprite()->getUvSize();
				mesh = ResourceManager::getInstance()->getSpriteMesh(spriteSize.x, spriteSize.y, uvSize.x, uvSize.y);
			}
			else
			{
				mesh = entity->getSprite()->getDynamicMesh();
			}
			entity->getSprite()->setSpriteSize(Vector2(mesh->getWidth(), mesh->getHeight()));
			renderMesh(MVP, mesh, texture->getTextureBuffer(), entity->getSprite()->getUvOffset(), entity->color);
		}
	}
	if (entity->getPhysicsBody()->getDrawColliders())
	{
		entity->getPhysicsBody()->regenerateColliderMesh();
		if (entity->getPhysicsBody()->getColliderDrawMesh() != NULL)
		{
			glm::mat4 colliderModel = getModelMatrix(entity->getPosition(), Vector2(1, 1), entity->getRotation());
			glm::mat4 colliderModerMVP = ProjectionMatrix * camera->getViewMatrix() * colliderModel;
			renderMesh(colliderModerMVP, entity->getPhysicsBody()->getColliderDrawMesh(), ResourceManager::getInstance()->getEmptyTexture(), Vector2(0, 0), COLLIDER_DRAW_COLOR);
		}
	}
	entity->setScale(entity->getScale());
	std::vector<Entity*>children = entity->getChildren();
	std::vector<Entity*>::iterator it = children.begin();
	while (it != children.end())
	{
		renderEntity(modelmatrix, (*it), camera);
		modelmatrix = getModelMatrix((*it)->getParent()->getPosition(), (*it)->getParent()->getScale(), (*it)->getParent()->getRotation());
		it++;
	}
}

void Renderer::renderHudElement(HudElement* hudelement)
{

	Vector2 position = Vector2();
	Vector2 windowSize = Camera::getWindowSize();

	position = hudelement->getGlobalPosition();

	// Use our shader
	glm::mat4 MVP;
	glUseProgram(programID);
	glm::mat4 modelmatrix = getModelMatrix(position + windowSize/2.0f, hudelement->getScale(), hudelement->getRotation());
	glm::mat4 vm = glm::lookAt(
		glm::vec3(0, 0, 5), // Camera is at (0,0,5), in World Space
		glm::vec3(0, 0, -5), // and looks towards Z
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);
	MVP = ProjectionMatrix * vm * modelmatrix;

	if (hudelement->getSprite() == NULL) {
		return;
	}

	Texture* texture = NULL;
	Mesh* mesh = NULL;
	bool isVisable = false;

	// Get the globals
	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 pos;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(modelmatrix, scale, rotation, pos, skew, perspective);

	if (hudelement->getSprite()->hasDynamicMesh())
	{
		isVisable = isMeshVisable(Vector2(pos.x, pos.y), Vector2(scale.x, scale.y), Vector2(hudelement->getSprite()->getDynamicMesh()->getWidth(), hudelement->getSprite()->getDynamicMesh()->getHeight()), Camera::getWindowSize() * 0.5f);
	}
	else 
	{
		isVisable = isMeshVisable(Vector2(pos.x, pos.y), Vector2(scale.x, scale.y), hudelement->getSprite()->getSpriteSize(), Camera::getWindowSize() * 0.5f);
	}

	if (hudelement->getSprite()->getSpriteSize().magnitude() == 0)
	{
		isVisable = true;
	}

	if (isVisable)
	{
		if (!hudelement->getSprite()->hasDynamicTexture())
		{
			if (hudelement->getSprite()->getFileName() == "")
			{
				return;
			}
			bool succes = false;

			texture = ResourceManager::getInstance()->getTexture(hudelement->getSprite()->getFileName(), succes);
			if (!succes)
			{
				return;
			}
		}
		else
		{
			texture = hudelement->getSprite()->getDynamicTexture();
		}
		Vector2 size = Vector2(texture->getWidth(), texture->getHeight());
		hudelement->getSprite()->setTextureSize(size);
		if (hudelement->getSprite()->getSpriteSize().x == 0 && hudelement->getSprite()->getSpriteSize().y == 0)
		{
			hudelement->getSprite()->setSpriteSize(size);
		}

		if (!hudelement->getSprite()->hasDynamicMesh())
		{
			Vector2 spriteSize = hudelement->getSprite()->getSpriteSize();
			Vector2 uvSize = hudelement->getSprite()->getUvSize();
			mesh = ResourceManager::getInstance()->getSpriteMesh(spriteSize.x, spriteSize.y, uvSize.x, uvSize.y);
		}
		else
		{
			mesh = hudelement->getSprite()->getDynamicMesh();
		}
		hudelement->getSprite()->setSpriteSize(Vector2(mesh->getWidth(), mesh->getHeight()));

		renderMesh(MVP, mesh, texture->getTextureBuffer(), hudelement->getSprite()->getUvOffset(), hudelement->color);
	}
}


void Renderer::renderMesh(glm::mat4 matrix, Mesh* mesh, GLuint textureBuffer, Vector2 uvOffset, Color color)
{

	// Send our transformation to the currently bound shader,
	// in the "MVP" uniform
	glUniformMatrix4fv(matrixID, 1, GL_FALSE, &matrix[0][0]);

	glUniform2f(uvoffset, uvOffset.x, uvOffset.y);
	glUniform1f(alphafloat, 1.0f / 255.0f * (float)color.a);
	
	glUniform3f(rgbVec, (1.0f / 255.0f) * (float)color.r, (1.0f / 255.0f) * (float)color.g, (1.0f / 255.0f) * (float)color.b);


	// Bind our texture in Texture Unit 0
	if (lastRenderedTexture != textureBuffer)
	{
		glActiveTexture(GL_TEXTURE0);

		glBindTexture(GL_TEXTURE_2D, textureBuffer);
		lastRenderedTexture = textureBuffer;
	}
	// Set our "myTextureSampler" sampler to user Texture Unit 0
	glUniform1i(textureID, 0);

	// 1st attribute buffer : vertices
	glEnableVertexAttribArray(vertexPosition_modelspaceID);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->getVertexBuffer());
	glVertexAttribPointer(
		vertexPosition_modelspaceID,  // The attribute we want to configure
		3,                            // size : x+y+z => 3
		GL_FLOAT,                     // type
		GL_FALSE,                     // normalized?
		0,                            // stride
		(void*)0                      // array buffer offset
	);

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(vertexUVID);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->getUvBuffer());
	glVertexAttribPointer(
		vertexUVID,                   // The attribute we want to configure
		2,                            // size : U+V => 2
		GL_FLOAT,                     // type
		GL_FALSE,                     // normalized?
		0,                            // stride
		(void*)0                      // array buffer offset
	);

	// Draw the triangles !
	
	glDrawArrays(mesh->getDrawMode(), 0, mesh->getNumVerts());

	glDisableVertexAttribArray(vertexPosition_modelspaceID);
	glDisableVertexAttribArray(vertexUVID);
}

bool Renderer::isMeshVisable(Vector2 position, Vector2 scale, Vector2 size, Vector2 cameraPos)
{
	float meshSize = std::max(size.x * scale.x, size.y * scale.y); //if the mesh is rotated and the width and height are not the same, it may be visable but not detectable as visable if this is not applied

	Vector2 windowSize = Camera::getWindowSize();

	if ((abs(position.x - cameraPos.x) * 2 < (meshSize + windowSize.x)) &&
		(abs(position.y - cameraPos.y) * 2 < (meshSize + windowSize.y)))
	{
		return true;
	}

	return false;
}

glm::mat4 Renderer::getModelMatrix(Vector2 pos, Vector2 scal, float rot)
{
	glm::vec3 position = glm::vec3(pos.x, pos.y, 0.0f);
	glm::vec3 rotation = glm::vec3(0.0f, 0.0f, rot);
	glm::vec3 scale = glm::vec3(scal.x, scal.y, 1.0f);

	// Build the Model matrix
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
	glm::mat4 rotationMatrix = glm::eulerAngleYXZ(0.0f, 0.0f, (float)(rotation.z * DEG_TO_RAD));
	glm::mat4 scalingMatrix = glm::scale(glm::mat4(1.0f), scale);

	glm::mat4 mm = translationMatrix * rotationMatrix * scalingMatrix;

	return mm;
}

void Renderer::updateDeltaTime()
{
	double old_time = currentTime;
	currentTime = SDL_GetTicks();
	dt = (currentTime - old_time) / 1000.0f;
}

void Renderer::showFps() {
	fpsCounter += dt;
	fps++;
	if (fpsCounter >= 1.0f) {

		fpsCounter = fmod(fpsCounter, dt);
		std::cout << "FPS: " << fps << std::endl;
		fps = 0;
	}
}
Renderer::~Renderer()
{

	if(Camera::getFullScreen())
	{
		Camera::setResolution(originalResolution);
	}
	// delete RecouseManager instance
	delete  ResourceManager::getInstance();

	// delete Input instance
	delete Input::getInstance();

	glDeleteProgram(programID);
	glDeleteTextures(1, &textureID);
	std::cout << "DELETED SHADER" << std::endl;
	SDL_GL_DeleteContext(gContext);
	std::cout << "DELETED CONTEXT" << std::endl;
	// Close and destroy the window
	SDL_DestroyWindow(window);
	std::cout << "DELETED WINDOW" << std::endl;
	// Quit SDL
	SDL_Quit();
	std::cout << "CLOSED SDL" << std::endl;

}
