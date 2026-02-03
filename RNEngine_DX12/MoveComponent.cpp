#include "stdafx.h"
#include "project.h"

namespace RNEngine {
	INSPECTOR_COMPONENT_CPP(MoveComponent)

	void MoveComponent::Update() {
		auto owner = GetOwner();
		auto transform = owner->GetTransform();
		float delta = Engine::GetFrameTimer()->GetDeltaTime();
		auto position = transform->GetPosition();

		Vector3 frontVec = { 0,0,1 };
		Vector3 rightVec = frontVec.Cross({ 0,-1,0 });

		if (Input::IsHeld(m_MoveInputKeyVec[0])) {
			position = position + frontVec * m_Speed * delta;
		}
		if (Input::IsHeld(m_MoveInputKeyVec[1])) {
			position = position - frontVec * m_Speed * delta;
		}
		if (Input::IsHeld(m_MoveInputKeyVec[2])) {
			position = position + rightVec * m_Speed * delta;
		}
		if (Input::IsHeld(m_MoveInputKeyVec[3])) {
			position = position - rightVec * m_Speed * delta;
		}
		if (Input::IsHeld(m_MoveInputKeyVec[4])) {
			auto rigidbody = owner->GetComponent<Rigidbody>();
			if (rigidbody) {
				rigidbody->StartJump({ 0,m_JumpPower,0 }, ForceMode::Impulse);
			}
		}

		transform->SetPosition(position);
	}
}
