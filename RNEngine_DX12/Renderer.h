#pragma once
namespace RNEngine {

	///----------------------------------------------------------------
	/// Renderer ヘッダ
	/// 
	/// Renderer クラス
	/// PipelineState クラス
	/// Shader クラス
	/// Buffer クラス
	/// Texture クラス
	/// CommandContext クラス
	///		:
	/// 
	/// ----------------------------------------------------------------

	/// <summary>
	/// 描画処理を行うクラス
	/// </summary>
	class Renderer {
		RTVBuffer m_RTVBuffer;	//レンダーターゲットビュー用のヒープ
		DSVBuffer m_DSVBuffer;	//深度バッファ用のヒープ
		ComPtr<D3D12_VIEWPORT> m_ViewPort;
		ComPtr<ID3D12GraphicsCommandList> m_CommandList;
		ComPtr<IDXGISwapChain4> m_SwapChain;
	public:
		Renderer() {}
		~Renderer() {}

		void Init(Device& _dev,const Window& _window);
		void BeginRenderer();
	};


	class RenderTarget {
		ComPtr<ID3D12Resource> m_RenderTarget;
	};
}