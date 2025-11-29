#include "stdafx.h"
#include "project.h"

namespace RNEngine {
	INSPECTOR_COMPONENT_CPP(MoveComponent)

	void MoveComponent::Update() {
		auto owner = GetOwner();
		auto transform = owner->GetComponent<Transform>();
		auto position = transform->GetPosition();
		position.x += m_Velocity.x;
		position.y += m_Velocity.y;
		position.z += m_Velocity.z;
		transform->SetPosition(position);
	}
}
