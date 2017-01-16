#pragma once
#include <engine/entity.h>
#include <engine/camera.h>
class Background : public Entity
{
public:
	Background(Camera* sceneCamera);
	virtual ~Background();
	virtual void update(float deltaTime);

private:
	Camera* sceneCamera;
};

