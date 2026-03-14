#pragma once

namespace RNEngine {
	struct Vector4 : public XMFLOAT4 {
	public:
		const static Vector4 Zero;
		const static Vector4 One;

		Vector4();
		Vector4(float value);
		Vector4(float x, float y, float z, float w);
		Vector4(const Vector4& other);

		Vector4(const XMVECTOR& other);
		operator XMVECTOR() const;
		Vector4& operator=(const XMVECTOR& other);
		Vector4& operator=(const Vector4& other);
		Vector4 operator+(const Vector4& other);
		Vector4 operator-(const Vector4& other);
		Vector4 operator*(const Vector4& other);
		Vector4 operator*(const float& other);
		Vector4 operator/(const Vector4& other);
		Vector4 operator/(const float& other);
		bool operator==(const Vector4& other);
		bool operator!=(const Vector4& other);

		void Set(float x, float y, float z, float w);
		void Set(float value);

		Vector4 Normalize();
		Vector4 Normalized()const;

		void RandomVec(float min, float max);

		Vector2 XX();
		Vector2 XY();
		Vector2 XZ();
		Vector2 XW();

		Vector2 YX();
		Vector2 YY();
		Vector2 YZ();
		Vector2 YW();

		Vector2 ZX();
		Vector2 ZY();
		Vector2 ZZ();
		Vector2 ZW();

		Vector2 WX();
		Vector2 WY();
		Vector2 WZ();
		Vector2 WW();

		Vector3 XYZ();
	};
}