#pragma once
#include "Component.h"
namespace RNEngine {
	enum class ForceMode {
		Add, Impulse
	};
    class Rigidbody : public Component{
		Vector3 m_Gravity;
		Vector3 m_Velocity;
	public:
		Rigidbody(const shared_ptr<GameObject>& ptr) :Component(ptr), m_Gravity{} {}
		virtual ~Rigidbody() {}

		virtual void Update()override;

		void SetGravity(const Vector3& gravity) { m_Gravity = gravity; }
		Vector3 GetGravity()const { return m_Gravity; }

		void StartJump(const Vector3& jumpSpeed, ForceMode forceMode = ForceMode::Add);

		INSPECTOR_COMPONENT(Rigidbody)
			REGISTER_NAME(Rigidbody)
			BEGIN_REFLECT()
			REGISTER_REFLECT(m_Gravity.x, FieldInfo::Type::Vec3, HeaderAttribute(u8"èdóÕ"))
			END_REFLECT()
    };
}

