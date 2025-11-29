#include "stdafx.h"
#include "Component.h"

namespace RNEngine {
	shared_ptr<GameObject> Component::GetOwner() {
		auto ptr = m_Owner.lock();
		return ptr;
	}

	INSPECTOR_COMPONENT_CPP(Transform)
}