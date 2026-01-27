#pragma once
#include "stdafx.h"
#include "Shader.h"
#include "Renderer.h"
#include "RNEngine.h"

namespace RNEngine {
	class PipelineState;
	class Shader;
	class RasterizerState;
	struct InputLayout;

	class PipelineStatePool
	{
		static unordered_map<wstring, shared_ptr<PipelineState>> m_PipelineStateMap;
	public:
		static shared_ptr<PipelineState> RegisterPipelineState(const wstring& name, InputLayout layout, const PipelineStateSetup& setup);

		static shared_ptr<PipelineState> GetPipelineState(const wstring& name);
	};
}


