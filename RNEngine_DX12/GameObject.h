#pragma once
#include "stdafx.h"
#include "Object.h"
namespace RNEngine {
	class Component;
	class Scene;
	class RendererComponent;

	class GameObject : public Object {
		string m_Name;
		weak_ptr<Scene> m_OwnerScene;
		vector<shared_ptr<Component>> m_Components;
	public:
		GameObject(const shared_ptr<Scene>& ptr) :m_OwnerScene(ptr),m_Name("GameObject"), Object() {}
		virtual ~GameObject() {}

		virtual void Start();

		void SetName(const string& name) {
			m_Name = name;
		}
		string GetName() {
			return m_Name;
		}

		template<class T,class... Param>
		shared_ptr<T> AddComponent(Param&&... params) {
			shared_ptr<T> ptr = make_shared<T>(GetThis<GameObject>(),params...);
			ptr->Start();
			m_Components.push_back(ptr);
			return ptr;
		}
		template<class T>
		shared_ptr<T> GetComponent() {
			auto it = m_Components.begin();
			for (; it != m_Components.end(); it++) {
				auto ptr = dynamic_pointer_cast<T>(*it);
				if (ptr) {
					return ptr;
				}
			}
			return nullptr;
		}
		vector<shared_ptr<Component>> GetComponents() {
			return m_Components;
		}

		vector<shared_ptr<RendererComponent>> GetRendererComponent();
		void ComponentUpdate();
	};
}
