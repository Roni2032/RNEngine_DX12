#pragma once
#include "stdafx.h"

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Quaternion.h"
#include "Color.h"

namespace RNEngine {

	struct AABB {
		Vector3 m_Max;
		Vector3 m_Min;

		AABB() :AABB(Vector3(), Vector3()) {}
		AABB(Vector3 max, Vector3 min) :m_Max(max), m_Min(min) {}

		Vector3 GetSize() {
			return m_Max - m_Min;
		}
		Vector3 GetCenter() {
			return (m_Max + m_Min) / 2.0f;
		}

		//中心位置を変えずサイズを変更する
		AABB operator*(const Vector3& vec) {
			Vector3 center = GetCenter();
			Vector3 halfSize = GetSize() * vec * 0.5f;

			return AABB(center + halfSize, center - halfSize);
		}
		//サイズを変えずに位置を変更する
		AABB operator+(const Vector3& vec) {
			return AABB(m_Max + vec, m_Min + vec);
		}
		AABB operator-(const Vector3& vec) {
			return AABB(m_Max - vec, m_Min - vec);
		};
	};
}
