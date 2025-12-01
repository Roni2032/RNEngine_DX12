#pragma once
#include "stdafx.h"
#include "Reflection.h"
namespace RNEngine
{
	class GameObject;

	class Component : public Object,public ReflectInterface
	{
		weak_ptr<GameObject> m_Owner;
	public:
		Component(const shared_ptr<GameObject>& ptr) :m_Owner(ptr), Object(){}
		virtual ~Component() {}

		shared_ptr<GameObject> GetOwner();

		REGISTER_NAME(Component)
		BEGIN_REFLECT()
		END_REFLECT()
	};
	class Transform : public Component {
		Vector3 m_Position;
		Vector3 m_Scale;
		Vector3 m_Rotation;
	public:
		Transform(const shared_ptr<GameObject>& ptr) :Component(ptr), m_Position{}, m_Scale{1,1,1}, m_Rotation{} {}
		virtual ~Transform(){}

		void SetPosition(Vector3 position) {
			m_Position = position;
		}
		Vector3 GetPosition() {
			return m_Position;
		}
		void SetScale(Vector3 scale) {
			m_Scale = scale;
		}
		Vector3 GetScale() {
			return m_Scale;
		}
		void SetRotation(Vector3 rotation) {
			m_Rotation = rotation;
		}
		Vector3 GetRotation() {
			return m_Rotation;
		}

		INSPECTOR_COMPONENT(Transform)
		REGISTER_NAME(Transform)
		BEGIN_REFLECT()
			REGISTER_REFLECT(m_Position.x, FieldInfo::Type::Vec3, HeaderAttribute("position"))
			REGISTER_REFLECT(m_Rotation.x, FieldInfo::Type::Vec3, HeaderAttribute("rotation"),
				ConvertToAttribute(
					[](void* internalPtr, void* displayPtr) {
						float* displayF = reinterpret_cast<float*>(displayPtr);
						float* internalF = reinterpret_cast<float*>(internalPtr);
						displayF[0] = XMConvertToDegrees(internalF[0]);
						displayF[1] = XMConvertToDegrees(internalF[1]);
						displayF[2] = XMConvertToDegrees(internalF[2]);
					},
					[](void* displayPtr, void* internalPtr){
						float* displayF = reinterpret_cast<float*>(displayPtr);
						float* internalF = reinterpret_cast<float*>(internalPtr);
						internalF[0] = XMConvertToRadians(displayF[0]);
						internalF[1] = XMConvertToRadians(displayF[1]);
						internalF[2] = XMConvertToRadians(displayF[2]);
					}))
			REGISTER_REFLECT(m_Scale.x, FieldInfo::Type::Vec3, HeaderAttribute("scale   "))
		END_REFLECT()
	};
}

