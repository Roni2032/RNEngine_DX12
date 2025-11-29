#include "stdafx.h"
#include "project.h"

namespace RNEngine {

    void PipelineState::Create(ID3D12Device* _dev,const Shader* vs, const Shader* ps) {
		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
		m_RootSignature = make_unique<RootSignature>();
		m_RootSignature->Create(_dev);

		psoDesc.pRootSignature = m_RootSignature->GetPtr();

		psoDesc.VS = vs->GetBytecode();
		psoDesc.PS = ps->GetBytecode();


		psoDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
		psoDesc.RasterizerState.MultisampleEnable = false;
		psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
		psoDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		psoDesc.RasterizerState.DepthClipEnable = true;

		m_BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		m_BlendState.RenderTarget->BlendEnable = true;
		m_BlendState.RenderTarget->SrcBlend = D3D12_BLEND_SRC_ALPHA;
		m_BlendState.RenderTarget->DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		m_BlendState.RenderTarget->BlendOp = D3D12_BLEND_OP_ADD;
		psoDesc.BlendState = m_BlendState;

		psoDesc.InputLayout.pInputElementDescs = m_InputLayout.m_Layout.data();
		psoDesc.InputLayout.NumElements = (UINT)m_InputLayout.m_Layout.size();
		psoDesc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;//カットなし

		psoDesc.DepthStencilState.DepthEnable = true;
		psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS; // 既存の深度値よりも小さかったら更新する
		psoDesc.DepthStencilState.StencilEnable = false;
		psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;

		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;//三角形

		psoDesc.NumRenderTargets = 1;//設定するレンダーターゲットの数
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;//レンダーターゲットの数に対応する場所に設定する

		psoDesc.SampleDesc.Count = 1;
		psoDesc.SampleDesc.Quality = 0;

		auto result = _dev->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_PipelineState));
		assert(SUCCEEDED(result));


    }
	void RootSignature::Create(ID3D12Device* _dev) {
		D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};

		rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		m_DescriptorTable = make_unique<DescriptorTable>();
		m_DescriptorTable->Create(D3D12_SHADER_VISIBILITY_ALL);
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

	void DescriptorTable::AddDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE type, UINT numDescriptor) {
		D3D12_DESCRIPTOR_RANGE range = {};
		range.NumDescriptors = numDescriptor;
		range.RangeType = type;
		range.BaseShaderRegister = 0;
		range.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		m_DescriptorRanges.push_back(range);
	}
	void DescriptorTable::Create(D3D12_SHADER_VISIBILITY visibility) {
		AddDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1);
		AddDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1);

		for (int i = 0; i < m_DescriptorRanges.size(); i++) {
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

	void RenderTarget::Create(Vector2 renderSize, DXGI_FORMAT format, array<float, 4> clearColor) {

		m_Width = renderSize.x;
		m_Height = renderSize.y;
		m_ClearColor = clearColor;

		auto dev = Engine::GetID3D12Device();
		m_Rtv = make_unique<RTVBuffer>();
		m_Rtv->Init(dev);

		m_Dsv = make_unique<DSVBuffer>();
		auto window = Engine::GetWindow();
		m_Dsv->Init(dev, window);

		m_RenderTargetTexture = make_shared<TextureBuffer>();
		m_RenderTargetTexture->Create(dev,(UINT)renderSize.x, (UINT)renderSize.y, format, clearColor);
		
		dev->CreateRenderTargetView(m_RenderTargetTexture->GetBuffer(), &m_Rtv->m_RTVDesc, m_Rtv->GetDecsriptorHeap()->GetCPUHandle());
	
		auto res = m_RenderTargetTexture->GetBuffer();
		if (!res) { printf("ERROR: resource null\n"); }

		auto desc = res->GetDesc();
		printf("RESOURCE: Format=%u Flags=%u Width=%u Height=%u Sample=%u Mip=%u\n",
			(unsigned)desc.Format, (unsigned)desc.Flags, (unsigned)desc.Width, (unsigned)desc.Height,
			(unsigned)desc.SampleDesc.Count, (unsigned)desc.MipLevels);

		D3D12_RENDER_TARGET_VIEW_DESC rtv = m_Rtv->m_RTVDesc; // or log fields individually
		printf("RTVDesc: Format=%u ViewDim=%u\n", (unsigned)rtv.Format, (unsigned)rtv.ViewDimension);

		auto heap = m_Rtv->GetDecsriptorHeap();
		printf("Heap: type? (not accessible here) Count or pointer: %p  CPU.ptr=%llu\n",
			heap, (unsigned long long)heap->GetCPUHandle().ptr);
	}

	void RenderTarget::DrawBegin(ID3D12GraphicsCommandList* cmdList) {
		auto barrier = make_unique<Barrier>();
		barrier->Transition(cmdList, m_RenderTargetTexture->GetBuffer(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		//m_Rtv->SetBufferState(0, D3D12_RESOURCE_STATE_RENDER_TARGET);
		auto rtvH = m_Rtv->GetDecsriptorHeap()->GetCPUHandle();
		auto dsvH = m_Dsv->GetDecsriptorHeap()->GetCPUHandle();
		cmdList->OMSetRenderTargets(1, &rtvH, true, &dsvH);

		cmdList->ClearRenderTargetView(rtvH, m_ClearColor.data(), 0, nullptr);
		cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	}
	void RenderTarget::DrawEnd(ID3D12GraphicsCommandList* cmdList) {
		auto barrier = make_unique<Barrier>();
		barrier->Transition(cmdList, m_RenderTargetTexture->GetBuffer(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		//m_Rtv->SetBufferState(0, D3D12_RESOURCE_STATE_PRESENT);
	}
	void RenderTarget::Draw(ID3D12GraphicsCommandList* cmdList, vector<shared_ptr<RendererComponent>>& renderers) {
		DrawBegin(cmdList);
		auto renderer = Engine::GetRenderer();
		auto srvHeap = renderer->GetSrvDescriptorHeap();
		for (auto& renderer : renderers) {
			renderer->Draw(cmdList, srvHeap);
		}
		DrawEnd(cmdList);
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

		/*m_FrameBufferRenderTargets = {make_unique<RenderTarget>() ,make_unique<RenderTarget>()};
		m_FrameBufferRenderTargets[0]->Create({ (float)_window->GetWidth(),(float)_window->GetHeight() }, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, m_ClearColor);
		m_FrameBufferRenderTargets[1]->Create({ (float)_window->GetWidth(),(float)_window->GetHeight() }, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, m_ClearColor);
		*/
		m_Fence = make_unique<Fence>(d3d12Device);
		m_Barrier = make_unique<Barrier>();

		Shader vs, ps;
		vs.LoadVS(L"SampleVertexShader.hlsl", "VSMain");
		ps.LoadPS(L"SamplePixelShader.hlsl", "PSMain");

		PipelineStatePool::RegisterPipelineState(L"Sample1", &vs, &ps, InputLayout::PUV);

		m_ViewPort = make_unique<Viewport>();
		m_Sicssor = make_unique<SicssorRect>();
		m_ViewPort->Create(_window);
		m_Sicssor->Create(m_ViewPort.get());
        //灰色に初期化
		m_ClearColor = { 0.5f,0.5f,0.5f,1.0f };
	}

    void Renderer::BeginRenderer() {
        auto idx = m_SwapChain->GetCurrentBackBufferIndex();

		D3D12_RESOURCE_STATES currentState = m_RTVBuffer->GetBufferState(idx);
		
		if (currentState != D3D12_RESOURCE_STATE_RENDER_TARGET) {
			m_Barrier->Transition(m_CommandList.Get(), m_RTVBuffer->GetBackBuffer(idx), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
			m_RTVBuffer->SetBufferState(idx, D3D12_RESOURCE_STATE_RENDER_TARGET);
		}

        auto rtvH = m_RTVBuffer->GetDecsriptorHeap()->GetCPUHandle();
        rtvH.ptr += idx * m_RTVBuffer->GetDecsriptorHeap()->GetHeapSize();

		auto dsvH = m_DSVBuffer->GetDecsriptorHeap()->GetCPUHandle();

        m_CommandList->OMSetRenderTargets(1, &rtvH, true, &dsvH);

		m_CommandList->ClearRenderTargetView(rtvH, m_ClearColor.data(), 0, nullptr);
		m_CommandList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

		m_CommandList->RSSetViewports(1, &m_ViewPort->GetViewport());
		m_CommandList->RSSetScissorRects(1, &m_Sicssor->GetRect());

		for (auto& rendererObject : m_CurrentFrameRenderObjects) {
			rendererObject.second.clear();
		}
    }
    void Renderer::EndRenderer(GUIRenderer* guiRenderer) {
		auto idx = m_SwapChain->GetCurrentBackBufferIndex();
		//m_FrameBufferRenderTargets[idx]->DrawBegin(m_CommandList.Get());
		auto rtvH = m_RTVBuffer->GetDecsriptorHeap()->GetCPUHandle();
		rtvH.ptr += idx * m_RTVBuffer->GetDecsriptorHeap()->GetHeapSize();

		auto dsvH = m_DSVBuffer->GetDecsriptorHeap()->GetCPUHandle();

		m_CommandList->OMSetRenderTargets(1, &rtvH, true, &dsvH);

		m_CommandList->ClearRenderTargetView(rtvH, m_ClearColor.data(), 0, nullptr);
		m_CommandList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

		//すべての描画が終わった後にGUIを表示
		if (guiRenderer != nullptr) {
			guiRenderer->UpdateRenderer(m_CommandList.Get(), m_SrvCbvDescriptorHeap.get());
		}
		m_CommandList->RSSetViewports(1, &m_ViewPort->GetViewport());
		m_CommandList->RSSetScissorRects(1, &m_Sicssor->GetRect());

		//m_FrameBufferRenderTargets[idx]->DrawEnd(m_CommandList.Get());

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

	void Renderer::RegisterTextureBuffer(TextureBuffer& texBuffer) {
		auto handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(
			m_SrvCbvDescriptorHeap->GetCPUHandle(),
			m_SrvCbvDescriptorHeap->GetHeapCount(),
			m_SrvCbvDescriptorHeap->GetHeapSize()
		);
		D3D12_SHADER_RESOURCE_VIEW_DESC desc = texBuffer.GetSRV()->m_SRVDesc;
		auto dev = Engine::GetID3D12Device();
		dev->CreateShaderResourceView(texBuffer.GetBuffer(), &desc, handle);
		texBuffer.SetSRVHandle(m_SrvCbvDescriptorHeap->GetHeapCount());
		m_SrvCbvDescriptorHeap->AddHeapCount();
	}
	void Renderer::RegisterConstantBuffer(ConstBuffer& constBuffer) {
		auto handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(
			m_SrvCbvDescriptorHeap->GetHeap()->GetCPUDescriptorHandleForHeapStart(),
			m_SrvCbvDescriptorHeap->GetHeapCount(),
			m_SrvCbvDescriptorHeap->GetHeapSize()
		);
		auto dev = Engine::GetID3D12Device();
		dev->CreateConstantBufferView(&constBuffer.m_CBVDesc, handle);
		constBuffer.SetCBVHandle(m_SrvCbvDescriptorHeap->GetHeapCount());
		m_SrvCbvDescriptorHeap->AddHeapCount();
	}

	void Renderer::Draw(shared_ptr<RendererComponent>& renderer) {
		//renderer->Draw(m_CommandList.Get(), m_SrvCbvDescriptorHeap.get());
		
		for (auto tag : renderer->GetRenderTargetTag()) {
			if (m_RenderTargets.find(tag) != m_RenderTargets.end()) {
				m_CurrentFrameRenderObjects[tag].push_back(renderer);
			}
		}
	}
	void Renderer::DrawAll() {
		for (auto& rendererMap : m_CurrentFrameRenderObjects) {
			auto renderTarget = m_RenderTargets[rendererMap.first];
			renderTarget->Draw(m_CommandList.Get(), rendererMap.second);
		}
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
}