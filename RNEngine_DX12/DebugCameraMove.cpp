#include "stdafx.h"
#include "project.h"

namespace RNEngine {
	void DebugCameraMove::Start() {
		Input::RegisterInput("CanDebugCameraMove", VK_RBUTTON, InputMode::Keyboard);
	}
	void DebugCameraMove::Update() {

		if (!Input::IsHeld("CanDebugCameraMove")) return;

		auto owner = GetOwner();
		auto transform = owner->GetTransform();
		Vector3 forward = transform->GetForward();

		//外積用の上方向ベクトル
		Vector3 up = Vector3(0, -1, 0);
		//縦の時は外積をZ軸で処理
		if (fabsf(forward.Dot(Vector3(0, 1, 0))) > 0.999f) {
			up = Vector3(0, 0, 1);
		}
		Vector3 right = forward.Cross(up);
		//up = forward.Dot(right);

		float delta = Engine::GetFrameTimer()->GetDeltaTime();

		Vector2 mouseDelta = Input::GetMouseOffset();

		Quaternion quat = transform->GetQuaternion();
		quat.RotateY(mouseDelta.x * delta * m_MouseSensitive);
		quat.Rotate(right, mouseDelta.y * delta * m_MouseSensitive);
		transform->SetQuaternion(quat);
	}
	INSPECTOR_COMPONENT_CPP(DebugCameraMove)
}
