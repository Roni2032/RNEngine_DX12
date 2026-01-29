#include "stdafx.h"
#include "project.h"

namespace RNEngine {
	void Scene::Start() {

	}
	void Scene::Update() {
		for (auto& cameraMap : m_CameraMap) {
			auto& camera = cameraMap.second;
			camera->Update();
		}
		for (auto& object : m_GameObjects) {
			object->Update();
			object->ComponentUpdate();
		}
	}
	void Scene::Draw() {
		auto renderer = Engine::GetRenderer();
		for (auto& object : m_GameObjects) {
			auto rendererComponents = object->GetRendererComponent();
			for (auto& rendererComponent : rendererComponents) {
				renderer->Draw(rendererComponent);
			}
			auto components = object->GetComponents();
			for(auto& component : components) {
				component->Draw();
			}
		}
		//複数のレンダーターゲットを利用できるようになったら戻そうね
		//renderer->DrawAll();
	}


	vector<shared_ptr<GameObject>> Scene::GetGameObjects() {
		return m_GameObjects;
	}

	shared_ptr<Camera> Scene::GetCamera(const string& name) {
		auto it = m_CameraMap.find(name);
		if (it != m_CameraMap.end()) {
			return it->second;
		}
		return nullptr;
	}
	void Scene::RegisterCamera(const string& name, const shared_ptr<Camera>& camera) {
		m_CameraMap[name] = camera;
	}
}