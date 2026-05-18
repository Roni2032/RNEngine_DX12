#pragma once
#include "stdafx.h"
#include "Component.h"

namespace RNEngine {
	class MoveComponent : public Component
	{
		float m_Speed;
		float m_JumpPower;
		bool m_IsApplyObjectForward;
		array<bool, 3> m_IsApplyForwardAxis;

		array<string,5> m_MoveInputKeyVec;

		void ApplyObjectForward(Vector3& front, Vector3& right);
	public:
		MoveComponent(const shared_ptr<GameObject>& ptr) 
			:Component(ptr), m_Speed(0.0f), m_JumpPower(0.0f), 
			m_IsApplyObjectForward(false), m_IsApplyForwardAxis({true,true,true}),
			m_MoveInputKeyVec{"","","","",""} { }
		virtual ~MoveComponent() {}

		virtual void Update()override;

		void SetSpeed(float speed) {
			m_Speed = speed;
		}
		void SetJumpPower(float power) {
			m_JumpPower = power;
		}
		void SetMoveKeys(const string& front, const string& back, const string& right, const string& left) {
			m_MoveInputKeyVec[0] = front;
			m_MoveInputKeyVec[1] = back;
			m_MoveInputKeyVec[2] = right;
			m_MoveInputKeyVec[3] = left;
		}
		void SetJumpKey(const string& jump) {
			m_MoveInputKeyVec[4] = jump;
		}


		void SetApplyObjectForward(bool flag) {
			m_IsApplyObjectForward = flag;
		}
		void SetApplyForwardAxisX(bool flag) {
			m_IsApplyForwardAxis[0] = flag;
		}
		void SetApplyForwardAxisY(bool flag) {
			m_IsApplyForwardAxis[1] = flag;
		}
		void SetApplyForwardAxisZ(bool flag) {
			m_IsApplyForwardAxis[2] = flag;
		}

		INSPECTOR_COMPONENT(MoveComponent)
		REGISTER_NAME(MoveComponent)
		BEGIN_REFLECT()
			REGISTER_REFLECT(m_Speed, FieldInfo::Type::Float, HeaderAttribute(L"ˆÚ“®‘¬“x"))
			REGISTER_REFLECT(m_JumpPower, FieldInfo::Type::Float, HeaderAttribute(L"ƒWƒƒƒ“ƒv—Í"))
		END_REFLECT()
	};
}