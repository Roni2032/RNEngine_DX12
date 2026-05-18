#include "stdafx.h"
#include "PipelineState.h"
#include "RNEngine.h"
#include "Renderer.h"
#include "EditorGUI.h"

namespace RNEngine {
	RootSignature* PipelineState::GetRootSignature() {
		return m_RootSignature.get();
	}
	void PipelineState::Create(ID3D12Device* _dev, const PipelineStateSetup& setup) {
		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
		m_RootSignature = make_unique<RootSignature>();
		m_RootSignature->Create(_dev, setup.m_SrvTextureCount, setup.m_CbvCount);

		psoDesc.pRootSignature = m_RootSignature->GetPtr();

		if (setup.m_Vs == nullptr) {
			DebugLog::Log(L"頂点シェーダーが登録されていません。", LogData::Type::Error);
			return;
		}
		if (setup.m_Ps == nullptr) {
			DebugLog::Log(L"ピクセルシェーダーが登録されていません。", LogData::Type::Error);
			return;
		}

		psoDesc.VS = setup.m_Vs->GetBytecode();
		psoDesc.PS = setup.m_Ps->GetBytecode();


		psoDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
		if (setup.m_RasterizerState) {
			psoDesc.RasterizerState = setup.m_RasterizerState->GetDesc();
		}
		else {
			RasterizerState state = RasterizerState();
			psoDesc.RasterizerState = state.GetDesc();
		}
		m_BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		m_BlendState.RenderTarget->BlendEnable = true;
		m_BlendState.RenderTarget->SrcBlend = D3D12_BLEND_SRC_ALPHA;
		m_BlendState.RenderTarget->DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		m_BlendState.RenderTarget->BlendOp = D3D12_BLEND_OP_ADD;
		psoDesc.BlendState = m_BlendState;

		psoDesc.InputLayout.pInputElementDescs = m_InputLayout.m_Layout.data();
		psoDesc.InputLayout.NumElements = (UINT)m_InputLayout.m_Layout.size();
		psoDesc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;//カットなし

		psoDesc.DepthStencilState.DepthEnable = setup.m_DepthEnable;
		psoDesc.DepthStencilState.DepthWriteMask = setup.m_DepthMask;
		psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS; // 既存の深度値よりも小さかったら更新する
		psoDesc.DepthStencilState.StencilEnable = false;
		psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;

		psoDesc.PrimitiveTopologyType = setup.m_TopologyType;

		psoDesc.NumRenderTargets = 1;//設定するレンダーターゲットの数
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;//レンダーターゲットの数に対応する場所に設定する

		psoDesc.SampleDesc.Count = 1;
		psoDesc.SampleDesc.Quality = 0;

		auto result = _dev->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_PipelineState));
		assert(SUCCEEDED(result));


	}


	unordered_map<wstring, shared_ptr<PipelineState>> PipelineStatePool::m_PipelineStateMap = {};

	shared_ptr<PipelineState> PipelineStatePool::RegisterPipelineState(const wstring& name, InputLayout layout,const PipelineStateSetup& setup) {
		
		auto it = m_PipelineStateMap.find(name);
		if (it != m_PipelineStateMap.end()) {
			return (*it).second;
		}
		auto dev = Engine::GetID3D12Device();
		auto pipelineState = make_shared<PipelineState>();
		pipelineState->SetInputLayout(layout);
		pipelineState->Create(dev, setup);

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