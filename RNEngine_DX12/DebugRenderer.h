#pragma once
#include "stdafx.h"
#include "RendererHeader.h"
#include "Singleton.h"

namespace RNEngine {
	class ConstantBuffer;

	struct DebugCommand {
		Vector3 position = {};
		Vector3 scale = { 1.0f,1.0f,1.0f };
		Vector3 rotation = {};
		string mesh = "";
		string camera = "Game";

		D3D_PRIMITIVE_TOPOLOGY topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	};

	class DebugRenderer : public SingletonBase<DebugRenderer> {
		friend class SingletonBase<DebugRenderer>;
		vector<unique_ptr<ConstantBuffer>> m_ConstantBuffers;
		vector<Matrix> m_Matrices;
		vector<DebugCommand> m_Commands;
	public:
		DebugRenderer();
		~DebugRenderer();

		void DrawCubeWireFrame(const Vector3& position, const Vector3& size);
		void DrawSphereWireFrame(const Vector3& position, const Vector3& size);
		void DrawLine(const Vector3& start, const Vector3& end, const float& scale);
		void DrawMeshWireFrame(const string& meshName, const Vector3& position, const Vector3& scale, const Vector3& rotation);

		void Initialize();
		void Flush();
	};
}
