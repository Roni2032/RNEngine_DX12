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

		shared_ptr<GameObject> GetOwner()const;

		virtual void OnCollisionEnter(const shared_ptr<GameObject>& object){}
		virtual void OnCollisionExecute(const shared_ptr<GameObject>& object){}
		virtual void OnCollisionExit(const shared_ptr<GameObject>& object){}

		REGISTER_NAME(Component)
		BEGIN_REFLECT()
		END_REFLECT()
	};
	class Transform : public Component {
		Vector3 m_Position;
		Vector3 m_Scale;
		Quaternion m_Quaternion;
	public:
		Transform(const shared_ptr<GameObject>& ptr);
		virtual ~Transform();

		void SetPosition(Vector3 position) {
			m_Position = position;
		}
		Vector3 GetPosition() const{
			return m_Position;
		}
		void SetScale(Vector3 scale) {
			m_Scale = scale;
		}
		Vector3 GetScale() const{
			return m_Scale;
		}
		void SetRotation(Vector3 rotation) {
			m_Quaternion = Quaternion(rotation).Normalize();
		}
		Vector3 GetRotation() const{
			return m_Quaternion.ConvertToRollPitchYaw();
		}
		Quaternion GetQuaternion() {
			return m_Quaternion;
		}
		void SetQuaternion(Quaternion quaternion) {
			m_Quaternion = quaternion;
		}

		Vector3 GetForward();

		void LookAt(const Vector3& target);

		INSPECTOR_COMPONENT(Transform)
		REGISTER_NAME(Transform)
		BEGIN_REFLECT()
			REGISTER_REFLECT(m_Position.x, FieldInfo::Type::Vec3, HeaderAttribute(L"Position"))
			REGISTER_REFLECT(m_Quaternion.x, FieldInfo::Type::Vec3, HeaderAttribute(L"Rotation"),
				ConvertToAttribute(
					[](void* internalPtr, void* displayPtr) {
						float* displayF = reinterpret_cast<float*>(displayPtr);
						float* internalF = reinterpret_cast<float*>(internalPtr);
						Vector3 rot = Quaternion(internalF[0],internalF[1],internalF[2],internalF[3]).ConvertToRollPitchYaw();

						displayF[0] = XMConvertToDegrees(rot.x);
						displayF[1] = XMConvertToDegrees(rot.y);
						displayF[2] = XMConvertToDegrees(rot.z);
					},
					[](void* displayPtr, void* internalPtr) {
						float* displayF = reinterpret_cast<float*>(displayPtr);
						float* internalF = reinterpret_cast<float*>(internalPtr);
						Quaternion quat = Quaternion(Vector3(
							XMConvertToRadians(displayF[0]),
							XMConvertToRadians(displayF[1]),
							XMConvertToRadians(displayF[2])));
						quat.Normalize();

						internalF[0] = quat.x;
						internalF[1] = quat.y;
						internalF[2] = quat.z;
						internalF[3] = quat.w;
					}))
			REGISTER_REFLECT(m_Scale.x, FieldInfo::Type::Vec3, HeaderAttribute(L"Scale   "))
		END_REFLECT()
	};
}

