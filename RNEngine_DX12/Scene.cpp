#include "stdafx.h"
#include "project.h"
#include "Renderer.h"

namespace RNEngine {
	void Scene::Start() {
		RegisterLayer("Default", 0);
		RegisterLayer("Object", 1);
		RegisterLayer("UI", 2);
	}
	void Scene::Update() {
		for (auto& object : m_GameObjects) {
			object->Update();
		}
	}
	void Scene::LastUpdate() {
		for (auto& object : m_GameObjects) {
			object->LastUpdate();
		}
	}
	void Scene::Draw() {
		auto renderer = Engine::GetRenderer();
		for (auto& object : m_GameObjects) {
			//どのカメラから見たものを描画するか
			for (auto& camera : m_CameraMap) {
				auto renderingLayers = camera.second->GetRenderingLayers();

				if (renderingLayers.size() < 0 || !object->CompareLayer(renderingLayers)) continue;

				//後々、Unityみたいにカメラが持つレンダーターゲットに書き込む...とかにしたいなあ...
				auto rendererComponents = object->GetRendererComponent();
				for (auto& rendererComponent : rendererComponents) {
					//カメラ指定(初期化で設定しないでほしい)
					camera.second->AddRenderObject(rendererComponent);
				}
				object->Draw();
			}
			
		}
		for (auto& camera : m_CameraMap) {
			camera.second->DrawRenderTarget();
		}
	}

	shared_ptr<Camera> Scene::CreateCameraObject(const string& key) {
		auto object = AddGameObject();
		object->SetName(key);
		auto camera = object->AddComponent<Camera>();
		RegisterCamera(key, camera);

		return camera;
	}
	void Scene::RegisterMainCamera(const string& key) {
		auto renderer = Engine::GetRenderer();
		renderer->RegisterMainCamera(GetCamera(key));
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