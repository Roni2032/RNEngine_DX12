#include "stdafx.h"
#include "project.h"
namespace RNEngine {
	void GameObject::Start() {
		m_Transform = make_shared<Transform>(GetThis<GameObject>());
	}
	void GameObject::Update() {
		for (auto& component : m_Components) {
			component->Update();
		}
	}
	void GameObject::LastUpdate() {
		for (auto& component : m_Components) {
			component->LastUpdate();
		}
	}
	void GameObject::Draw() {
		for (auto& component : m_Components) {
			component->Draw();
		}
#ifdef _DEBUG
		//デバッグ用に3D空間の矢印を表示

		auto gui = Engine::GetGUIRenderer();
		auto inspector = gui->GetGui<Inspector>("inspector");
		if (inspector && inspector->GetCurrentGameObject().get() == this) {
			DebugRenderer::Get().DrawTransformGizmo(m_Transform->GetPosition(), 1.0f);
#endif
		}
	}
	vector<shared_ptr<RendererComponent>> GameObject::GetRendererComponent() {
		vector<shared_ptr<RendererComponent>> renderers;
		for (auto& component : m_Components) {
			auto ptr = dynamic_pointer_cast<RendererComponent>(component);
			if (ptr) {
				renderers.push_back(ptr);
			}
		}
		return renderers;
	}

	shared_ptr<Scene> GameObject::GetOwnerScene() {
		return m_OwnerScene.lock();
	}
	shared_ptr<Transform> GameObject::GetTransform() {
		return m_Transform;
	}
}
