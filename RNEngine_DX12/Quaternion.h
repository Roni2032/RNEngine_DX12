#pragma once

namespace RNEngine {
	struct Quaternion : public XMFLOAT4 {
		const static Quaternion Identity;
		Quaternion();
		Quaternion(float x, float y, float z, float w);
		Quaternion(const Quaternion& other);
		Quaternion(const Vector3& other);

		Quaternion(const XMVECTOR& other);
		operator XMVECTOR() const;
		Quaternion& operator=(const XMVECTOR& other);
		Quaternion& operator=(const Quaternion& other);
		Quaternion operator*(const Quaternion& other);
		bool operator==(const Quaternion& other);
		bool operator!=(const Quaternion& other);

		void Set(float x, float y, float z, float w);

		Quaternion Normalize();
		Quaternion Normalized()const;

		void RotateX(float angle);
		void RotateY(float angle);
		void RotateZ(float angle);
		void Rotate(const Vector3& axis, float angle);

		Quaternion RotateToVector(const Vector3& direction)const;

		Vector3 ConvertToRollPitchYaw()const;
	};
}