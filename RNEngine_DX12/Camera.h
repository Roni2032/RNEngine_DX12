#pragma once
#include "Component.h"

namespace RNEngine {
	class Camera : public Component {
		//Vector3 m_Eye;
		Vector3 m_Target;
		Vector3 m_Up;

		XMMATRIX m_ViewMatrix;
		XMMATRIX m_ProjectionMatrix;

		float m_Width;
		float m_Height;

		bool m_IsOrthographic;

		vector<string> m_RenderingTag;

		void UpdateViewMatrix();
	public:
		Camera(const shared_ptr<GameObject>& ptr);
		virtual ~Camera();

		virtual void LastUpdate()override;

		void SetTarget(const Vector3& target);
		void SetEye(const Vector3& eye);
		void SetUp(const Vector3& up);

		Vector3 GetTarget()const;
		Vector3 GetEye()const;
		Vector3 GetUp()const;

		void SetViewMatrix(const Vector3& eye, const Vector3& target, const Vector3& up);
		void SetProjectionMatrix(float fovY, float aspectRatio, float nearZ, float farZ);

		XMMATRIX GetViewMatrix();
		XMMATRIX GetProjectionMatrix()const;
		XMMATRIX GetViewProjectionMatrix();

		void SetOrthographic(bool flag);
		bool IsOrthographic()const;

		void SetWidth(float width);
		void SetHeight(float height);

		float GetWidth()const;
		float GetHeight()const;

		void LookAtDirection(const Vector3& direction);

		void AddRenderingTag(const string& tag);
		vector<string> GetRenderingTags()const;

		INSPECTOR_COMPONENT(Camera)
			REGISTER_NAME(Camera)
			BEGIN_REFLECT()
			
			END_REFLECT()
	};

}
