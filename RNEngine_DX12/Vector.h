#pragma once
#include "stdafx.h"
namespace RNEngine {

	//---------------------------------------------------------------
	// 
	//	XY()ånìùÇÃä÷êîèëÇ≠ÇÃÇﬂÇÒÇ«Ç≠Ç≥Ç≠Ç»Ç¡ÇøÇ·Ç¡ÇΩÅöÇŸÇµÇ¢Ç∆Ç´Ç…í«â¡ÇµÇƒÉlÅö
	// 
	//---------------------------------------------------------------
	struct Vector2;
	struct Vector4;

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
		 bool operator==(const Vector3& other);
		 bool operator!=(const Vector3& other);

		 Vector3 Cross(const Vector3& other);
		 float Dot(const Vector3& other);

		 void Set(float x, float y, float z);
		 void Set(float value);
		 float Length();
		 Vector3 Normalize();
		 Vector3 Normalized()const;

		 Vector3 Cross(const Vector3& other)const;

		 void RandomVec(float min, float max);

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

		 Vector2 ZX() ;
		 Vector2 ZY() ;
		 Vector2 ZZ() ;
		 Vector2 ZW() ;

		 Vector2 WX() ;
		 Vector2 WY() ;
		 Vector2 WZ() ;
		 Vector2 WW() ;

		 Vector3 XYZ();
	};

	struct Quaternion : public XMFLOAT4{
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

	struct Color : public XMFLOAT4 {
		const static Color White;
		const static Color Red;
		const static Color Blue;
		const static Color Black;
		const static Color Green;

		 Color();
		 Color(float value);
		 Color(float x, float y, float z, float w);
		 Color(const Color& other);
		 Color(const array<float, 4>& other);

		 Color(const XMVECTOR& other);
		 operator XMVECTOR() const;
		 Color& operator=(const XMVECTOR& other);
		 Color& operator=(const Color& other);
		 Color operator+(const Color& other);
		 Color operator-(const Color& other);
		 Color operator*(const Color& other);
		 Color operator*(const float& other);
		 Color operator/(const Color& other);
		 Color operator/(const float& other);
		 bool operator==(const Color& other);
		 bool operator!=(const Color& other);

		 void Set(float x, float y, float z, float w);
		 void Set(float value);

		 void RandomVec(float min, float max);
	};



	struct AABB {
		Vector3 m_Max;
		Vector3 m_Min;
		Vector3 m_Center;

		AABB() :AABB(Vector3(), Vector3()) {}
		AABB(Vector3 max, Vector3 min) :m_Max(max), m_Min(min) {
			m_Center = (max + min) / 2.0f;
		}

		Vector3 GetSize() {
			return m_Max - m_Min;
		}
		Vector3 GetCenter() {
			return (m_Max + m_Min) / 2.0f;
		}
	};
}

