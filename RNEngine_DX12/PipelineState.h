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
		int m_SrvTextureCount = 1;
		int m_CbvCount = 1;

		bool m_DepthEnable = true;
		D3D12_DEPTH_WRITE_MASK m_DepthMask = D3D12_DEPTH_WRITE_MASK_ALL;

		D3D12_PRIMITIVE_TOPOLOGY_TYPE m_TopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	};
	class PipelineState {
		ComPtr<ID3D12PipelineState> m_PipelineState;
		unique_ptr<RootSignature> m_RootSignature;
		InputLayout m_InputLayout;
		shared_ptr<Shader> m_PSShader;
		shared_ptr<Shader> m_VSShader;
		D3D12_BLEND_DESC m_BlendState;
	public:
       /// <summary>
		/// コンストラクタ（BlendState をゼロ初期化）
		/// </summary>
		PipelineState() noexcept { ZeroMemory(&m_BlendState, sizeof(m_BlendState)); }
		/// <summary>
		/// デストラクタ
		/// </summary>
		~PipelineState() {
		}

		/// <summary>
		/// 入力レイアウトを設定する。
		/// </summary>
		/// <param name="layout">InputLayout オブジェクト</param>
		void SetInputLayout(const InputLayout& layout) { m_InputLayout = layout; }
		/// <summary>
		/// 入力レイアウトを vector から設定する。
		/// </summary>
		/// <param name="layout">D3D12_INPUT_ELEMENT_DESC の配列</param>
		void SetInputLayout(const vector<D3D12_INPUT_ELEMENT_DESC>& layout) { m_InputLayout = InputLayout(layout); }

		/// <summary>
		/// パイプラインステートを作成する。
		/// </summary>
		/// <param name="_dev">D3D12 デバイス</param>
		/// <param name="setup">作成に必要な設定情報</param>
		void Create(ID3D12Device* _dev, const PipelineStateSetup& setup);

		/// <summary>
		/// 頂点シェーダを設定する（内部でコピーする）。
		/// </summary>
		/// <param name="shader">設定するシェーダのポインタ</param>
		void SetVSShader(Shader* shader) { m_VSShader = make_shared<Shader>(*shader); }
		/// <summary>
		/// ピクセルシェーダを設定する（内部でコピーする）。
		/// </summary>
		/// <param name="shader">設定するシェーダのポインタ</param>
		void SetPSShader(Shader* shader) { m_PSShader = make_shared<Shader>(*shader); }

		/// <summary>
		/// 内部の ID3D12PipelineState の生ポインタを取得する。
		/// </summary>
		/// <returns>ID3D12PipelineState の生ポインタを返します。</returns>
		ID3D12PipelineState* GetPtr() { return m_PipelineState.Get(); }
		/// <summary>
		/// ルートシグネチャを取得する。
		/// </summary>
		/// <returns>RootSignature の生ポインタを返します。</returns>
		RootSignature* GetRootSignature();
	};


	class PipelineStatePool
	{
		static unordered_map<wstring, shared_ptr<PipelineState>> m_PipelineStateMap;
	public:
       /// <summary>
		/// パイプラインステートを登録（または取得）する。
		/// </summary>
		/// <param name="name">識別用の名前（ワイド文字列）</param>
		/// <param name="layout">入力レイアウト</param>
		/// <param name="setup">作成設定</param>
		/// <returns>登録された PipelineState の shared_ptr を返します。</returns>
		static shared_ptr<PipelineState> RegisterPipelineState(const wstring& name, InputLayout layout, const PipelineStateSetup& setup);

		/// <summary>
		/// 名前からパイプラインステートを取得する。
		/// </summary>
		/// <param name="name">識別用の名前（ワイド文字列）</param>
		/// <returns>見つかった場合は shared_ptr を返します。</returns>
		static shared_ptr<PipelineState> GetPipelineState(const wstring& name);
	};
}


