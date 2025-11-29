#include "stdafx.h"
#include "project.h"

namespace RNEngine {
	void ComponentRegistry::Register(const string& name, function<void(shared_ptr<GameObject>)> fuc) {
		auto& map = GetMap();
		auto it = map.find(name);
		if (it != map.end()) {
			return;
		}
		map[name] = fuc;
	}
	void ComponentRegistry::AddComponent(const string& name, shared_ptr<GameObject>& object) {
		auto& map = GetMap();

		string compName = name;
		size_t pos = name.find_last_of(".");
		if (pos != string::npos) {
			compName = compName.substr(0, pos);
		}

		auto it = map.find(compName);
		if (it != map.end()) {
			(*it).second(object);
		}
	}
}