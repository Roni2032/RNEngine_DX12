#pragma once
#include "stdafx.h"
#include "RendererHeader.h"
namespace RNEngine {
	class ConstBuffer;

	struct DebugCommand {
		Vector3 position;
		Vector3 scale;
		Vector3 rotation;
		string mesh;
		string camera;
	};
	class DebugRenderer{
		static vector<DebugCommand> g_Commands;
		static unique_ptr<ConstBuffer> g_ConstantBuffer;
		static Matrix g_Matrix;
	public:
		static void DrawCubeWireFrame(Vector3 position, Vector3 size);


		static void Init();
		static void Flush();
	};
}
