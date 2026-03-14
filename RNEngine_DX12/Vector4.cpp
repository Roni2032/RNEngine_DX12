#include "stdafx.h"
#include "Vector4.h"

namespace RNEngine {
	const Vector4 Vector4::Zero = Vector4(0, 0, 0, 0);
	const Vector4 Vector4::One = Vector4(1, 1, 1, 1);


	Vector4::Vector4() = default;
	Vector4::Vector4(float value) : XMFLOAT4(value, value, value, value) {}
	Vector4::Vector4(float x, float y, float z, float w) : XMFLOAT4(x, y, z, w) {}
	Vector4::Vector4(const Vector4& other) : XMFLOAT4(other.x, other.y, other.z, other.w) {}

	Vector4::Vector4(const XMVECTOR& other) : XMFLOAT4() {
		XMVECTOR temp = other;
		XMStoreFloat4((XMFLOAT4*)this, temp);
	}
	Vector4::operator XMVECTOR() const {
		XMFLOAT4 temp = *this;
		XMVECTOR Vec = XMLoadFloat4(&temp);
		return Vec;
	}
	Vector4& Vector4::operator=(const XMVECTOR& other) {
		XMStoreFloat4((XMFLOAT4*)this, other);
		return *this;
	}
	Vector4& Vector4::operator=(const Vector4& other) {
		this->x = other.x;
		this->y = other.y;
		this->z = other.z;
		this->w = other.w;
		return *this;
	}
	Vector4 Vector4::operator+(const Vector4& other) {
		return (Vector4)XMVectorAdd(*this, other);
	}
	Vector4 Vector4::operator-(const Vector4& other) {
		return (Vector4)XMVectorSubtract(*this, other);
	}
	Vector4 Vector4::operator*(const Vector4& other) {
		return (Vector4)XMVectorMultiply(*this, other);
	}
	Vector4 Vector4::operator*(const float& other) {
		Vector4 temp = { other };
		return (Vector4)XMVectorMultiply(*this, temp);
	}
	Vector4 Vector4::operator/(const Vector4& other) {
		return (Vector4)XMVectorDivide(*this, other);
	}
	Vector4 Vector4::operator/(const float& other) {
		Vector4 temp = { other };
		return (Vector4)XMVectorDivide(*this, temp);
	}
	bool Vector4::operator==(const Vector4& other) {
		return XMVector4Equal(*this, other);
	}
	bool Vector4::operator!=(const Vector4& other) {
		return !XMVector4Equal(*this, other);
	}

	void Vector4::Set(float x, float y, float z, float w) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}
	void Vector4::Set(float value) {
		this->x = this->y = this->z = this->w = value;
	}

	Vector4 Vector4::Normalize() {
		*this = (Vector4)XMVector4Normalize(*this);
		return *this;
	}
	Vector4 Vector4::Normalized()const {
		return (Vector4)XMVector4Normalize(*this);
	}

	void Vector4::RandomVec(float min, float max) {
		Set(Util::Random<float>(min, max), Util::Random<float>(min, max), Util::Random<float>(min, max), Util::Random<float>(min, max));
	}

	Vector2 Vector4::XX() { return Vector2(x, x); }
	Vector2 Vector4::XY() { return Vector2(x, y); }
	Vector2 Vector4::XZ() { return Vector2(x, z); }
	Vector2 Vector4::XW() { return Vector2(x, w); }

	Vector2 Vector4::YX() { return Vector2(y, x); }
	Vector2 Vector4::YY() { return Vector2(y, y); }
	Vector2 Vector4::YZ() { return Vector2(y, z); }
	Vector2 Vector4::YW() { return Vector2(y, w); }

	Vector2 Vector4::ZX() { return Vector2(z, x); }
	Vector2 Vector4::ZY() { return Vector2(z, y); }
	Vector2 Vector4::ZZ() { return Vector2(z, z); }
	Vector2 Vector4::ZW() { return Vector2(z, w); }

	Vector2 Vector4::WX() { return Vector2(w, x); }
	Vector2 Vector4::WY() { return Vector2(w, y); }
	Vector2 Vector4::WZ() { return Vector2(w, z); }
	Vector2 Vector4::WW() { return Vector2(w, w); }

	Vector3 Vector4::XYZ() { return Vector3(x, y, z); }
}