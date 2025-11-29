#pragma once
#include "stdafx.h"
#include "Component.h"
#include "Vector.h"
namespace RNEngine {
	class MoveComponent : public Component
	{
		Vector3 m_Velocity;
	public:
		MoveComponent(const shared_ptr<GameObject>& ptr) :Component(ptr), m_Velocity{} {}
		virtual ~MoveComponent() {}

		virtual void Update()override;

		INSPECTOR_COMPONENT(MoveComponent)
		REGISTER_NAME(MoveComponent)
		BEGIN_REFLECT()
			REGISTER_REFLECT(m_Velocity.x, FieldInfo::Type::Vec3, HeaderAttribute(u8"ˆÚ“®—Ê"))
		END_REFLECT()
	};
}