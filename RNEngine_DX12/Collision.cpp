#include "stdafx.h"
#include "project.h"
namespace RNEngine {
	Collision::Collision(const shared_ptr<GameObject>& ptr):Component(ptr){}
	Collision::~Collision() {}

	bool Collision::IsSimpleHit(const shared_ptr<Collision>& other) {
		AABB aabb = GetAABB();
		AABB otherAabb = other->GetAABB();

		//‚±‚±‚ÉAABB‚Ì“–‚½‚è”»’è
		Vector3 destCenter = aabb.m_Center - otherAabb.m_Center;
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
		Vector3 position = owner->GetComponent<Transform>()->GetPosition();
		Vector3 offset = GetOffset();

		Vector3 max = position + m_Scale / 2.0f;
		max += offset;
		Vector3 min = position - m_Scale / 2.0f;
		min += offset;

		return AABB(max, min);
	}

	void CollisionCube::Draw() {
		auto aabb = GetAABB();
		DebugRenderer::DrawCubeWireFrame(aabb.m_Center, aabb.GetSize());
	}
}