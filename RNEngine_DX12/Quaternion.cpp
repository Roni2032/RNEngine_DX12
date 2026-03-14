#include "stdafx.h"
#include "Quaternion.h"

namespace RNEngine {
	const Quaternion Quaternion::Identity = Quaternion(0, 0, 0, 1);

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

	Vector3 Quaternion::ConvertToRollPitchYaw() const {
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
}