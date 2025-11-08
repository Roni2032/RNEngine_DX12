#pragma once
#include "stdafx.h"

namespace RNEngine {
	class PipelineState;
	class Shader;
	struct InputLayout;

	class PipelineStatePool
	{
		static unordered_map<wstring, shared_ptr<PipelineState>> m_PipelineStateMap;
	public:
		static shared_ptr<PipelineState> RegisterPipelineState(const wstring& name, const Shader* vs, const Shader* ps, InputLayout layout);

		static shared_ptr<PipelineState> GetPipelineState(const wstring& name);
	};
}


