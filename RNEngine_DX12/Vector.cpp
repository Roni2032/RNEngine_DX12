#include "stdafx.h"
#include "Vector.h"
namespace RNEngine {
	const Vector2 Vector2::Zero = Vector2(0, 0);
	const Vector2 Vector2::One = Vector2(1, 1);

	const Vector3 Vector3::Zero = Vector3(0, 0, 0);
	const Vector3 Vector3::One = Vector3(1, 1, 1);

	const Vector4 Vector4::Zero = Vector4(0, 0, 0, 0);
	const Vector4 Vector4::One = Vector4(1, 1, 1, 1);

	const Quaternion Quaternion::Identity = Quaternion(0, 0, 0, 1);

	const Color Color::White = Color(1, 1, 1, 1);
	const Color Color::Red = Color(1, 0, 0, 1);
	const Color Color::Blue = Color(0, 0, 1, 1);
	const Color Color::Black = Color(0, 0, 0, 1);
	const Color Color::Green = Color(0, 1, 0, 1);

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
	float Vector3::Length() {
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
	void Vector3::RandomVec(float min, float max) {
		Set(Util::Random<float>(min, max), Util::Random<float>(min, max), Util::Random<float>(min, max));
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


	Quaternion::Quaternion() = default;
	Quaternion::Quaternion(float x, float y, float z, float w) : XMFLOAT4(x, y, z, w) {}
	Quaternion::Quaternion(const Quaternion& other) : XMFLOAT4(other.x, other.y, other.z, other.w) {}
	Quaternion::Quaternion(const Vector3& other) {
		XMVECTOR quat = XMQuaternionRotationRollPitchYaw(other.x, other.y, other.z);
		XMStoreFloat4((XMFLOAT4*)this, quat);
	}

	Quaternion::Quaternion(const XMVECTOR& other) : XMFLOAT4() {
		XMVECTOR temp = other;
		XMStoreFloat4((XMFLOAT4*)this, temp);
	}
	Quaternion::operator XMVECTOR() const {
		XMFLOAT4 temp = *this;
		XMVECTOR Vec = XMLoadFloat4(&temp);
		return Vec;
	}
	Quaternion& Quaternion::operator=(const XMVECTOR& other) {
		XMStoreFloat4((XMFLOAT4*)this, other);
		return *this;
	}
	Quaternion& Quaternion::operator=(const Quaternion& other) {
		this->x = other.x;
		this->y = other.y;
		this->z = other.z;
		this->w = other.w;
		return *this;
	}
	Quaternion Quaternion::operator*(const Quaternion& other) {
		return (Quaternion)XMQuaternionMultiply(*this, other);
	}
	bool Quaternion::operator==(const Quaternion& other) {
		return XMVector4Equal(*this, other);
	}
	bool Quaternion::operator!=(const Quaternion& other) {
		return !XMVector4Equal(*this, other);
	}

	void Quaternion::Set(float x, float y, float z, float w) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	Quaternion Quaternion::Normalize() {
		*this = (Quaternion)XMQuaternionNormalize(*this);
		return *this;
	}
	Quaternion Quaternion::Normalized() const {
		return (Quaternion)XMQuaternionNormalize(*this);
	}
	void Quaternion::RotateX(float angle) {
		XMVECTOR rot = XMQuaternionRotationAxis(Vector3(1, 0, 0), angle);
		*this = XMQuaternionMultiply(rot, *this);
		Normalize();
	}
	void Quaternion::RotateY(float angle) {
		XMVECTOR rot = XMQuaternionRotationAxis(Vector3(0, 1, 0), angle);
		*this = XMQuaternionMultiply(rot, *this);
		Normalize();
	}
	void Quaternion::RotateZ(float angle) {
		XMVECTOR rot = XMQuaternionRotationAxis(Vector3(0, 0, 1), angle);
		*this = XMQuaternionMultiply(rot, *this);
		Normalize();
	}
	void Quaternion::Rotate(const Vector3& axis, float angle) {
		XMVECTOR rot = XMQuaternionRotationAxis(axis, angle);
		*this = XMQuaternionMultiply(rot, *this);
		Normalize();
	}
	Quaternion Quaternion::RotateToVector(const Vector3& direction)const {
		Vector3 normalizedDirection = direction.Normalized();

		// ほぼゼロ長対策
		if (XMVector3NearEqual(normalizedDirection, XMVectorZero(), XMVectorReplicate(1e-6f)))
			return XMQuaternionIdentity();

		// ワールド上方向
		Vector3 up = Vector3(0, 1, 0);

		// 真上・真下対策（デバッグラインでよく踏む）
		if (fabsf(normalizedDirection.Dot(up)) > 0.999f)
			up = Vector3(0, 0, 1);

		// 直交基底作成
		Vector3 right = up.Cross(normalizedDirection).Normalized();
		//何かいい変数名を教えてくれ
		Vector3 top = normalizedDirection.Cross(right);

		// 回転行列 → Quaternion
		XMMATRIX rot = XMMATRIX(right, top, normalizedDirection, Vector4(0, 0, 0, 1));

		return (Quaternion)XMQuaternionNormalize(XMQuaternionRotationMatrix(rot));
		
	}

	Vector3 Quaternion::ConvertToRollPitchYaw() const{
		XMVECTOR v = *this;
		//各要素を抽出
		float x = XMVectorGetX(v);
		float y = XMVectorGetY(v);
		float z = XMVectorGetZ(v);
		float w = XMVectorGetW(v);

		Vector3 rpy;
		//Roll (X)
		float sinp = 2.0f * (w * x - y * z);
		//誤差対策
		if (fabs(sinp) >= 1.0f)
			rpy.x = copysign(XM_PIDIV2, sinp);
		else
			rpy.x = asinf(sinp);

		// Yaw (Y)
		rpy.y = atan2f(
			2.0f * (w * y + z * x),
			1.0f - 2.0f * (x * x + y * y)
		);

		// Yaw (Z)
		rpy.z = atan2f(
			2.0f * (w * z + x * y),
			1.0f - 2.0f * (z * z + x * x)
		);

		//角度を0～360度に変換
		if (rpy.x < 0) rpy.x = XM_PI + (rpy.x + XM_PI);
		if (rpy.y < 0) rpy.y = XM_PI + (rpy.y + XM_PI);
		if (rpy.z < 0) rpy.z = XM_PI + (rpy.z + XM_PI);

		return rpy;
	}


	Color::Color() = default;
	Color::Color(float value) : XMFLOAT4(value, value, value, value) {}
	Color::Color(float x, float y, float z, float w) : XMFLOAT4(x, y, z, w) {}
	Color::Color(const Color& other) : XMFLOAT4(other.x, other.y, other.z, other.w) {}

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