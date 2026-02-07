#include "stdafx.h"
#include "project.h"

namespace RNEngine {
	void Scene::Start() {
		CreateCameraObject("Debug");
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
				auto renderingTags = camera.second->GetRenderingTags();
				//タグが指定していなければ全部描画する
				//タグが見つかれば描画する
				if (renderingTags.size() > 0 && !object->HasTags(renderingTags)) continue;

				//後々、Unityみたいにカメラが持つレンダーターゲットに書き込む...とかにしたいなあ...
				auto rendererComponents = object->GetRendererComponent();
				for (auto& rendererComponent : rendererComponents) {
					//カメラ指定(初期化で設定しないでほしい)
					rendererComponent->Init(camera.second);
					renderer->Draw(rendererComponent);
				}
				object->Draw();
			}
			
		}
		//複数のレンダーターゲットを利用できるようになったら戻そうね
		//renderer->DrawAll();
	}

	shared_ptr<Camera> Scene::CreateCameraObject(const string& key) {
		auto object = AddGameObject<GameObject>();
		object->SetName(key);
		auto camera = object->AddComponent<Camera>();
		RegisterCamera(key, camera);

		return camera;
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