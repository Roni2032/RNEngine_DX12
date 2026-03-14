#include "stdafx.h"
#include "project.h"

namespace RNEngine{
	bool RayCast::Hit(const Ray& ray) {
		auto currentScne = Engine::GetCurrentScene();
		auto objects = currentScne->GetGameObjects();
		for (auto& object : objects) {
			auto draw = object->GetComponent<ModelRenderer>();
			if (draw == nullptr) continue;
			auto transform = object->GetTransform();

			Vector3 position = transform->GetPosition();
			Vector3 scale = transform->GetScale();

			auto modelData = draw->GetModel()->GetModelData();
			//モデルのローカル空間のAABBをワールド空間に変換
			auto boundingBox = modelData.m_LocalBoundingBox * scale + position;

			if (HitTest::RayAABB(ray, boundingBox)) {
				return true;
			}
		}

		return false;
	}
}