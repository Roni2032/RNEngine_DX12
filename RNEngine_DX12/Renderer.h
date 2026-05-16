#pragma once
#include "stdafx.h"
#include "Buffer.h"
#include "Shader.h"

namespace RNEngine {
	class RendererComponent;
	class PipelineState;
	class RootSignature;
	class RenderTarget;
	class Sampler;
	class Fence;
	class Barrier;
	class GUIRenderer;

	enum CullMode {
		NONE = D3D12_CULL_MODE_NONE,
		FRONT = D3D12_CULL_MODE_FRONT,
		BACK = D3D12_CULL_MODE_BACK
	};
	enum FillMode {
		WIREFRAME = D3D12_FILL_MODE_WIREFRAME,
		SOLID = D3D12_FILL_MODE_SOLID
	};
	class RasterizerState {
		D3D12_RASTERIZER_DESC m_RasterizerState;

		void Init();
	public:
		RasterizerState();
		~RasterizerState();

		D3D12_RASTERIZER_DESC GetDesc()const { return m_RasterizerState; }

		void SetCullMode(CullMode mode) { m_RasterizerState.CullMode = (D3D12_CULL_MODE)mode; }
		void SetFillMode(FillMode mode) { m_RasterizerState.FillMode = (D3D12_FILL_MODE)mode; }
		void SetDepthBias(int bias) { m_RasterizerState.DepthBias = bias; }
		void SetSlopeScaledDepthBias(float biasScale) { m_RasterizerState.SlopeScaledDepthBias = biasScale; }
		void SetDepthBiasClamp(float clamp) { m_RasterizerState.DepthBiasClamp = clamp; }
	};

	class DescriptorTable {
		vector<D3D12_ROOT_PARAMETER> m_Parameters;
		vector<D3D12_DESCRIPTOR_RANGE> m_DescriptorRanges;

		void AddDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE type, const UINT numDescriptor,const UINT startRegisterIndex);
	public:
		DescriptorTable();
		~DescriptorTable();

		void Create(D3D12_SHADER_VISIBILITY visibility,const UINT srvCount,const UINT cbvCount);

		vector<D3D12_ROOT_PARAMETER>& GetRootParameters() { return m_Parameters; }
		const vector<D3D12_DESCRIPTOR_RANGE>& GetDescriptorRanges() { return m_DescriptorRanges; }
		size_t GetRangeSize()const { return m_DescriptorRanges.size(); }
	};
	class RootSignature {
		ComPtr<ID3D12RootSignature> m_RootSignature;
		unique_ptr<DescriptorTable> m_DescriptorTable;
		unique_ptr<Sampler> m_Sampler;
	public:
		RootSignature();
		~RootSignature();

		ID3D12RootSignature* GetPtr() { return m_RootSignature.Get(); }

		void Create(ID3D12Device* _dev, const UINT srvCount, const UINT cbvCount);

	};
	class Sampler {
		D3D12_STATIC_SAMPLER_DESC m_SamplerDesc{};
	public:
		Sampler(){}
		~Sampler() {}

		void Create();

		D3D12_STATIC_SAMPLER_DESC& GetDesc() { return m_SamplerDesc; }
	};

	class Window;
	class Viewport {
		D3D12_VIEWPORT m_Viewport;
	public:
		Viewport() noexcept { ZeroMemory(&m_Viewport, sizeof(m_Viewport)); }
		~Viewport() {}

		void Create(const Window*_window);
		void Create(UINT width, UINT height, float topX, float topY);

		float GetWidth()const { return m_Viewport.Width; }
		float GetHeight()const { return m_Viewport.Height; }
		float GetTopX()const { return m_Viewport.TopLeftX; }
		float GetTopY()const { return m_Viewport.TopLeftY; }
		D3D12_VIEWPORT& GetViewport() { return m_Viewport; }
	};
	class SicssorRect {
		D3D12_RECT m_Rect;
	public:
		SicssorRect() noexcept { ZeroMemory(&m_Rect, sizeof(m_Rect)); }
		~SicssorRect() {}
		void Create(const Viewport* _viewport) {
			Create((UINT)_viewport->GetTopX(), (UINT)_viewport->GetTopY(), (UINT)_viewport->GetWidth(), (UINT)_viewport->GetHeight());
		}
		void Create(int left, int top, int right, int bottom) {
			m_Rect.left = left;
			m_Rect.top = top;
			m_Rect.right = right;
			m_Rect.bottom = bottom;
		}

		D3D12_RECT& GetRect() { return m_Rect; }
	};

	enum HeapType {
		CBV,
		SRV
	};

	class TextureBuffer;
	class ConstantBuffer;
	class DescriptorHeap;
	class RTVBuffer;
	class DSVBuffer;
	class Camera;

    //グラフィックスエンジンを作りましょう

	/// <summary>
	/// 描画処理を行うクラス
	/// </summary>
	class Renderer {
		weak_ptr<Camera> m_MainCamera;

		array<unique_ptr<RenderTarget>, 2> m_FrameBufferRenderTargets;
		unique_ptr<RTVBuffer> m_RTVBuffer;	//レンダーターゲットビュー用のヒープ
		unique_ptr<DSVBuffer> m_DSVBuffer;	//深度バッファ用のヒープ
		unique_ptr<Viewport> m_ViewPort;
		unique_ptr<SicssorRect> m_Sicssor;

		unique_ptr<DescriptorHeap> m_SrvCbvDescriptorHeap;
		ComPtr<ID3D12GraphicsCommandList> m_CommandList;
		ComPtr<ID3D12CommandQueue> m_CommandQueue;
		ComPtr<ID3D12CommandAllocator> m_CommandAllocator;
		ComPtr<IDXGISwapChain4> m_SwapChain;
		unique_ptr<Fence> m_Fence;
		unique_ptr<Barrier> m_Barrier;
		unique_ptr<PipelineState> m_PipelineState;
		array<float, 4> m_ClearColor;


     /// <summary>
		/// 指定したレンダーターゲットの内容をフレームバッファへ転送する内部ユーティリティ。
		/// </summary>
		/// <param name="renderTarget">転送元のレンダーターゲット</param>
		void CopyToFrameBuffer(RenderTarget* renderTarget);
	public:
     /// <summary>
		/// コンストラクタ
		/// </summary>
		Renderer();
		/// <summary>
		/// デストラクタ
		/// </summary>
		~Renderer();

		/// <summary>
		/// レンダラーを初期化する。
		/// </summary>
		/// <param name="_window">描画対象のウィンドウ情報</param>
		void Init(const Window* _window);
		/// <summary>
		/// GPU の完了を待つユーティリティ。
		/// </summary>
		void WaitGPU();

		/// <summary>
		/// フレーム描画の開始処理。
		/// </summary>
		void BeginRenderer();
		/// <summary>
		/// フレーム描画の終了処理。
		/// </summary>
		/// <param name="guiRenderer">GUI 描画用のレンダラ（任意）</param>
		void EndRenderer(GUIRenderer* guiRenderer = nullptr);

		/// <summary>
		/// クリアカラーを設定する。
		/// </summary>
		/// <param name="r">赤成分</param>
		/// <param name="g">緑成分</param>
		/// <param name="b">青成分</param>
		/// <param name="a">アルファ成分</param>
		void SetClearColor(float r, float g, float b, float a) {
			m_ClearColor = { r,g,b,a };
		}
		/// <summary>
		/// 現在設定されているクリアカラーを取得する。
		/// </summary>
		/// <returns>RGBA の配列を返します。</returns>
		array<float, 4> GetClearColor() { return m_ClearColor; }

		/// <summary>
		/// メインカメラを登録する。
		/// </summary>
		/// <param name="camera">登録するカメラの共有ポインタ</param>
		void RegisterMainCamera(const shared_ptr<Camera>& camera);
		/// <summary>
		/// 登録されているメインカメラを取得する。
		/// </summary>
		/// <returns>メインカメラの生ポインタを返します（所有権は渡しません）。</returns>
		Camera* GetMainCamera()const;

		/// <summary>
		/// テクスチャバッファを登録する。
		/// </summary>
		/// <param name="texBuffer">登録するテクスチャバッファのポインタ</param>
		void RegisterTextureBuffer(TextureBuffer* texBuffer);
		/// <summary>
		/// 定数バッファを登録する。
		/// </summary>
		/// <param name="constBuffer">登録する定数バッファのポインタ</param>
		void RegisterConstantBuffer(ConstantBuffer* constBuffer);

		/// <summary>
		/// SRV 用の GPU ディスクリプタハンドルを取得する。
		/// </summary>
		/// <param name="handle">ハンドルインデックス</param>
		/// <returns>GPU ハンドルを返します。</returns>
		CD3DX12_GPU_DESCRIPTOR_HANDLE GetSRVDescriptorGPUHandle(UINT handle);
		/// <summary>
		/// SRV 用の CPU ディスクリプタハンドルを取得する。
		/// </summary>
		/// <param name="handle">ハンドルインデックス</param>
		/// <returns>CPU ハンドルを返します。</returns>
		CD3DX12_CPU_DESCRIPTOR_HANDLE GetSRVDescriptorCPUHandle(UINT handle);

		/// <summary>
		/// 現在のコマンドリストを取得する。
		/// </summary>
		/// <returns>ID3D12GraphicsCommandList の生ポインタを返します。</returns>
		ID3D12GraphicsCommandList* GetCommandList() { return m_CommandList.Get(); }
		/// <summary>
		/// SRV/CBV 用のディスクリプタヒープを取得する。
		/// </summary>
		/// <returns>DescriptorHeap の生ポインタを返します。</returns>
		DescriptorHeap* GetSrvDescriptorHeap();

		/// <summary>
		/// フレーム用の RTV バッファを取得する。
		/// </summary>
		/// <returns>RTVBuffer の生ポインタを返します。</returns>
		RTVBuffer* GetFrameRTVBuffer() { return m_RTVBuffer.get(); }
	};
}