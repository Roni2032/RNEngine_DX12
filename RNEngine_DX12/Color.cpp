#include "stdafx.h"
#include "Color.h"

namespace RNEngine {
	const Color Color::White = Color(1, 1, 1, 1);
	const Color Color::Red = Color(1, 0, 0, 1);
	const Color Color::Blue = Color(0, 0, 1, 1);
	const Color Color::Black = Color(0, 0, 0, 1);
	const Color Color::Green = Color(0, 1, 0, 1);

	Color::Color() = default;
	Color::Color(float value) : XMFLOAT4(value, value, value, value) {}
	Color::Color(float x, float y, float z, float w) : XMFLOAT4(x, y, z, w) {}
	Color::Color(const Color& other) : XMFLOAT4(other.x, other.y, other.z, other.w) {}
	Color::Color(const array<float, 4>& other) :XMFLOAT4(other[0], other[1], other[2], other[3]) {}

	Color::Color(const XMVECTOR& other) : XMFLOAT4() {
		XMVECTOR temp = other;
		XMStoreFloat4((XMFLOAT4*)this, temp);
	}
	Color::operator XMVECTOR() const {
		XMFLOAT4 temp = *this;
		XMVECTOR Vec = XMLoadFloat4(&temp);
		return Vec;
	}
	Color& Color::operator=(const XMVECTOR& other) {
		XMStoreFloat4((XMFLOAT4*)this, other);
		return *this;
	}
	Color& Color::operator=(const Color& other) {
		this->x = other.x;
		this->y = other.y;
		this->z = other.z;
		this->w = other.w;
		return *this;
	}
	Color Color::operator+(const Color& other) {
		return (Color)XMVectorAdd(*this, other);
	}
	Color Color::operator-(const Color& other) {
		return (Color)XMVectorSubtract(*this, other);
	}
	Color Color::operator*(const Color& other) {
		return (Color)XMVectorMultiply(*this, other);
	}
	Color Color::operator*(const float& other) {
		Color temp = { other };
		return (Color)XMVectorMultiply(*this, temp);
	}
	Color Color::operator/(const Color& other) {
		return (Color)XMVectorDivide(*this, other);
	}
	Color Color::operator/(const float& other) {
		Color temp = { other };
		return (Color)XMVectorDivide(*this, temp);
	}
	bool Color::operator==(const Color& other) {
		return XMVector3Equal(*this, other);
	}
	bool Color::operator!=(const Color& other) {
		return !XMVector3Equal(*this, other);
	}

	void Color::Set(float x, float y, float z, float w) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}
	void Color::Set(float value) {
		this->x = this->y = this->z = this->w = value;
	}

	void Color::RandomVec(float min, float max) {
		Set(Util::Random<float>(min, max), Util::Random<float>(min, max), Util::Random<float>(min, max), Util::Random<float>(min, max));
	}
}