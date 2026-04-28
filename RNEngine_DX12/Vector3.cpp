#include "stdafx.h"
#include "Vector3.h"

namespace RNEngine {
	const Vector3 Vector3::Zero = Vector3(0, 0, 0);
	const Vector3 Vector3::One = Vector3(1, 1, 1);


	Vector3::Vector3() :XMFLOAT3(0, 0, 0) {}
	Vector3::Vector3(float value) : XMFLOAT3(value, value, value) {}
	Vector3::Vector3(float x, float y, float z) : XMFLOAT3(x, y, z) {}
	Vector3::Vector3(const Vector3& other) : XMFLOAT3(other.x, other.y, other.z) {}
	Vector3::Vector3(const XMVECTOR& other) : XMFLOAT3() {
		XMVECTOR temp = other;
		XMStoreFloat3((XMFLOAT3*)this, temp);
	}
	Vector3::operator XMVECTOR() const {
		XMFLOAT3 temp = *this;
		XMVECTOR Vec = XMLoadFloat3(&temp);
		return Vec;
	}
	Vector3& Vector3::operator=(const XMVECTOR& other) {
		XMStoreFloat3((XMFLOAT3*)this, other);
		return *this;
	}
	Vector3& Vector3::operator=(const Vector3& other) {
		this->x = other.x;
		this->y = other.y;
		this->z = other.z;
		return *this;
	}
	Vector3 Vector3::operator+(const Vector3& other) {
		return (Vector3)XMVectorAdd(*this, other);
	}
	Vector3 Vector3::operator+(const Vector2& other) {
		return (Vector3)XMVectorAdd(*this, other);
	}
	Vector3 Vector3::operator+=(const Vector3& other) {
		*this = (Vector3)XMVectorAdd(*this, other);
		return *this;
	}
	Vector3 Vector3::operator+=(const Vector2& other) {
		*this = (Vector3)XMVectorAdd(*this, other);
		return *this;
	}
	Vector3 Vector3::operator-(const Vector3& other) {
		return (Vector3)XMVectorSubtract(*this, other);
	}
	Vector3 Vector3::operator-=(const Vector3& other) {
		*this = (Vector3)XMVectorSubtract(*this, other);
		return *this;
	}
	Vector3 Vector3::operator*(const Vector3& other) {
		return (Vector3)XMVectorMultiply(*this, other);
	}
	Vector3 Vector3::operator*(const float& other) {
		Vector3 temp = { other };
		return (Vector3)XMVectorMultiply(*this, temp);
	}
	Vector3 Vector3::operator*=(const Vector3& other) {
		*this = (Vector3)XMVectorMultiply(*this, other);
		return *this;
	}
	Vector3 Vector3::operator/(const Vector3& other) {
		return (Vector3)XMVectorDivide(*this, other);
	}
	Vector3 Vector3::operator/(const float& other) {
		Vector3 temp = { other };
		return (Vector3)XMVectorDivide(*this, temp);
	}
	Vector3 Vector3::operator/=(const Vector3& other) {
		*this = (Vector3)XMVectorDivide(*this, other);
		return *this;
	}
	float& Vector3::operator[](int index) {
		switch (index) {
		case 0: return x;
		case 1: return y;
		case 2: return z;
		default: throw out_of_range("out of range from Vector3");
		}
	}
	const float& Vector3::operator[](int index)const {
		switch (index) {
		case 0: return x;
		case 1: return y;
		case 2: return z;
		default: throw out_of_range("out of range from Vector3");
		}
	}
	bool Vector3::operator==(const Vector3& other) {
		return XMVector3Equal(*this, other);
	}
	bool Vector3::operator!=(const Vector3& other) {
		return !XMVector3Equal(*this, other);
	}

	Vector3 Vector3::Cross(const Vector3& other) {
		return (Vector3)XMVector3Cross(*this, other);
	}
	float Vector3::Dot(const Vector3& other) {
		return XMVectorGetX(XMVector3Dot(*this, other));
	}

	void Vector3::Set(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	void Vector3::Set(float value) {
		this->x = this->y = this->z = value;
	}
	float Vector3::Length() const {
		return XMVectorGetX(XMVector3Length(*this));
	}
	Vector3 Vector3::Normalize() {
		*this = (Vector3)XMVector3Normalize(*this);
		return *this;
	}
	Vector3 Vector3::Normalized()const {
		return (Vector3)XMVector3Normalize(*this);
	}
	Vector3 Vector3::Cross(const Vector3& other)const {
		return (Vector3)XMVector3Cross(*this, other);
	}
	Vector3 Vector3::Transform(const XMMATRIX& matrix) const{
		XMVECTOR vec = XMVector3Transform(*this, matrix);
		Vector3 result;
		XMStoreFloat3(&result, vec);
		return result;
	}
	void Vector3::RandomVec(float min, float max) {
		Set(Util::Random<float>(min, max), Util::Random<float>(min, max), Util::Random<float>(min, max));
	}
	Vector3 Vector3::Min(const Vector3& other)const {
		return (Vector3)XMVectorMin(*this, other);
	}
	Vector3 Vector3::Max(const Vector3& other)const {
		return (Vector3)XMVectorMax(*this, other);
	}

	Vector2 Vector3::XX() { return Vector2(x, x); }
	Vector2 Vector3::XY() { return Vector2(x, y); }
	Vector2 Vector3::XZ() { return Vector2(x, z); }

	Vector2 Vector3::YX() { return Vector2(y, x); }
	Vector2 Vector3::YY() { return Vector2(y, y); }
	Vector2 Vector3::YZ() { return Vector2(y, z); }

	Vector2 Vector3::ZX() { return Vector2(z, x); }
	Vector2 Vector3::ZY() { return Vector2(z, y); }
	Vector2 Vector3::ZZ() { return Vector2(z, z); }
}