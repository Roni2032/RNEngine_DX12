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

		void RandomVec(float min, float max);

		//Vector3 XYZ(float z = 0.0f) { return Vector3(x, y, z); }
		//Vector3 XZY(float z = 0.0f) { return Vector3(x, z, y); }
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

		void Set(float x, float y, float z);
		void Set(float value);

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

}

