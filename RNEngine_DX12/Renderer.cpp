#include "stdafx.h"
#include "Renderer.h"
#include "RNEngine.h"
#include "Window.h"

#include "Descriptor.h"
#include "TextureBuffer.h"
#include "ConstantBuffer.h"
#include "DSV.h"
#include "RTV.h"
#include "SRV.h"

#include "PipelineState.h"
#include "DebugRenderer.h"
#include "EditorGUI.h"

#include "TextureResource.h"

#include "RendererComponent.h"
#include "ImageRenderer.h"
#include "Scene.h"
#include "Camera.h"
#include "RenderTarget.h"

namespace RNEngine {
	RasterizerState::RasterizerState() :m_RasterizerState{} {
		Init();
	}
	RasterizerState::~RasterizerState() {}

	void RasterizerState::Init() {
		ZeroMemory(&m_RasterizerState, sizeof(m_RasterizerState));
		m_RasterizerState = {
			(D3D12_FILL_MODE)FillMode::SOLID,
			(D3D12_CULL_MODE)CullMode::BACK,
			TRUE,                    
			D3D12_DEFAULT_DEPTH_BIAS,
			D3D12_DEFAULT_DEPTH_BIAS_CLAMP,
			D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS,
			TRUE,                     // DepthClip
			FALSE,                    // Multisample
			FALSE,                    // AA line
			0,                        // ForcedSampleCount
			D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF
		};
	}

	RootSignature::RootSignature() {}
	RootSignature::~RootSignature() {}

	void RootSignature::Create(ID3D12Device* _dev,const UINT srvCount,const UINT cbvCount) {
		D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};

		rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		m_DescriptorTable = make_unique<DescriptorTable>();
		m_DescriptorTable->Create(D3D12_SHADER_VISIBILITY_ALL, 1, 1);
		rootSignatureDesc.pParameters = m_DescriptorTable->GetRootParameters().data();
		rootSignatureDesc.NumParameters = (UINT)m_DescriptorTable->GetRootParameters().size();

		m_Sampler = make_unique<Sampler>();
		m_Sampler->Create();
		rootSignatureDesc.pStaticSamplers = &m_Sampler->GetDesc();
		rootSignatureDesc.NumStaticSamplers = 1;

		ID3DBlob* errorBlob;
		ID3DBlob* signatureBlob;
		auto result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &signatureBlob, &errorBlob);
		//assert(SUCCEEDED(result));
		if (FAILED(result)) {
			if (errorBlob) {
				OutputDebugStringA((char*)errorBlob->GetBufferPointer());
				errorBlob->Release();
			}
			assert(false); // シリアライズ失敗
		}
		result = _dev->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&m_RootSignature));
		assert(SUCCEEDED(result));

		signatureBlob->Release();
		if (errorBlob) errorBlob->Release();
	}

	DescriptorTable::DescriptorTable() {}
	DescriptorTable::~DescriptorTable() {}

	void DescriptorTable::AddDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE type, const UINT numDescriptor, const UINT startRegisterIndex){
		D3D12_DESCRIPTOR_RANGE range = {};
		range.NumDescriptors = numDescriptor;
		range.RangeType = type;
		range.BaseShaderRegister = startRegisterIndex;
		range.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		m_DescriptorRanges.push_back(range);
	}
	void DescriptorTable::Create(D3D12_SHADER_VISIBILITY visibility,const UINT srvCount, const UINT cbvCount) {
		//個数分のSRV用レンジを作成
		for (UINT i = 0; i < srvCount; i++) {
			AddDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, i);
		}
		//個数分のCBV用レンジを作成
		for (UINT i = 0; i < cbvCount; i++) {
			AddDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, i);
		}

		for (UINT i = 0; i < m_DescriptorRanges.size(); i++) {
			D3D12_ROOT_PARAMETER parameter = {};
			parameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
			parameter.ShaderVisibility = visibility;
			parameter.DescriptorTable.pDescriptorRanges = &m_DescriptorRanges[i];
			parameter.DescriptorTable.NumDescriptorRanges = 1;

			m_Parameters.push_back(parameter);
		}
	}

	void Sampler::Create() {
		ZeroMemory(&m_SamplerDesc, sizeof(m_SamplerDesc));
		m_SamplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		m_SamplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		m_SamplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		m_SamplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
		m_SamplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		m_SamplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		m_SamplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
		m_SamplerDesc.MinLOD = 0.0f;
		m_SamplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	}

	void Viewport::Create(const Window* _window) {
		Create(_window->GetWidth(), _window->GetHeight(), 0, 0);
	}
	void Viewport::Create(UINT width, UINT height, float topX, float topY) {
		m_Viewport.TopLeftX = topX;
		m_Viewport.TopLeftY = topY;
		m_Viewport.Width = (FLOAT)width;
		m_Viewport.Height = (FLOAT)height;
		m_Viewport.MinDepth = 0.0f;
		m_Viewport.MaxDepth = 1.0f;
	}


	Renderer::Renderer() : m_ClearColor({ 1,1,1,1 }) {}
	Renderer::~Renderer() = default;

    void Renderer::Init(const Window* _window)
    {
		auto dev = Engine::GetDevice();
		auto d3d12Device = dev->GetPtr();
		m_CommandList = dev->GetCommandContext()->GetList();
		m_CommandQueue = dev->GetCommandContext()->GetQueue();
		m_CommandAllocator = dev->GetCommandContext()->GetAllocator();

        m_SwapChain = dev->GetSwapChain()->GetPtr();
		m_RTVBuffer = make_unique<RTVBuffer>();
        m_RTVBuffer->InitFrameBuffer(d3d12Device, dev->GetSwapChain());
		m_DSVBuffer = make_unique<DSVBuffer>();
        m_DSVBuffer->Init(d3d12Device, _window);

		m_SrvCbvDescriptorHeap = make_unique<DescriptorHeap>();
		m_SrvCbvDescriptorHeap->Init(d3d12Device, 2048, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);

		m_Fence = make_unique<Fence>(d3d12Device);
		m_Barrier = make_unique<Barrier>();

		PipelineStateSetup setup = {};
		setup.m_Vs = new Shader();
		setup.m_Vs->LoadVS(L"SampleVertexShader.hlsl", "VSMain");
		setup.m_Ps = new Shader();
		setup.m_Ps->LoadPS(L"SamplePixelShader.hlsl", "PSMain");
		PipelineStatePool::RegisterPipelineState(L"Sample1", InputLayout::PUV, setup);

		m_ViewPort = make_unique<Viewport>();
		m_Sicssor = make_unique<SicssorRect>();
		m_ViewPort->Create(_window);
		m_Sicssor->Create(m_ViewPort.get());
        //灰色に初期化
		m_ClearColor = { 0.5f,0.5f,0.5f,1.0f };

		DebugRenderer::Get().Initialize();
	}

    void Renderer::BeginRenderer() {
        auto idx = m_SwapChain->GetCurrentBackBufferIndex();

		D3D12_RESOURCE_STATES currentState = m_RTVBuffer->GetBufferState(idx);
		
		if (currentState != D3D12_RESOURCE_STATE_RENDER_TARGET) {
			m_Barrier->Transition(m_CommandList.Get(), m_RTVBuffer->GetBackBuffer(idx), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
			m_RTVBuffer->SetBufferState(idx, D3D12_RESOURCE_STATE_RENDER_TARGET);
		}

        auto rtvH = m_RTVBuffer->GetDescriptorHeap()->GetCPUHandle();
        rtvH.ptr += idx * m_RTVBuffer->GetDescriptorHeap()->GetHeapSize();

		auto dsvH = m_DSVBuffer->GetDescriptorHeap()->GetCPUHandle();

        m_CommandList->OMSetRenderTargets(1, &rtvH, true, &dsvH);

		m_CommandList->ClearRenderTargetView(rtvH, m_ClearColor.data(), 0, nullptr);
		m_CommandList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

		m_CommandList->RSSetViewports(1, &m_ViewPort->GetViewport());
		m_CommandList->RSSetScissorRects(1, &m_Sicssor->GetRect());
    }
    void Renderer::EndRenderer(GUIRenderer* guiRenderer) {
		BeginRenderer();

		auto mainCamera = GetMainCamera();
		if (mainCamera) {
			CopyToFrameBuffer(mainCamera->GetRenderTarget());
		}
		auto idx = m_SwapChain->GetCurrentBackBufferIndex();

		//DebugRenderer::Get().DrawSphereWireFrame(Vector3(0, 0, 0), Vector3(1.0f));
		
		//デバッグ描画のコマンドをすべて実行
		DebugRenderer::Get().FlushWireFrames();

		
		//すべての描画が終わった後にGUIを表示
		if (guiRenderer != nullptr) {
			guiRenderer->UpdateRenderer(m_CommandList.Get(), m_SrvCbvDescriptorHeap.get());
		}

		m_Barrier->Transition(m_CommandList.Get(), m_RTVBuffer->GetBackBuffer(idx), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		m_RTVBuffer->SetBufferState(idx, D3D12_RESOURCE_STATE_PRESENT);
        m_CommandList->Close();

		ID3D12CommandList* cmdLists[] = { m_CommandList.Get() };
		m_CommandQueue->ExecuteCommandLists(_countof(cmdLists), cmdLists);
        
		WaitGPU();

		m_CommandAllocator->Reset();
		m_CommandList->Reset(m_CommandAllocator.Get(), nullptr);

		m_SwapChain->Present(1, 0);

	}

	void Renderer::WaitGPU() {
		m_Fence->WaitGPU(m_CommandQueue.Get());
	}

	void Renderer::CopyToFrameBuffer(RenderTarget* renderTarget) {
		auto idx = m_SwapChain->GetCurrentBackBufferIndex();

		auto rtvH = m_RTVBuffer->GetDescriptorHeap()->GetCPUHandle();
		rtvH.ptr += idx * m_RTVBuffer->GetDescriptorHeap()->GetHeapSize();

		auto dsvH = m_DSVBuffer->GetDescriptorHeap()->GetCPUHandle();

		m_CommandList->ClearRenderTargetView(rtvH, m_ClearColor.data(), 0, nullptr);
		m_CommandList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

		auto barrier = Barrier();

		auto textureRes = renderTarget->GetRenderTargetTexture()->GetTexture()->GetBuffer();

		barrier.Transition(m_CommandList.Get(), m_RTVBuffer->GetBackBuffer(idx), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_COPY_DEST);
		barrier.Transition(m_CommandList.Get(), textureRes, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_COPY_SOURCE);
		m_RTVBuffer->SetBufferState(idx, D3D12_RESOURCE_STATE_COPY_DEST);

		m_CommandList->OMSetRenderTargets(1, &rtvH, true, &dsvH);

		m_CommandList->RSSetViewports(1, &m_ViewPort->GetViewport());
		m_CommandList->RSSetScissorRects(1, &m_Sicssor->GetRect());

		m_CommandList->CopyResource(m_RTVBuffer->GetBackBuffer(idx), textureRes);

		barrier.Transition(m_CommandList.Get(), m_RTVBuffer->GetBackBuffer(idx), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_RENDER_TARGET);
		barrier.Transition(m_CommandList.Get(), textureRes, D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		m_RTVBuffer->SetBufferState(idx, D3D12_RESOURCE_STATE_RENDER_TARGET);
	}
	void Renderer::RegisterMainCamera(const shared_ptr<Camera>& camera) {
		auto renderTarget = camera->GetRenderTarget();
		renderTarget->SetClearColor(Color(m_ClearColor));
		m_MainCamera = camera;
	}
	Camera* Renderer::GetMainCamera()const {
		return m_MainCamera.lock().get();
	}

	void Renderer::RegisterTextureBuffer(TextureBuffer* texBuffer) {
		auto handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(
			m_SrvCbvDescriptorHeap->GetCPUHandle(),
			m_SrvCbvDescriptorHeap->GetHeapCount(),
			m_SrvCbvDescriptorHeap->GetHeapSize()
		);
		auto srv = texBuffer->GetSRV();
		auto dev = Engine::GetID3D12Device();
		dev->CreateShaderResourceView(texBuffer->GetBuffer(), &srv->GetDesc(), handle);
		texBuffer->SetSRVHandle(m_SrvCbvDescriptorHeap->GetHeapCount());
		m_SrvCbvDescriptorHeap->AddHeapCount();
	}
	void Renderer::RegisterConstantBuffer(ConstantBuffer* constBuffer) {
		auto handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(
			m_SrvCbvDescriptorHeap->GetHeap()->GetCPUDescriptorHandleForHeapStart(),
			m_SrvCbvDescriptorHeap->GetHeapCount(),
			m_SrvCbvDescriptorHeap->GetHeapSize()
		);
		auto dev = Engine::GetID3D12Device();
		dev->CreateConstantBufferView(&constBuffer->GetDesc(), handle);
		constBuffer->SetHandle(m_SrvCbvDescriptorHeap->GetHeapCount());
		m_SrvCbvDescriptorHeap->AddHeapCount();
	}

	CD3DX12_GPU_DESCRIPTOR_HANDLE Renderer::GetSRVDescriptorGPUHandle(UINT handle) {
		return CD3DX12_GPU_DESCRIPTOR_HANDLE(
			m_SrvCbvDescriptorHeap->GetGPUHandle(),
			handle,
			m_SrvCbvDescriptorHeap->GetHeapSize()
		);
	}
	CD3DX12_CPU_DESCRIPTOR_HANDLE Renderer::GetSRVDescriptorCPUHandle(UINT handle) {
		return CD3DX12_CPU_DESCRIPTOR_HANDLE(
			m_SrvCbvDescriptorHeap->GetCPUHandle(),
			handle,
			m_SrvCbvDescriptorHeap->GetHeapSize()
		);
	}
	DescriptorHeap* Renderer::GetSrvDescriptorHeap() { return m_SrvCbvDescriptorHeap.get(); }
}