#pragma once
#include "stdafx.h"
#include "RendererHeader.h"
#include "Singleton.h"

namespace RNEngine {
	class ConstantBuffer;

	struct WireFrameCB {
		Matrix m_Matrix;
		Vector4 m_Color;
	};
	struct DebugCommand {
		Vector3 position = {};
		Vector3 scale = { 1.0f,1.0f,1.0f };
		Quaternion rotation = Quaternion::Identity;
		string mesh = "";
		wstring camera = L"Game";
		Color color = Color(1.0f);

		D3D_PRIMITIVE_TOPOLOGY topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		string pipelineState = "WireFrame";
	};

	class DebugRenderer : public SingletonBase<DebugRenderer> {
		friend class SingletonBase<DebugRenderer>;
		vector<unique_ptr<ConstantBuffer>> m_ConstantBuffers;
		vector<WireFrameCB> m_Matrices;
		vector<DebugCommand> m_Commands;
	public:
        DebugRenderer();
		~DebugRenderer();

		/// <summary>
		/// ワイヤーフレーム立方体を描画キューに追加する。
		/// </summary>
		void DrawCubeWireFrame(const Vector3& position, const Vector3& size,const Color& color = Color(1.0f));
		/// <summary>
		/// ワイヤーフレーム球を描画キューに追加する。
		/// </summary>
		void DrawSphereWireFrame(const Vector3& position, const Vector3& size, const Color& color = Color(1.0f));
		/// <summary>
		/// 線分を描画キューに追加する。
		/// </summary>
		void DrawLine(const Vector3& start, const Vector3& end, const float& scale, const Color& color = Color(1.0f),const bool isDepth = true);
		/// <summary>
		/// メッシュのワイヤーフレームを描画キューに追加する。
		/// </summary>
		void DrawMeshWireFrame(const string& meshName, const Vector3& position, const Vector3& scale, const Vector3& rotation);
		/// <summary>
		/// ワールドグリッドを描画する。
		/// </summary>
		void DrawWorldGridFrame(const Vector3& min, const Vector3& max, const float gridSize = 1.0f);
		/// <summary>
		/// トランスフォームギズモを描画する。
		/// </summary>
		void DrawTransformGizmo(const Vector3& position, const float size = 1.0f);

		/// <summary>
		/// 初期化処理
		/// </summary>
		void Initialize();
		/// <summary>
		/// キューにたまったワイヤーフレームをフラッシュして描画する。
		/// </summary>
		void FlushWireFrames();
	};
}
