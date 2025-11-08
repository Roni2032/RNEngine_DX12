#pragma once
namespace RNEngine {
	class Camera
	{
		XMFLOAT3 m_Eye;
		XMFLOAT3 m_Target;
		XMFLOAT3 m_Up;

		XMMATRIX m_ViewMatrix;
		XMMATRIX m_ProjectionMatrix;

		//m_IsOrthographicがtrueのときのみ使用(それ以外は上の行列を使用)
		float m_WindowWidth;
		float m_WindowHeight;

		bool m_IsOrthographic;
		bool m_IsDirty;//View行列更新フラグ
	public:
		Camera();
		~Camera(){}

		void Update();

		void SetEye(XMFLOAT3 eye) { 
			m_Eye = eye;
			if (m_IsOrthographic) m_Eye.z = -1.0f;
			m_IsDirty = true;
		}
		void SetTarget(XMFLOAT3 target) {
			m_Target = target;
			if (m_IsOrthographic) m_Target.z = 0.0f;
			m_IsDirty = true;
		}
		void SetUp(XMFLOAT3 up) { 
			m_Up = up;
			m_IsDirty = true;
		}

		XMFLOAT3 GetEye() const { return m_Eye; }
		XMFLOAT3 GetTarget() const { return m_Target; }
		XMFLOAT3 GetUp() const { return m_Up; }

		void SetViewMatrix(XMFLOAT3 eye, XMFLOAT3 target, XMFLOAT3 up);
		void UpdateViewMatrix();
		void SetProjectionMatrix(float fovY, float aspectRatio, float nearZ, float farZ);

		XMMATRIX GetViewMatrix() const { return m_ViewMatrix; }
		XMMATRIX GetProjectionMatrix() const { return m_ProjectionMatrix; }

		XMMATRIX GetViewProjectionMatrix()const;
		void SetWindowWidth(float width) { m_WindowWidth = width; }
		void SetWindowHeight(float height) { m_WindowHeight = height; }

		float GetWindowWidth() const { return m_WindowWidth; }
		float GetWindowHeight() const { return m_WindowHeight; }

		void SetOrthographic(bool isOrtho) { m_IsOrthographic = isOrtho; }
		bool IsOrthographic() const { return m_IsOrthographic; }
	};

}
