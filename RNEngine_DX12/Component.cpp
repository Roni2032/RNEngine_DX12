#include "stdafx.h"
#include "Component.h"

namespace RNEngine {
	shared_ptr<GameObject> Component::GetOwner() {
		auto ptr = m_Owner.lock();
		return ptr;
	}


	Transform::Transform(const shared_ptr<GameObject>& ptr) :Component(ptr),
		m_Position(Vector3::Zero),
		m_Scale(Vector3::One),
		//m_Rotation(Vector3::Zero),
		m_Quaternion(Quaternion::Identity) {}

	Transform::~Transform() {}

	INSPECTOR_COMPONENT_CPP(Transform)
}