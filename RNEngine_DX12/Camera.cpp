#include "stdafx.h"
#include "Camera.h"

namespace RNEngine {
	INSPECTOR_COMPONENT_CPP(Camera)

	Camera::Camera(const shared_ptr<GameObject>& ptr):
		Component(ptr),m_Target(Vector3()),m_Up(Vector3(0.0f,1.0f,0.0f)),
		m_Height(720),m_Width(1280),
		m_IsOrthographic(false)
	{
		SetViewMatrix(Vector3(), m_Target, m_Up);
		SetProjectionMatrix(XM_PIDIV2, static_cast<float>(m_Width) / static_cast<float>(m_Height), 0.1f, 1000.0f);
	}
	Camera::~Camera(){}

	void Camera::LastUpdate() {
		auto transform = GetOwner()->GetTransform();
		Vector3 forward = transform->GetForward();
		Vector3 eye = GetEye();

		SetTarget(eye + forward);
	}

	void Camera::UpdateViewMatrix() {
		Vector3 eye = GetOwner()->GetTransform()->GetPosition();
		if (m_IsOrthographic) {
			eye.z = -1.0f;
			m_Target.z = 0.0f;
		}
		m_ViewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&m_Target), XMLoadFloat3(&m_Up));
	}

	void Camera::SetTarget(const Vector3& target) {
		m_Target = target;
		GetOwner()->GetTransform()->LookAt(m_Target);
	}
	void Camera::SetEye(const Vector3& eye) {
		GetOwner()->GetTransform()->SetPosition(eye);
	}
	void Camera::SetUp(const Vector3& up) {
		m_Up = up;
	}

	Vector3 Camera::GetTarget()const {
		return m_Target;
	}
	Vector3 Camera::GetEye()const {
		return GetOwner()->GetTransform()->GetPosition();
	}
	Vector3 Camera::GetUp()const {
		return m_Up;
	}

	void Camera::SetViewMatrix(const Vector3& eye, const Vector3& target, const Vector3& up) {
		SetEye(eye);
		SetTarget(target);
		SetUp(up);
	}
	void Camera::SetProjectionMatrix(float fovY, float aspectRatio, float nearZ, float farZ) {
		m_ProjectionMatrix = XMMatrixPerspectiveFovLH(fovY, aspectRatio, nearZ, farZ);
	}

	XMMATRIX Camera::GetViewMatrix() {
		UpdateViewMatrix();
		return m_ViewMatrix;
	}
	XMMATRIX Camera::GetProjectionMatrix()const {
		return m_ProjectionMatrix;
	}
	XMMATRIX Camera::GetViewProjectionMatrix() {
		UpdateViewMatrix();

		if (m_IsOrthographic) {
			XMFLOAT2 offset = { m_Target.x, m_Target.y };
			return XMMatrixOrthographicOffCenterLH(offset.x, m_Width + offset.x, m_Height + offset.y, offset.y, 0, 1);
		}

		return m_ViewMatrix * m_ProjectionMatrix;
	}

	void Camera::SetOrthographic(bool flag) {
		m_IsOrthographic = flag;
	}
	bool Camera::IsOrthographic()const {
		return m_IsOrthographic;
	}

	void Camera::SetWidth(float width) {
		m_Width = width;
	}
	void Camera::SetHeight(float height) {
		m_Height = height;
	}

	float Camera::GetWidth()const {
		return m_Width;
	}
	float Camera::GetHeight()const {
		return m_Height;
	}

	void Camera::LookAtDirection(const Vector3& direction) {
		Vector3 eye = GetOwner()->GetTransform()->GetPosition();
		SetTarget(eye + direction.Normalized());
	}

	void Camera::AddRenderingTag(const string& tag) {
		m_RenderingTag.push_back(tag);
	}
	vector<string> Camera::GetRenderingTags()const {
		return m_RenderingTag;
	}
}