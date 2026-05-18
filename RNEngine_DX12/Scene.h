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
		unordered_map<wstring, shared_ptr<Camera>> m_CameraMap;

		//登録されたレイヤー一覧
		unordered_map<wstring, int> m_LayerMap;
	public:
		Scene(){}
		~Scene(){}

		virtual void Start();
		virtual void Update();
		virtual void LastUpdate();
		virtual void Draw();

		shared_ptr<Camera> CreateCameraObject(const wstring& key);
		void RegisterMainCamera(const wstring& key);

		template<class... Params>
		shared_ptr<GameObject> AddGameObject(Params&&... params) {
			shared_ptr<GameObject> ptr = make_shared<GameObject>(shared_from_this(), params...);
			ptr->Start();
			m_GameObjects.push_back(ptr);
			return ptr;
		}

		vector<shared_ptr<GameObject>> GetGameObjects();

		shared_ptr<Camera> GetCamera(const wstring& name);
		void RegisterCamera(const wstring& name, const shared_ptr<Camera>& camera);

		void RegisterLayer(const wstring& name, int layer) {
			m_LayerMap[name] = layer;
		}
		wstring GetLayerName(int layer) {
			for (auto& pair : m_LayerMap) {
				if (pair.second == layer) {
					return pair.first;
				}
			}
			return L"";
		}
		int GetLayer(const wstring& name) {
			auto it = m_LayerMap.find(name);
			if (it != m_LayerMap.end()) {
				return (*it).second;
			}
			return -1;
		}
	};
	class SceneManager {
		unordered_map<wstring, shared_ptr<Scene>> m_SceneMap;
		wstring m_ActiveSceneName;
	};
}

