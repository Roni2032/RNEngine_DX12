#include "stdafx.h"
#include "project.h"

namespace RNEngine {

    void PipelineState::Create(ComPtr<ID3D12Device>& _dev,const Shader* vs, const Shader* ps) {
		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
		m_RootSignature = new RootSignature();
		m_RootSignature->Create(_dev);

		psoDesc.pRootSignature = m_RootSignature->GetPtr().Get();

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

		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;//三角形

		psoDesc.NumRenderTargets = 1;//設定するレンダーターゲットの数
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;//レンダーターゲットの数に対応する場所に設定する

		psoDesc.SampleDesc.Count = 1;
		psoDesc.SampleDesc.Quality = 0;

		auto result = _dev->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_PipelineState));
		assert(SUCCEEDED(result));


    }
	void RootSignature::Create(ComPtr<ID3D12Device>& _dev) {
		D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};

		rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		ID3DBlob* erorBlob;
		ID3DBlob* signatureBlob;
		auto result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &signatureBlob, &erorBlob);
		assert(SUCCEEDED(result));

		result = _dev->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&m_RootSignature));
		assert(SUCCEEDED(result));

		signatureBlob->Release();
		if (erorBlob) erorBlob->Release();
	}

	void Viewport::Create(const unique_ptr<Window>& _window) {
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
    void Renderer::Init(unique_ptr<Device>& _dev,const unique_ptr<Window>& _window)
    {
		m_Device = _dev->GetPtr();
		m_CommandList = _dev->GetCommandContext()->GetList();
		m_CommandQueue = _dev->GetCommandContext()->GetQueue();
		m_CommandAllocator = _dev->GetCommandContext()->GetAllocator();

        m_SwapChain = _dev->GetSwapChain()->GetPtr();
		m_RTVBuffer = make_unique<RTVBuffer>();
        m_RTVBuffer->Init(m_Device, _dev->GetSwapChain());
        //m_DSVBuffer.Init(_dev.GetPtr(), _window);

		m_Fence = new Fence(m_Device);
		m_Barrier = new Barrier();

		Shader vs, ps;
		vs.LoadVS(L"SampleVertexShader.hlsl", "VSMain");
		ps.LoadPS(L"SamplePixelShader.hlsl", "PSMain");
		m_PipelineState = new PipelineState();
		m_PipelineState->SetInputLayout(InputLayout::P);
		m_PipelineState->Create(m_Device, &vs, &ps);


		m_ViewPort = new Viewport();
		m_Sicssor = new SicssorRect();
		m_ViewPort->Create(_window);
		m_Sicssor->Create(m_ViewPort);
        //灰色に初期化
		m_ClearColor = { 0.5f,0.5f,0.5f,1.0f };
		m_TempVertex = new VertexBuffer();
		//テスト用
		//仮の三角形データ
		vector<XMFLOAT3> vertex = {
			{0.0f,0.5f,0.0f},
			{0.5f,-0.5f,0.0f},
			{-0.5f,-0.5f,0.0f}
		};
		m_TempVertex->Create(m_Device, vertex);

    }

    void Renderer::BeginRenderer() {
        auto idx = m_SwapChain->GetCurrentBackBufferIndex();
		D3D12_RESOURCE_STATES currentState = m_RTVBuffer->GetBufferState(idx);
		
		if (currentState != D3D12_RESOURCE_STATE_RENDER_TARGET) {
			m_Barrier->Transition(m_CommandList, m_RTVBuffer->GetBackBuffer(idx), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
			m_RTVBuffer->SetBufferState(idx, D3D12_RESOURCE_STATE_RENDER_TARGET);
		}

		m_CommandList->SetPipelineState(m_PipelineState->GetPtr().Get());

        auto rtvH = m_RTVBuffer->GetDecsriptorHeap()->GetHeap()->GetCPUDescriptorHandleForHeapStart();
        rtvH.ptr += idx * m_RTVBuffer->GetDecsriptorHeap()->GetHeapSize();

        m_CommandList->OMSetRenderTargets(1, &rtvH, true, nullptr);

		m_CommandList->ClearRenderTargetView(rtvH, m_ClearColor.data(), 0, nullptr);

		m_CommandList->RSSetViewports(1, &m_ViewPort->GetViewport());
		m_CommandList->RSSetScissorRects(1, &m_Sicssor->GetRect());
		m_CommandList->SetGraphicsRootSignature(m_PipelineState->GetRootSignature()->GetPtr().Get());
		m_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
		m_CommandList->IASetVertexBuffers(0, 1, &m_TempVertex->m_VertexBufferView);

		m_CommandList->DrawInstanced(3, 1, 0, 0);

    }
    void Renderer::EndRenderer() {
		auto idx = m_SwapChain->GetCurrentBackBufferIndex();
		m_Barrier->Transition(m_CommandList, m_RTVBuffer->GetBackBuffer(idx),D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
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
		m_Fence->WaitGPU(m_CommandQueue);
	}
}