#pragma once
#include "stdafx.h"
#include "GameObject.h"

namespace RNEngine {
	class Camera;

	class Scene : public enable_shared_from_this<Scene>
	{
		//シーンに追加されたゲームオブジェクト一覧
		vector<shared_ptr<GameObject>> m_GameObjects;

		//追加されたカメラ一覧
		unordered_map<string, shared_ptr<Camera>> m_CameraMap;

		//登録されたレイヤー一覧
		unordered_map<string, int> m_LayerMap;
	public:
		Scene(){}
		~Scene(){}

		virtual void Start();
		virtual void Update();
		virtual void LastUpdate();
		virtual void Draw();

		shared_ptr<Camera> CreateCameraObject(const string& key);
		void RegisterMainCamera(const string& key);

		template<class... Params>
		shared_ptr<GameObject> AddGameObject(Params&&... params) {
			shared_ptr<GameObject> ptr = make_shared<GameObject>(shared_from_this(), params...);
			ptr->Start();
			m_GameObjects.push_back(ptr);
			return ptr;
		}

		vector<shared_ptr<GameObject>> GetGameObjects();

		shared_ptr<Camera> GetCamera(const string& name);
		void RegisterCamera(const string& name, const shared_ptr<Camera>& camera);

		void RegisterLayer(const string& name, int layer) {
			m_LayerMap[name] = layer;
		}
		string GetLayerName(int layer) {
			for (auto& pair : m_LayerMap) {
				if (pair.second == layer) {
					return pair.first;
				}
			}
			return "";
		}
		int GetLayer(const string& name) {
			auto it = m_LayerMap.find(name);
			if (it != m_LayerMap.end()) {
				return (*it).second;
			}
			return -1;
		}
	};
	class SceneManager {
		unordered_map<string, shared_ptr<Scene>> m_SceneMap;
		string m_ActiveSceneName;
	};
}

