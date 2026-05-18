#include "stdafx.h"
#include "project.h"

namespace RNEngine{
	bool RayCast::Hit(const Ray& ray, HitInfo* info) {
		auto currentScne = Engine::GetCurrentScene();
		auto objects = currentScne->GetGameObjects();
		shared_ptr<GameObject> frontObject = nullptr;
		float hitDist = -FLT_MAX;

		for (auto& object : objects) {
			auto draw = object->GetComponent<ModelRenderer>();
			if (draw == nullptr) continue;
			auto transform = object->GetTransform();

			Vector3 position = transform->GetPosition();
			Vector3 scale = transform->GetScale();

			auto& modelData = draw->GetModel()->GetModelData();
			//モデルのローカル空間のAABBをワールド空間に変換
			auto boundingBox = modelData.m_LocalBoundingBox * scale + position;

			//RayAABB : 簡易テスト
			//RayMesh : メッシュとのちゃんとした当たり判定(未実装)
			if (HitTest::RayAABB(ray, boundingBox,info)/* && HitTest::RayMesh(ray,object)*/) {
				info->m_Object = object;
				return true;
			}
		}
		return false;
	}
}