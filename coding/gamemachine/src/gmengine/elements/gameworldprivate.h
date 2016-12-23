﻿#ifndef __GAMEWORLD_PRIVATE_H__
#define __GAMEWORLD_PRIVATE_H__
#include "common.h"
#include <vector>
#include "utilities/autoptr.h"
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"

BEGIN_NS

class GameObject;
class Character;
struct IGraphicEngine;
class GameWorldPrivate
{
	friend class GameWorld;

private:
	GameWorldPrivate();
	~GameWorldPrivate();

	void init();
	void setGravity(GMfloat x, GMfloat y, GMfloat z);
	void appendObject(GameObject* obj);

private:
	AutoPtr<btDefaultCollisionConfiguration> m_collisionConfiguration;
	AutoPtr<btCollisionDispatcher> m_dispatcher;
	AutoPtr<btBroadphaseInterface> m_overlappingPairCache;
	AutoPtr<btSequentialImpulseConstraintSolver> m_solver;
	AutoPtr<btGhostPairCallback> m_ghostPairCallback;
	AutoPtr<btDiscreteDynamicsWorld> m_dynamicsWorld;
	AutoPtr<IGraphicEngine> m_pEngine;
	std::vector<GameObject*> m_shapes;
	Character* m_character;
};

END_NS
#endif