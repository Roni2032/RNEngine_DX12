#pragma once
#include "stdafx.h"
namespace RNEngine {
	class GameObject;
	class Component;

	class ComponentRegistry {
		static auto& GetMap() {
			static unordered_map<string, function<void(shared_ptr<GameObject>)>> componentMap = {};
			return componentMap;
		}
	public:
		static void Register(const string& name, function<void(shared_ptr<GameObject>)> fuc);

		static void AddComponent(const string& name, shared_ptr<GameObject>& object);
	};
}
