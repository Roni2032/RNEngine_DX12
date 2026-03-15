#include "stdafx.h"
#include "project.h"
namespace RNEngine {
	bool HitTest::RayAABB(const Ray& ray, const AABB& aabb){
		auto& aabbMax = aabb.m_Max;
		auto& aabbMin = aabb.m_Min;

		float entryDist = -FLT_MAX;
		float exitDist = FLT_MAX;

		//x,y,z軸それぞれに対して判定
		for(int i = 0;i < 3;i++) {
			const float& originPos = ray.m_Origin[i];
			const float& direction = ray.m_Direction[i];

			const float& max = aabbMax[i];
			const float& min = aabbMin[i];

			//レイの向きが0のときは、レイの原点がAABBの範囲内にあるかどうかで判定
			if (direction == 0) {
				if (originPos < min || originPos > max)return false;
				continue;
			}
			float odd = 1.0f / direction;
			float nearDist = (min - originPos) * odd;
			float farDist = (max - originPos) * odd;

			if (nearDist < 0 && farDist < 0) return false;

			if (nearDist > farDist) {
				std::swap(nearDist, farDist);
			}
			entryDist = max(entryDist, nearDist);
			exitDist = std::min(exitDist, farDist);

			if (entryDist >= exitDist) return false;
		}

		return entryDist <= ray.m_Length;
	}
	bool HitTest::RayMesh(const Ray& ray, const shared_ptr<GameObject>& object) {
		//ここにレイとメッシュの当たり判定
		return false;
	}

	Collision::Collision(const shared_ptr<GameObject>& ptr):Component(ptr){}
	Collision::~Collision() {}

	bool Collision::IsSimpleHit(const shared_ptr<Collision>& other) {
		AABB aabb = GetAABB();
		AABB otherAabb = other->GetAABB();

		//ここにAABBの当たり判定
		Vector3 destCenter = aabb.GetCenter() - otherAabb.GetCenter();
		Vector3 totalHalfScale = aabb.GetSize() + otherAabb.GetSize();
		totalHalfScale /= 2.0f;

		if (fabs(destCenter.x) > totalHalfScale.x) return false;
		if (fabs(destCenter.y) > totalHalfScale.y) return false;
		if (fabs(destCenter.z) > totalHalfScale.z) return false;

		return true;
	}

	CollisionCube::CollisionCube(const shared_ptr<GameObject>& ptr) :Collision(ptr) {}
	CollisionCube::~CollisionCube() {}

	AABB CollisionCube::GetAABB() {
		auto owner = GetOwner();
		Vector3 position = owner->GetTransform()->GetPosition();
		Vector3 offset = GetOffset();
		position += offset;


		Vector3 max = position + m_Scale / 2.0f;
		Vector3 min = position - m_Scale / 2.0f;

		return AABB(max, min);
	}

	void CollisionCube::Draw() {
		auto aabb = GetAABB();
		DebugRenderer::Get().DrawCubeWireFrame(aabb.GetCenter(), aabb.GetSize());
	}
}