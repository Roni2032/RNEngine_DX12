#pragma once
#include "stdafx.h"
#include "Component.h"
#include "Vector.h"
namespace RNEngine {
	class MoveComponent : public Component
	{
		float m_Speed;
		float m_JumpPower;

		array<string,5> m_MoveInputKeyVec;
	public:
		MoveComponent(const shared_ptr<GameObject>& ptr) 
			:Component(ptr), m_Speed(0.0f), m_JumpPower(0.0f),
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

		INSPECTOR_COMPONENT(MoveComponent)
		REGISTER_NAME(MoveComponent)
		BEGIN_REFLECT()
			REGISTER_REFLECT(m_Speed, FieldInfo::Type::Float, HeaderAttribute(u8"ˆÚ“®‘¬“x"))
			REGISTER_REFLECT(m_JumpPower, FieldInfo::Type::Float, HeaderAttribute(u8"ƒWƒƒƒ“ƒv—Í"))
		END_REFLECT()
	};
}