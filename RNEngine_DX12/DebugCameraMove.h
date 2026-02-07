#pragma once
#include "stdafx.h"
#include "Component.h"

namespace RNEngine {
	class DebugCameraMove : public Component
	{
		float m_MouseSensitive;

		void ApplyObjectForward(Vector3& front, Vector3& right);
	public:
		DebugCameraMove(const shared_ptr<GameObject>& ptr)
			:Component(ptr),m_MouseSensitive(0.1f) { }
		virtual ~DebugCameraMove() {}

		virtual void Start()override;
		virtual void Update()override;

		INSPECTOR_COMPONENT(DebugCameraMove)
		REGISTER_NAME(DebugCameraMove)
		BEGIN_REFLECT()
			REGISTER_REFLECT(m_MouseSensitive, FieldInfo::Type::Float, HeaderAttribute(u8"Š´“x"))
		END_REFLECT()
	};
}