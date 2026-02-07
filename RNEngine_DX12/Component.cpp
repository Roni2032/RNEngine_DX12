#include "stdafx.h"
#include "Component.h"

namespace RNEngine {
	shared_ptr<GameObject> Component::GetOwner()const {
		auto ptr = m_Owner.lock();
		return ptr;
	}


	Transform::Transform(const shared_ptr<GameObject>& ptr) :Component(ptr),
		m_Position(Vector3::Zero),
		m_Scale(Vector3::One),
		m_Quaternion(Quaternion::Identity) {}

	Transform::~Transform() {}

	Vector3 Transform::GetForward() {
		Vector3 forward = Vector3(0, 0, 1);
		Quaternion normalizedQuat = m_Quaternion.Normalized();
		return (Vector3)XMVector3Rotate(forward, normalizedQuat);
	}

	void Transform::LookAt(const Vector3& target) {
		Quaternion newQuat = Quaternion::Identity;
		Vector3 dist = target - m_Position;
		dist.Normalize();

		newQuat = newQuat.RotateToVector(dist);

		SetQuaternion(newQuat);
	}

	INSPECTOR_COMPONENT_CPP(Transform)
}