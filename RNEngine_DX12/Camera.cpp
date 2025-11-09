#include "stdafx.h"
#include "Camera.h"

namespace RNEngine {
	Camera::Camera() : m_Eye{0.0f,0.0f,0.0f},m_Target{0.0f,0.0f,1.0f},m_Up{0.0f,1.0f,0.0f},
		m_IsOrthographic(false), m_IsDirty(true)
	{
		SetViewMatrix(m_Eye, m_Target, m_Up);
		SetProjectionMatrix(XM_PIDIV2,static_cast<float>(1280) / static_cast<float>(720),0.1f,1000.0f);
	}

	void Camera::Update() {
		if (m_IsDirty) {
			UpdateViewMatrix();
		}
	}
	XMMATRIX Camera::GetViewProjectionMatrix()const{
		if (m_IsOrthographic) {
			XMFLOAT2 offset = { m_Target.x, m_Target.y };
			return XMMatrixOrthographicOffCenterLH(offset.x, m_WindowWidth + offset.x, m_WindowHeight + offset.y, offset.y, 0, 1);
		}
		return m_ViewMatrix * m_ProjectionMatrix;
	}
	void Camera::UpdateViewMatrix() {
		if (m_IsOrthographic) {
			m_Eye.z = -1.0f;
			m_Target.z = 0.0f;
		}
		m_ViewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&m_Eye), XMLoadFloat3(&m_Target), XMLoadFloat3(&m_Up));
		m_IsDirty = false;
	}
	void Camera::SetViewMatrix(XMFLOAT3 eye, XMFLOAT3 target, XMFLOAT3 up) {
		SetEye(eye);
		SetTarget(target);
		SetUp(up);

		UpdateViewMatrix();
	}
	void Camera::SetProjectionMatrix(float fovY, float aspectRatio, float nearZ, float farZ) {
		m_ProjectionMatrix = XMMatrixPerspectiveFovLH(fovY,aspectRatio,nearZ,farZ);
	}

}