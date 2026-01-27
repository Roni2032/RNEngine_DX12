#pragma once
#include "stdafx.h"
#include "RendererHeader.h"
#include "Singleton.h"

namespace RNEngine {
	class ConstantBuffer;

	struct DebugCommand {
		Vector3 position;
		Vector3 scale;
		Vector3 rotation;
		string mesh;
		string camera;
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
		void Initialize();
		void Flush();
	};
}
