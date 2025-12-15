#pragma once
#include "stdafx.h"

namespace RNEngine {
	class PipelineState;
	class Shader;
	class RasterizerState;
	struct InputLayout;

	class PipelineStatePool
	{
		static unordered_map<wstring, shared_ptr<PipelineState>> m_PipelineStateMap;
	public:
		static shared_ptr<PipelineState> RegisterPipelineState(const wstring& name, InputLayout layout,const Shader* vs, const Shader* ps,const RasterizerState* rasterizerState = nullptr );

		static shared_ptr<PipelineState> GetPipelineState(const wstring& name);
	};
}


