#include "stdafx.h"
#include "project.h"
namespace RNEngine {
	void Rigidbody::Update() {
		float delta = Engine::GetFrameTimer()->GetDeltaTime();

		auto owner = GetOwner();
		m_Velocity = m_Velocity + m_Gravity * delta;
		auto transform = owner->GetTransform();
		if (!transform)return;

		Vector3 position = transform->GetPosition();
		position = position + m_Velocity * delta;
		transform->SetPosition(position);
	}
	void Rigidbody::StartJump(const Vector3& jumpSpeed, ForceMode forceMode) {
		if (forceMode == ForceMode::Add) {
			m_Velocity = m_Velocity + jumpSpeed;
		}
		else if (forceMode == ForceMode::Impulse) {
			m_Velocity = jumpSpeed;
		}
	}
}