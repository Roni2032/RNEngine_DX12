#include "stdafx.h"
#include "CollisionManager.h"

namespace RNEngine {
	CollisionManager::CollisionManager(){}
	CollisionManager::~CollisionManager(){}

	void CollisionManager::RegisterCollision(const shared_ptr<Collision>& collision) {
		m_Collisions.push_back(collision);
	}

	void CollisionManager::Update() {

	}
}