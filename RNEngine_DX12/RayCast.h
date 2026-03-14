#pragma once
#include "stdafx.h"

namespace RNEngine {
	struct Ray {
		Vector3 m_Origin;
		Vector3 m_Direction;
		float m_Length;

		Ray() : m_Origin(Vector3::Zero), m_Direction(Vector3::Zero), m_Length(0.0f) {}
		Ray(const Vector3& origin, const Vector3& direction, float length) : m_Origin(origin), m_Direction(direction.Normalized()), m_Length(length) {}
		Ray(const Vector3& origin, const Vector3& direction) : m_Origin(origin), m_Direction(Vector3::Zero), m_Length(0){
			m_Length = direction.Length();
			m_Direction = direction.Normalized();
		}
	};
	class RayCast{
		public:
			static bool Hit(const Ray& ray);
	};
}


