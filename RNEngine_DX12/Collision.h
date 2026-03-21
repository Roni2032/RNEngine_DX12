#pragma once
#include "stdafx.h"
#include "Component.h"
#include "RayCast.h"

namespace RNEngine {

	class HitTest {
	public:
		static bool RayAABB(const Ray& ray, const AABB& aabb, HitInfo* info);
		static bool RayMesh(const Ray& ray, const shared_ptr<GameObject>& object);
	};

	class Collision : public Component{
		Vector3 m_Offset;
	public:
		Collision(const shared_ptr<GameObject>& ptr);
		virtual ~Collision();

		/// <summary>
		/// 簡易的な当たり判定(AABB)
		/// </summary>
		/// <param name="other">他のコリジョン</param>
		/// <returns>判定</returns>
		bool IsSimpleHit(const shared_ptr<Collision>& other);

		/// <summary>
		/// ちゃんとした当たり判定
		/// </summary>
		/// <returns>判定</returns>
		virtual bool IsHit() { return false; }

		/// <summary>
		/// 簡易判定用AABBの取得
		/// Collisionごとに書く
		/// </summary>
		/// <returns>AABB</returns>
		virtual AABB GetAABB(){ return {}; }

		void SetOffset(Vector3 offset) { m_Offset = offset; }
		Vector3 GetOffset()const { return m_Offset; }
	};


	class CollisionCube : public Collision {
		Vector3 m_Scale;
	public:
		CollisionCube(const shared_ptr<GameObject>& ptr);
		virtual ~CollisionCube();

		virtual AABB GetAABB();

		void SetScale(Vector3 scale) { m_Scale = scale; }
		void SetScale(float scale) { m_Scale = Vector3(scale); }

		Vector3 GetScale()const { return m_Scale; }

		virtual void Draw()override;
	};
}
