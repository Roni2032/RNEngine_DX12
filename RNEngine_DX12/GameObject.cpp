#include "stdafx.h"
#include "project.h"
namespace RNEngine {
	void GameObject::Start() {
		AddComponent<Transform>();
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
	void GameObject::ComponentUpdate() {
		for (auto& component : m_Components) {
			component->Update();
		}
		for (auto& component : m_Components) {
			component->LateUpdate();
		}
	}

}
