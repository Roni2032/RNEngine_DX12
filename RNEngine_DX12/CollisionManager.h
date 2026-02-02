#pragma once

#include "Component.h"
namespace RNEngine {
	class Collision;
	/*
	* アルゴリズムは八分木を想定
	* https://edom18.hateblo.jp/entry/2017/07/28/083153
	*/
	class CollisionManager{
		vector<weak_ptr<Collision>> m_Collisions;
	public:
		CollisionManager();
		~CollisionManager();

		void RegisterCollision(const shared_ptr<Collision>& collision);

		void Update();

	};

	//八分木アルゴリズム
	class Octree {
	public:

	};
}
