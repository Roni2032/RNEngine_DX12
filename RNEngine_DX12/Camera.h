#pragma once
#include "Component.h"

namespace RNEngine {
	class RendererComponent;
	class RenderTarget;

	class Camera : public Component {
		//カメラの注視点
		Vector3 m_Target;
		//カメラの上方向
		Vector3 m_Up;

		//ビュー行列と射影行列
		XMMATRIX m_ViewMatrix;
		XMMATRIX m_ProjectionMatrix;

		//画角
		float m_Width;
		float m_Height;

		//正射影かどうか
		bool m_IsOrthographic;

		//カメラに移すレイヤー名
		vector<int> m_RenderingLayers;
		//カメラに映すレンダラーコンポーネント
		vector<shared_ptr<RendererComponent>> m_RendererObjects;
		//レンダリングターゲット
		shared_ptr<RenderTarget> m_RenderTarget;

		void UpdateViewMatrix();
	public:
		Camera(const shared_ptr<GameObject>& ptr);
		virtual ~Camera();

		virtual void Start()override;
		virtual void LastUpdate()override;

		void DrawRenderTarget();

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

		void AddRenderingLayer(const string& layer);
		void AddRenderingLayer(const int layer);
		vector<int> GetRenderingLayers()const;


		void AddRenderObject(const shared_ptr<RendererComponent>& renderer);
		RenderTarget* GetRenderTarget();

		INSPECTOR_COMPONENT(Camera)
			REGISTER_NAME(Camera)
			BEGIN_REFLECT()
			
			END_REFLECT()
	};

}
