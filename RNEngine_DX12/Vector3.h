#pragma once

namespace RNEngine {
	struct Vector3 : public XMFLOAT3 {
	public:
		const static Vector3 Zero;
		const static Vector3 One;

		Vector3();
		Vector3(float value);
		Vector3(float x, float y, float z);
		Vector3(const Vector3& other);
		Vector3(const XMVECTOR& other);
		operator XMVECTOR() const;
		Vector3& operator=(const XMVECTOR& other);
		Vector3& operator=(const Vector3& other);
		Vector3 operator+(const Vector3& other);
		Vector3 operator+(const Vector2& other);
		Vector3 operator+=(const Vector3& other);
		Vector3 operator+=(const Vector2& other);
		Vector3 operator-(const Vector3& other);
		Vector3 operator-=(const Vector3& other);
		Vector3 operator*(const Vector3& other);
		Vector3 operator*(const float& other);
		Vector3 operator*=(const Vector3& other);
		Vector3 operator/(const Vector3& other);
		Vector3 operator/(const float& other);
		Vector3 operator/=(const Vector3& other);
		float& operator[](int index);
		const float& operator[](int index)const;
		bool operator==(const Vector3& other);
		bool operator!=(const Vector3& other);

		Vector3 Cross(const Vector3& other);
		float Dot(const Vector3& other);

		void Set(float x, float y, float z);
		void Set(float value);
		float Length()const;
		Vector3 Normalize();
		Vector3 Normalized()const;

		Vector3 Cross(const Vector3& other)const;

		void RandomVec(float min, float max);

		Vector3 Min(const Vector3& other)const;
		Vector3 Max(const Vector3& other)const;

		Vector2 XX();
		Vector2 XY();
		Vector2 XZ();

		Vector2 YX();
		Vector2 YY();
		Vector2 YZ();

		Vector2 ZX();
		Vector2 ZY();
		Vector2 ZZ();

		//Vector4 XYZW(float w = 0.0f) { return Vector4(x, y, z, w); }
	};
}