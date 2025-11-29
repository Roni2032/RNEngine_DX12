#pragma once
#include "stdafx.h"
#include "Buffer.h"
#include "Shader.h"

namespace RNEngine {
	class RendererComponent;
	class PipelineState;
	class RootSignature;
	class RenderTarget;
	class DescriptorTable;
	class Sampler;
	class Fence;
	class Barrier;
	class GUIRenderer;


	///----------------------------------------------------------------
	/// Renderer ヘッダ
	/// 
	/// Renderer クラス
	/// PipelineState クラス
	/// CommandContext クラス
	///		:
	/// 
	/// ----------------------------------------------------------------

	class RootSignature {
		ComPtr<ID3D12RootSignature> m_RootSignature;
		unique_ptr<DescriptorTable> m_DescriptorTable;
		unique_ptr<Sampler> m_Sampler;
	public:
		RootSignature() {}
		~RootSignature() {}

		ID3D12RootSignature* GetPtr() { return m_RootSignature.Get(); }

		void Create(ID3D12Device* _dev);

	};
	class DescriptorTable {
		vector<D3D12_ROOT_PARAMETER> m_Parameters;
		vector<D3D12_DESCRIPTOR_RANGE> m_DescriptorRanges;

		void AddDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE type, UINT numDescriptor);
	public:
		DescriptorTable() {}
		~DescriptorTable() {}

		void Create(D3D12_SHADER_VISIBILITY visibility);

		vector<D3D12_ROOT_PARAMETER>& GetRootParameters() { return m_Parameters; }
		const vector<D3D12_DESCRIPTOR_RANGE>& GetDescriptorRanges() { return m_DescriptorRanges; }
		size_t GetRangeSize()const { return m_DescriptorRanges.size(); }
	};
	class Sampler {
		D3D12_STATIC_SAMPLER_DESC m_SamplerDesc{};
	public:
		Sampler(){}
		~Sampler() {}

		void Create();

		D3D12_STATIC_SAMPLER_DESC& GetDesc() { return m_SamplerDesc; }
	};
	class RenderTarget {
		shared_ptr<TextureBuffer> m_RenderTargetTexture;
		unique_ptr<RTVBuffer> m_Rtv;
		unique_ptr<DSVBuffer> m_Dsv;
		
		float m_Width;
		float m_Height;

		DXGI_FORMAT m_Format;
		array<float, 4> m_ClearColor;
	public:
		void Create(Vector2 renderSize, DXGI_FORMAT format, array<float, 4> clearColor = {1.0f,1.0f,1.0f,1.0f});

		void DrawBegin(ID3D12GraphicsCommandList* cmdList);
		void DrawEnd(ID3D12GraphicsCommandList* cmdList);
		void Draw(ID3D12GraphicsCommandList* cmdList, vector<shared_ptr<RendererComponent>>& renderers);

		RTVBuffer* GetRTVBuffer() { return m_Rtv.get(); }

		shared_ptr<TextureBuffer> GetRenderTargetTexture() { return m_RenderTargetTexture; }
	};

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

		void Create(ID3D12Device* _dev,const Shader* vs,const Shader* ps);

		void SetVSShader( Shader* shader) { m_VSShader = make_shared<Shader>(*shader); }
		void SetPSShader( Shader* shader) { m_PSShader = make_shared<Shader>(*shader); }

		ID3D12PipelineState* GetPtr() { return m_PipelineState.Get(); }
		RootSignature* GetRootSignature() { return m_RootSignature.get(); }
	};



	/// <summary>
	/// 描画処理を行うクラス
	/// </summary>
	class Renderer {
		unordered_map<string, shared_ptr<RenderTarget>> m_RenderTargets;//登録されたレンダーターゲット
		//今回のフレームに描画するオブジェクトをレンダーターゲットに振り分けたやつ
		unordered_map<string, vector<shared_ptr<RendererComponent>>> m_CurrentFrameRenderObjects;
		vector<string> m_RenderTargetOrder;//レンダーターゲットの描画順(何もしなければ登録順)

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
	public:
		Renderer() : m_ClearColor({1,1,1,1}) {}
		~Renderer() {}

		void Init(const Window* _window);
		void BeginRenderer();
		void EndRenderer(GUIRenderer* guiRenderer = nullptr);

		void WaitGPU();

		void SetClearColor(float r, float g, float b, float a) {
			m_ClearColor = { r,g,b,a };
		}
		array<float, 4> GetClearColor() { return m_ClearColor; }

		void RegisterTextureBuffer(TextureBuffer& texBuffer);
		void RegisterConstantBuffer(ConstBuffer& constBuffer);

		void RegisterRenderTarget(const string& name, shared_ptr<RenderTarget>& renderTarget) {
			m_RenderTargets[name] = renderTarget;
			m_CurrentFrameRenderObjects[name] = {};
			m_RenderTargetOrder.push_back(name);
		}

		void Draw(shared_ptr<RendererComponent>& renderer);
		void DrawAll();

		CD3DX12_GPU_DESCRIPTOR_HANDLE GetSRVDescriptorGPUHandle(UINT handle);
		CD3DX12_CPU_DESCRIPTOR_HANDLE GetSRVDescriptorCPUHandle(UINT handle);

		ID3D12GraphicsCommandList* GetCommandList() { return m_CommandList.Get(); }
		DescriptorHeap* GetSrvDescriptorHeap() { return m_SrvCbvDescriptorHeap.get(); }

		RTVBuffer* GetFrameRTVBuffer() { return m_RTVBuffer.get(); }
	};
}