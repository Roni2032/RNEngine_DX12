#pragma once
#include "stdafx.h"
namespace RNEngine
{
	class GameObject;

	class Component : public Object
	{
		weak_ptr<GameObject> m_Owner;
	public:
		Component(const shared_ptr<GameObject>& ptr) :m_Owner(ptr), Object(){}
		virtual ~Component() {}

		shared_ptr<GameObject> GetOwner();
	};
	class Transform : public Component {
		XMFLOAT3 m_Position;
		XMFLOAT3 m_Scale;
		XMFLOAT3 m_Rotation;
	public:
		Transform(const shared_ptr<GameObject>& ptr) :Component(ptr), m_Position{}, m_Scale{1,1,1}, m_Rotation{} {}
		virtual ~Transform(){}

		void SetPosition(XMFLOAT3 position) {
			m_Position = position;
		}
		XMFLOAT3 GetPosition() {
			return m_Position;
		}
		void SetScale(XMFLOAT3 scale) {
			m_Scale = scale;
		}
		XMFLOAT3 GetScale() {
			return m_Scale;
		}
		void SetRotation(XMFLOAT3 rotation) {
			m_Rotation = rotation;
		}
		XMFLOAT3 GetRotation() {
			return m_Rotation;
		}
	};
}

