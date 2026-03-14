#include "stdafx.h"
#include "Vector2.h"

namespace RNEngine {
	const Vector2 Vector2::Zero = Vector2(0, 0);
	const Vector2 Vector2::One = Vector2(1, 1);

	Vector2::Vector2() = default;
	Vector2::Vector2(float value) : XMFLOAT2(value, value) {}
	Vector2::Vector2(float x, float y) : XMFLOAT2(x, y) {}
	Vector2::Vector2(const Vector2& other) : XMFLOAT2(other.x, other.y) {}

	Vector2::Vector2(const XMVECTOR& other) : XMFLOAT2() {
		XMVECTOR temp = other;
		XMStoreFloat2((XMFLOAT2*)this, temp);
	}
	Vector2::operator XMVECTOR() const {
		XMFLOAT2 temp = *this;
		return XMLoadFloat2(&temp);
	}
	Vector2& Vector2::operator=(const XMVECTOR& other) {
		XMStoreFloat2((XMFLOAT2*)this, other);
		return *this;
	}
	Vector2& Vector2::operator=(const Vector2& other) {
		this->x = other.x;
		this->y = other.y;
		return *this;
	}

	Vector2 Vector2::operator+(const Vector2& other) {
		return (Vector2)XMVectorAdd(*this, other);
	}
	Vector2 Vector2::operator-(const Vector2& other) {
		return (Vector2)XMVectorSubtract(*this, other);
	}
	Vector2 Vector2::operator*(const Vector2& other) {
		return (Vector2)XMVectorMultiply(*this, other);
	}
	Vector2 Vector2::operator*(const float& other) {
		Vector2 temp = { other };
		return (Vector2)XMVectorMultiply(*this, temp);
	}
	Vector2 Vector2::operator/(const Vector2& other) {
		return (Vector2)XMVectorDivide(*this, other);
	}
	Vector2 Vector2::operator/(const float& other) {
		Vector2 temp = { other };
		return (Vector2)XMVectorDivide(*this, temp);
	}
	bool Vector2::operator==(const Vector2& other) {
		return XMVector2Equal(*this, other);
	}
	bool Vector2::operator!=(const Vector2& other) {
		return !XMVector2Equal(*this, other);
	}

	void Vector2::Set(float x, float y) {
		this->x = x;
		this->y = y;
	}
	void Vector2::Set(float value) {
		this->x = this->y = value;
	}

	float Vector2::Length() {
		return XMVectorGetX(XMVector2Length(*this));
	}
	Vector2 Vector2::Normalize() {
		*this = (Vector2)XMVector2Normalize(*this);
		return *this;
	}
	Vector2 Vector2::Normalized()const {
		return (Vector2)XMVector2Normalize(*this);
	}

	void Vector2::RandomVec(float min, float max) {
		Set(Util::Random<float>(min, max), Util::Random<float>(min, max));
	}
}
