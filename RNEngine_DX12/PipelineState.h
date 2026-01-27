#pragma once
#include "stdafx.h"
#include "Shader.h"

namespace RNEngine {
	class RasterizerState;

	class RootSignature;
	struct PipelineStateSetup {
		RasterizerState* m_RasterizerState = nullptr;
		Shader* m_Ps = nullptr;
		Shader* m_Vs = nullptr;

		bool m_DepthEnable = true;
		D3D12_DEPTH_WRITE_MASK m_DepthMask = D3D12_DEPTH_WRITE_MASK_ALL;
	};
	class PipelineState {
		ComPtr<ID3D12PipelineState> m_PipelineState;
		unique_ptr<RootSignature> m_RootSignature;
		InputLayout m_InputLayout;
		shared_ptr<Shader> m_PSShader;
		shared_ptr<Shader> m_VSShader;
		D3D12_BLEND_DESC m_BlendState;
	public:
		PipelineState() noexcept { ZeroMemory(&m_BlendState, sizeof(m_BlendState)); }
		~PipelineState() {
		}

		void SetInputLayout(const InputLayout& layout) { m_InputLayout = layout; }
		void SetInputLayout(const vector<D3D12_INPUT_ELEMENT_DESC>& layout) { m_InputLayout = InputLayout(layout); }

		void Create(ID3D12Device* _dev, const PipelineStateSetup& setup);

		void SetVSShader(Shader* shader) { m_VSShader = make_shared<Shader>(*shader); }
		void SetPSShader(Shader* shader) { m_PSShader = make_shared<Shader>(*shader); }

		ID3D12PipelineState* GetPtr() { return m_PipelineState.Get(); }
		RootSignature* GetRootSignature();
	};


	class PipelineStatePool
	{
		static unordered_map<wstring, shared_ptr<PipelineState>> m_PipelineStateMap;
	public:
		static shared_ptr<PipelineState> RegisterPipelineState(const wstring& name, InputLayout layout, const PipelineStateSetup& setup);

		static shared_ptr<PipelineState> GetPipelineState(const wstring& name);
	};
}


