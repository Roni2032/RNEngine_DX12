#pragma once

namespace RNEngine {
	struct Vector2 : public XMFLOAT2 {
	public:
		const static Vector2 Zero;
		const static Vector2 One;

		Vector2();
		Vector2(float value);
		Vector2(float x, float y);
		Vector2(const Vector2& other);

		Vector2(const XMVECTOR& other);
		operator XMVECTOR() const;
		Vector2& operator=(const XMVECTOR& other);
		Vector2& operator=(const Vector2& other);
		Vector2 operator+(const Vector2& other);
		Vector2 operator-(const Vector2& other);
		Vector2 operator*(const Vector2& other);
		Vector2 operator*(const float& other);
		Vector2 operator/(const Vector2& other);
		Vector2 operator/(const float& other);
		bool operator==(const Vector2& other);
		bool operator!=(const Vector2& other);

		void Set(float x, float y);
		void Set(float value);
		float Length();
		Vector2 Normalize();
		Vector2 Normalized()const;

		void RandomVec(float min, float max);
	};
}
