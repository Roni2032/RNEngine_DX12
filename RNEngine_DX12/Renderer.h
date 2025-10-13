#pragma once
#include "stdafx.h"
#include "Buffer.h"
#include "Shader.h"
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
	class PipelineState;
	class RootSignature;
	class RenderTarget;

	class RootSignature {
		ComPtr<ID3D12RootSignature> m_RootSignature;
	public:
		RootSignature() {}
		~RootSignature() {}

		ComPtr<ID3D12RootSignature>& GetRootSignature() { return m_RootSignature; }

		void Create(ComPtr<ID3D12Device>& _dev);

	};

	class RenderTarget {
		ComPtr<ID3D12Resource> m_RenderTarget;
	};

	class Viewport {
		D3D12_VIEWPORT m_Viewport;
	public:
		Viewport() noexcept { ZeroMemory(&m_Viewport, sizeof(m_Viewport)); }
		~Viewport() {}

		void Create(const Window& _window);
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
		void Create(const Viewport& _viewport) {
			Create((UINT)_viewport.GetTopX(), (UINT)_viewport.GetTopY(), (UINT)_viewport.GetWidth(), (UINT)_viewport.GetHeight());
		}
		void Create(int left, int top, int right, int bottom) {
			m_Rect.left = left;
			m_Rect.top = top;
			m_Rect.right = right;
			m_Rect.bottom = bottom;
		}

		D3D12_RECT& GetRect() { return m_Rect; }
	};

	class PipelineState {
		ComPtr<ID3D12PipelineState> m_PipelineState;
		RootSignature m_RootSignature;
		InputLayout m_InputLayout;
		Shader m_PSShader;
		Shader m_VSShader;
		D3D12_BLEND_DESC m_BlendState;
	public:
		PipelineState() noexcept { ZeroMemory(&m_BlendState, sizeof(m_BlendState)); }
		~PipelineState() {}

		void SetInputLayout(const InputLayout& layout) { m_InputLayout = layout; }
		void SetInputLayout(const vector<D3D12_INPUT_ELEMENT_DESC>& layout) { m_InputLayout = InputLayout(layout); }

		void Create(ComPtr<ID3D12Device>& _dev,const Shader& vs,const Shader& ps);

		void SetVSShader(const Shader& shader) { m_VSShader = shader; }
		void SetPSShader(const Shader& shader) { m_PSShader = shader; }

		ComPtr<ID3D12PipelineState>& GetPipelineState() { return m_PipelineState; }
		RootSignature& GetRootSignature() { return m_RootSignature; }
	};

	/// <summary>
	/// 描画処理を行うクラス
	/// </summary>
	class Renderer {
		ComPtr<ID3D12Device> m_Device;

		RTVBuffer m_RTVBuffer;	//レンダーターゲットビュー用のヒープ
		DSVBuffer m_DSVBuffer;	//深度バッファ用のヒープ
		Viewport m_ViewPort;
		SicssorRect m_Sicssor;
		ComPtr<ID3D12GraphicsCommandList> m_CommandList;
		ComPtr<ID3D12CommandQueue> m_CommandQueue;
		ComPtr<ID3D12CommandAllocator> m_CommandAllocator;
		ComPtr<IDXGISwapChain4> m_SwapChain;
		Fence m_Fence;
		Barrier m_Barrier;
		PipelineState m_PipelineState;

		vector<float> m_ClearColor;
	public:
		Renderer() {}
		~Renderer() {}

		void Init(Device& _dev, const Window& _window);
		void BeginRenderer();
		void EndRenderer();

		void SetClearColor(float r, float g, float b, float a) {
			m_ClearColor = { r,g,b,a };
		}
	};

}