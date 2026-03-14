#include "stdafx.h"
#include "project.h"
namespace RNEngine {
	bool HitTest::RayAABB(const Ray& ray, const AABB& aabb){
		auto& aabbMax = aabb.m_Max;
		auto& aabbMin = aabb.m_Min;


		//x,y,z軸それぞれに対して判定
		for(int i = 0;i < 3;i++) {
			const float& originPos = ray.m_Origin[i];
			const float& direction = ray.m_Direction[i];

			const float& max = aabbMax[i];
			const float& min = aabbMin[i];

			if (direction == 0) {
				//レイの原点がAABBの範囲内にあるか判定
				if (originPos < min || originPos > max) {
					return false;
				}
				continue;
			}

			float nearDist = (min - originPos) / direction;
			float farDist = (max - originPos) / direction;

			if (abs(nearDist) > abs(farDist)) {
				swap(nearDist, farDist);
			}

			//レイが届かないまたは、レイの反対側にAABBがある場合は当たらない
			if (nearDist > ray.m_Length || (nearDist < 0 && farDist < 0)) {
				return false;
			}
		}
		return true;
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