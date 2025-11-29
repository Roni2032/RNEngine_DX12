#pragma once
#include "stdafx.h"
namespace RNEngine {

	//---------------------------------------------------------------
	// 
	//	XY()Œn“‚ÌŠÖ”‘‚­‚Ì‚ß‚ñ‚Ç‚­‚³‚­‚È‚Á‚¿‚á‚Á‚½š‚Ù‚µ‚¢‚Æ‚«‚É’Ç‰Á‚µ‚Äƒlš
	// 
	//---------------------------------------------------------------
	struct Vector2;
	struct Vector4;

	struct Vector2 : public XMFLOAT2{
	public:
		const static Vector2 Zero;
		const static Vector2 One;

		Vector2() = default;
		Vector2(float value) : XMFLOAT2(value,value){}
		Vector2(float x,float y) : XMFLOAT2(x,y){}
		Vector2(const Vector2& other) : XMFLOAT2(other.x,other.y){}

		Vector2(const XMVECTOR& other) : XMFLOAT2() {
			XMVECTOR temp = other;
			XMStoreFloat2((XMFLOAT2*)this, temp);
		}
		operator XMVECTOR() const {
			XMFLOAT2 temp = *this;
			return XMLoadFloat2(&temp);
		}
		Vector2& operator=(const XMVECTOR& other) {
			XMStoreFloat2((XMFLOAT2*)this, other);
			return *this;
		}
		Vector2& operator=(const Vector2& other) {
			this->x = other.x;
			this->y = other.y;
			return *this;
		}
		Vector2 operator+(const Vector2& other) {
			return (Vector2)XMVectorAdd(*this, other);
		}
		Vector2 operator-(const Vector2& other) {
			return (Vector2)XMVectorSubtract(*this, other);
		}
		Vector2 operator*(const Vector2& other) {
			return (Vector2)XMVectorMultiply(*this, other);
		}
		Vector2 operator*(const float& other) {
			Vector2 temp = { other };
			return (Vector2)XMVectorMultiply(*this, temp);
		}
		Vector2 operator/(const Vector2& other) {
			return (Vector2)XMVectorDivide(*this, other);
		}
		Vector2 operator/(const float& other) {
			Vector2 temp = { other };
			return (Vector2)XMVectorDivide(*this, temp);
		}
		bool operator==(const Vector2& other) {
			return XMVector3Equal(*this, other);
		}
		bool operator!=(const Vector2& other) {
			return !XMVector3Equal(*this, other);
		}
		
		void Set(float x, float y) {
			this->x = x;
			this->y = y;
		}
		void Set(float value) {
			this->x = this->y = value;
		}

		void RandomVec(float min,float max) {
			Set(Util::Random<float>(min, max), Util::Random<float>(min, max));
		}

		//Vector3 XYZ(float z = 0.0f) { return Vector3(x, y, z); }
		//Vector3 XZY(float z = 0.0f) { return Vector3(x, z, y); }
	};
	struct Vector3 : public XMFLOAT3 {
	public:
		const static Vector3 Zero;
		const static Vector3 One;

		Vector3():XMFLOAT3(0,0,0){}
		Vector3(float value) : XMFLOAT3(value, value, value) {}
		Vector3(float x, float y, float z) : XMFLOAT3(x, y, z) {}
		Vector3(const Vector3& other) : XMFLOAT3(other.x, other.y, other.z) {}
		Vector3(const XMVECTOR& other) : XMFLOAT3() {
			XMVECTOR temp = other;
			XMStoreFloat3((XMFLOAT3*)this, temp);
		}
		operator XMVECTOR() const {
			XMFLOAT3 temp = *this;
			XMVECTOR Vec = XMLoadFloat3(&temp);
			return Vec;
		}
		Vector3& operator=(const XMVECTOR& other) {
			XMStoreFloat3((XMFLOAT3*)this, other);
			return *this;
		}
		Vector3& operator=(const Vector3& other) {
			this->x = other.x;
			this->y = other.y;
			this->z = other.z;
			return *this;
		}
		Vector3 operator+(const Vector3& other) {
			return (Vector3)XMVectorAdd(*this, other);
		}
		Vector3 operator-(const Vector3& other) {
			return (Vector3)XMVectorSubtract(*this, other);
		}
		Vector3 operator*(const Vector3& other) {
			return (Vector3)XMVectorMultiply(*this, other);
		}
		Vector3 operator*(const float& other) {
			Vector3 temp = { other };
			return (Vector3)XMVectorMultiply(*this, temp);
		}
		Vector3 operator/(const Vector3& other) {
			return (Vector3)XMVectorDivide(*this, other);
		}
		Vector3 operator/(const float& other) {
			Vector3 temp = { other };
			return (Vector3)XMVectorDivide(*this, temp);
		}
		bool operator==(const Vector3& other) {
			return XMVector3Equal(*this, other);
		}
		bool operator!=(const Vector3& other) {
			return !XMVector3Equal(*this, other);
		}

		void Set(float x, float y, float z) {
			this->x = x;
			this->y = y;
			this->z = z;
		}
		void Set(float value) {
			this->x = this->y = this->z = value;
		}

		void RandomVec(float min, float max) {
			Set(Util::Random<float>(min, max), Util::Random<float>(min, max), Util::Random<float>(min, max));
		}

		Vector2 XX() { return Vector2(x, x); }
		Vector2 XY() { return Vector2(x, y); }
		Vector2 XZ() { return Vector2(x, z); }

		Vector2 YX() { return Vector2(y, x); }
		Vector2 YY() { return Vector2(y, y); }
		Vector2 YZ() { return Vector2(y, z); }

		Vector2 ZX() { return Vector2(z, x); }
		Vector2 ZY() { return Vector2(z, y); }
		Vector2 ZZ() { return Vector2(z, z); }

		//Vector4 XYZW(float w = 0.0f) { return Vector4(x, y, z, w); }
	};
	struct Vector4 : public XMFLOAT4 {
	public:
		const static Vector4 Zero;
		const static Vector4 One;

		Vector4() = default;
		Vector4(float value) : XMFLOAT4(value, value, value,value) {}
		Vector4(float x, float y, float z,float w) : XMFLOAT4(x, y, z,w) {}
		Vector4(const Vector4& other) : XMFLOAT4(other.x, other.y, other.z,other.w) {}

		Vector4(const XMVECTOR& other) : XMFLOAT4() {
			XMVECTOR temp = other;
			XMStoreFloat4((XMFLOAT4*)this, temp);
		}
		operator XMVECTOR() const {
			XMFLOAT4 temp = *this;
			XMVECTOR Vec = XMLoadFloat4(&temp);
			return Vec;
		}
		Vector4& operator=(const XMVECTOR& other) {
			XMStoreFloat4((XMFLOAT4*)this, other);
			return *this;
		}
		Vector4& operator=(const Vector4& other) {
			this->x = other.x;
			this->y = other.y;
			this->z = other.z;
			this->w = other.w;
			return *this;
		}
		Vector4 operator+(const Vector4& other) {
			return (Vector4)XMVectorAdd(*this, other);
		}
		Vector4 operator-(const Vector4& other) {
			return (Vector4)XMVectorSubtract(*this, other);
		}
		Vector4 operator*(const Vector4& other) {
			return (Vector4)XMVectorMultiply(*this, other);
		}
		Vector4 operator*(const float& other) {
			Vector4 temp = { other };
			return (Vector4)XMVectorMultiply(*this, temp);
		}
		Vector4 operator/(const Vector4& other) {
			return (Vector4)XMVectorDivide(*this, other);
		}
		Vector4 operator/(const float& other) {
			Vector4 temp = { other };
			return (Vector4)XMVectorDivide(*this, temp);
		}
		bool operator==(const Vector4& other) {
			return XMVector3Equal(*this, other);
		}
		bool operator!=(const Vector4& other) {
			return !XMVector3Equal(*this, other);
		}

		void Set(float x, float y, float z,float w) {
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;
		}
		void Set(float value) {
			this->x = this->y = this->z = this->w = value;
		}

		void RandomVec(float min, float max) {
			Set(Util::Random<float>(min, max), Util::Random<float>(min, max), Util::Random<float>(min, max), Util::Random<float>(min, max));
		}

		Vector2 XX() { return Vector2(x, x); }
		Vector2 XY() { return Vector2(x, y); }
		Vector2 XZ() { return Vector2(x, z); }
		Vector2 XW() { return Vector2(x, w); }

		Vector2 YX() { return Vector2(y, x); }
		Vector2 YY() { return Vector2(y, y); }
		Vector2 YZ() { return Vector2(y, z); }
		Vector2 YW() { return Vector2(y, w); }

		Vector2 ZX() { return Vector2(z, x); }
		Vector2 ZY() { return Vector2(z, y); }
		Vector2 ZZ() { return Vector2(z, z); }
		Vector2 ZW() { return Vector2(z, w); }

		Vector2 WX() { return Vector2(w, x); }
		Vector2 WY() { return Vector2(w, y); }
		Vector2 WZ() { return Vector2(w, z); }
		Vector2 WW() { return Vector2(w, w); }

		Vector3 XYZ() { return Vector3(x, y, z); }
	};

}

