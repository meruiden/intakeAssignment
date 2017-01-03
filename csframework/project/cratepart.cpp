#include <project/cratepart.h>


CratePart::CratePart(int sliceId) : Entity()
{
	this->sliceId = sliceId;
	std::stringstream pathname;
	pathname << "assets/crate/crate_slice_";
	pathname << sliceId;
	pathname << ".png";

	this->addSprite(pathname.str());
	lifeTime = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) * 6 + 3;
	lifeTimeCounter = 0;

	getPhysicsBody()->setPhysicsActive(true);

	getPhysicsBody()->setDrawColliders(true);
}


CratePart::~CratePart()
{

}

void CratePart::update(float deltaTime)
{
	lifeTimeCounter += deltaTime;
}

std::vector<Vector2> CratePart::getColliderVerts(Vector2 scale)
{
	std::vector<Vector2> colliderVerts;
	switch (sliceId)
	{
	case 1:
		colliderVerts.push_back(Vector2(38, 59));
		colliderVerts.push_back(Vector2(-37, 59));
		colliderVerts.push_back(Vector2(-36, 4));
		colliderVerts.push_back(Vector2(37, -63));
		break;
	case 2:
		colliderVerts.push_back(Vector2(61, 31));
		colliderVerts.push_back(Vector2(-64, 32));
		colliderVerts.push_back(Vector2(-26, -37));
		colliderVerts.push_back(Vector2(13, -42));
		colliderVerts.push_back(Vector2(64, -23));
		break;
	case 3:
		colliderVerts.push_back(Vector2(-45, 56));
		colliderVerts.push_back(Vector2(-45, -56));
		colliderVerts.push_back(Vector2(46, -31));
		colliderVerts.push_back(Vector2(45, -18));
		colliderVerts.push_back(Vector2(6, 55));
		break;
	case 4:
		colliderVerts.push_back(Vector2(-37, 62));
		colliderVerts.push_back(Vector2(-63, 25));
		colliderVerts.push_back(Vector2(-64, -40));
		colliderVerts.push_back(Vector2(62, -31));
		colliderVerts.push_back(Vector2(62, 28));
		break;
	case 5:
		colliderVerts.push_back(Vector2(42, 60));
		colliderVerts.push_back(Vector2(-46, 46));
		colliderVerts.push_back(Vector2(-59, -63));
		colliderVerts.push_back(Vector2(54, 3));
		break;
	case 6:
		colliderVerts.push_back(Vector2(41, 54));
		colliderVerts.push_back(Vector2(-58, -22));
		colliderVerts.push_back(Vector2(-58, -56));
		colliderVerts.push_back(Vector2(57, -56));
		break;
	case 7:
		colliderVerts.push_back(Vector2(20, 63));
		colliderVerts.push_back(Vector2(-7, 63));
		colliderVerts.push_back(Vector2(-49, 49));
		colliderVerts.push_back(Vector2(-33, -63));
		colliderVerts.push_back(Vector2(46, -63));
		break;
	case 8:
		colliderVerts.push_back(Vector2(36, 39));
		colliderVerts.push_back(Vector2(-13, 36));
		colliderVerts.push_back(Vector2(-37, -18));
		colliderVerts.push_back(Vector2(-22, -43));
		colliderVerts.push_back(Vector2(37, -43));
		break;
	case 9:
		colliderVerts.push_back(Vector2(18, 63));
		colliderVerts.push_back(Vector2(0, 63));
		colliderVerts.push_back(Vector2(-42, 49));
		colliderVerts.push_back(Vector2(-46, -45));
		colliderVerts.push_back(Vector2(-32, -63)); 
		colliderVerts.push_back(Vector2(42, -3));
		colliderVerts.push_back(Vector2(43, 44));
		break;
	}
	for (int i = 0; i < colliderVerts.size(); i++)
	{
		colliderVerts[i] *= scale;
	}

	return colliderVerts;
}
