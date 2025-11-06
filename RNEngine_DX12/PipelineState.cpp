#include "stdafx.h"
#include "PipelineState.h"
#include "Shader.h"
#include "Renderer.h"
#include "RNEngine.h"
namespace RNEngine {
	unordered_map<wstring, shared_ptr<PipelineState>> PipelineStatePool::m_PipelineStateMap = {};

	shared_ptr<PipelineState> PipelineStatePool::RegisterPipelineState(const wstring& name, const Shader* vs, const Shader* ps, InputLayout layout) {
		
		auto it = m_PipelineStateMap.find(name);
		if (it != m_PipelineStateMap.end()) {
			return (*it).second;
		}
		auto dev = Engine::GetID3D12Device();
		auto pipelineState = make_shared<PipelineState>();
		pipelineState->SetInputLayout(layout);
		pipelineState->Create(dev, vs, ps);

		m_PipelineStateMap[name] = pipelineState;

		return pipelineState;
	}

	shared_ptr<PipelineState> PipelineStatePool::GetPipelineState(const wstring& name) {
		auto it = m_PipelineStateMap.find(name);
		if (it != m_PipelineStateMap.end()) {
			return (*it).second;
		}
		return nullptr;
	}
}