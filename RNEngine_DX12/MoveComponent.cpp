#include "stdafx.h"
#include "project.h"

namespace RNEngine {
	INSPECTOR_COMPONENT_CPP(MoveComponent)

	void MoveComponent::Update() {
		//基礎情報
		auto owner = GetOwner();
		auto transform = owner->GetTransform();
		float delta = Engine::GetFrameTimer()->GetDeltaTime();
		auto position = transform->GetPosition();

		//正面、右方向ベクトル
		Vector3 frontVec, rightVec;
		ApplyObjectForward(frontVec, rightVec);

		//現在の移動量
		float currentSpeed = m_Speed * delta;

		//四方移動
		if (Input::IsHeld(m_MoveInputKeyVec[0])) {
			position = position + frontVec * currentSpeed;
		}
		if (Input::IsHeld(m_MoveInputKeyVec[1])) {
			position = position - frontVec * currentSpeed;
		}
		if (Input::IsHeld(m_MoveInputKeyVec[2])) {
			position = position + rightVec * currentSpeed;
		}
		if (Input::IsHeld(m_MoveInputKeyVec[3])) {
			position = position - rightVec * currentSpeed;
		}
		//ジャンプ(Rigidbodyありの時のみ)
		if (Input::IsPressed(m_MoveInputKeyVec[4])) {
			auto rigidbody = owner->GetComponent<Rigidbody>();
			if (rigidbody) {
				rigidbody->StartJump({ 0,m_JumpPower,0 }, ForceMode::Impulse);
			}
		}

		transform->SetPosition(position);
	}

	void MoveComponent::ApplyObjectForward(Vector3& front, Vector3& right) {
		auto owner = GetOwner();
		//外積用の上方向ベクトル
		Vector3 up = Vector3(0, -1, 0);
		if (!m_IsApplyObjectForward) {
			front = Vector3(0, 0, 1);
		}
		else {
			front = owner->GetTransform()->GetForward();
			//縦の時は外積をZ軸で処理
			if (fabsf(front.Dot(Vector3(0, 1, 0))) > 0.999f) {
				up = Vector3(0, 0, 1);
			}
		}
		right = front.Cross(up);

		//適用しない軸は0に固定
		if (!m_IsApplyForwardAxis[0]) {
			front.x = 0.0f;
			right.x = 0.0f;
		}
		if (!m_IsApplyForwardAxis[1]) {
			front.y = 0.0f;
			right.y = 0.0f;
		}
		if (!m_IsApplyForwardAxis[2]) {
			front.z = 0.0f;
			right.z = 0.0f;
		}
	}
}
