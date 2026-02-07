#pragma once
#include "stdafx.h"
#include "Object.h"
namespace RNEngine {
	class Component;
	class Scene;
	class RendererComponent;
	class Transform;

	class GameObject : public Object {
		string m_Name;
		weak_ptr<Scene> m_OwnerScene;
		shared_ptr<Transform> m_Transform;

		vector<shared_ptr<Component>> m_Components;
		vector<string> m_Tags;
	public:
		GameObject(const shared_ptr<Scene>& ptr) :m_OwnerScene(ptr),m_Name("GameObject"), Object() {}
		virtual ~GameObject() {}

		virtual void Start()override;
		virtual void Update()override;
		virtual void LastUpdate()override;
		virtual void Draw()override;

		void SetName(const string& name) {
			m_Name = name;
		}
		string GetName() {
			return m_Name;
		}
		void AddTag(const string& tag) {
			m_Tags.push_back(tag);
		}
		bool HasTag(const string& tag) {
			return find(m_Tags.begin(), m_Tags.end(), tag) != m_Tags.end();
		}
		bool HasTags(const vector<string>& tags) {
			for (auto& tag : tags) {
				if (HasTag(tag)) return true;
			}
			return false;
		}

		shared_ptr<Scene> GetOwnerScene()const;
		shared_ptr<Transform> GetTransform()const;

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
		template<class T>
		void RemoveComponent() {
			auto it = m_Components.begin();
			for (; it != m_Components.end(); it++) {
				if (dynamic_pointer_cast<T>(*it)) {
					m_Components.erase(it);
					break;
				}
			}
		}
		vector<shared_ptr<Component>> GetComponents() {
			return m_Components;
		}

		vector<shared_ptr<RendererComponent>> GetRendererComponent();
	};
}
