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
		}
		renderer->DrawAll();
	}
}