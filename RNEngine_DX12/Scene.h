#pragma once
#include "stdafx.h"

namespace RNEngine {
	class GameObject;
	class Camera;

	class Scene : public enable_shared_from_this<Scene>
	{
		vector<shared_ptr<GameObject>> m_GameObjects;
		unordered_map<string, shared_ptr<Camera>> m_CameraMap;
	public:
		Scene(){}
		~Scene(){}

		virtual void Start();
		virtual void Update();
		virtual void LastUpdate();
		virtual void Draw();

		shared_ptr<Camera> CreateCameraObject(const string& key);

		template<class T,class... Params>
		shared_ptr<T> AddGameObject(Params&&... params) {
			shared_ptr<T> ptr = make_shared<T>(shared_from_this(), params...);
			ptr->Start();
			m_GameObjects.push_back(ptr);
			return ptr;
		}

		vector<shared_ptr<GameObject>> GetGameObjects();

		shared_ptr<Camera> GetCamera(const string& name);
		void RegisterCamera(const string& name, const shared_ptr<Camera>& camera);
	};
	class SceneManager {
		unordered_map<string, shared_ptr<Scene>> m_SceneMap;
		string m_ActiveSceneName;
	};
}

