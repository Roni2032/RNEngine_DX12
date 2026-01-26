#pragma once
#include "stdafx.h"
#include "RendererHeader.h"
namespace RNEngine {
	class ConstantBuffer;

	struct DebugCommand {
		Vector3 position;
		Vector3 scale;
		Vector3 rotation;
		string mesh;
		string camera;
	};
	struct WireFrameCB {
		Matrix m_Matrix;
		Vector4 m_Color;
	};
	class DebugRenderer{
		static vector<DebugCommand> g_Commands;
		static unique_ptr<ConstantBuffer> g_ConstantBuffer;
		static WireFrameCB g_FrameCB;
		static Matrix g_Matrix;
	public:
		static void DrawCubeWireFrame(Vector3 position, Vector3 size);


		static void Init();
		static void Flush();
	};
}
