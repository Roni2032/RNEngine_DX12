#pragma once
namespace RNEngine {
	class Camera
	{
		XMFLOAT3 m_Eye;
		XMFLOAT3 m_Target;
		XMFLOAT3 m_Up;
	public:
		Camera() :m_Eye{}, m_Target{},m_Up(0,1,0) {}
		~Camera(){}

		void SetEye(XMFLOAT3 eye) { m_Eye = eye; }
		void SetTarget(XMFLOAT3 target) { m_Target = target; }
		void SetUp(XMFLOAT3 up) { m_Up = up; }

		XMMATRIX GetViewMatrix() { return XMMatrixLookAtLH(XMLoadFloat3(&m_Eye), XMLoadFloat3(&m_Target), XMLoadFloat3(&m_Up)); }
	};

}
