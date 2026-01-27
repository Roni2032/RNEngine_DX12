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

	class Debug : public SingletonBase<Debug> {
		friend class SingletonBase<Debug>;
		vector<unique_ptr<ConstantBuffer>> m_ConstantBuffers;
		vector<Matrix> m_Matrices;
		vector<DebugCommand> m_Commands;
	public:
		void DrawCubeWireFrame(const Vector3& position, const Vector3& size);
		void Initialize();
		void Flush();
	};
}
