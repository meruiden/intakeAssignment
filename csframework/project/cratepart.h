#pragma once

#include <sstream>
#include <vector>

#include <engine/entity.h>
#include <engine/vector2.h>

class CratePart : public Entity
{
public:
	CratePart(int slideId);
	virtual ~CratePart();
	virtual void update(float deltaTime);

	bool destroyMe() { return lifeTimeCounter >= lifeTime; }
	std::vector<Vector2> getColliderVerts(Vector2 scale);

private:
	int sliceId;
	float lifeTime;
	float lifeTimeCounter;

};

