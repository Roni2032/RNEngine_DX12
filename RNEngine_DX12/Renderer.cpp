#include "stdafx.h"
#include "project.h"

namespace RNEngine {

    void PipelineState::Create(ComPtr<ID3D12Device>& _dev,const Shader* vs, const Shader* ps) {
		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
		m_RootSignature = make_unique<RootSignature>();
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
		psoDesc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;//�J�b�g�Ȃ�

		psoDesc.DepthStencilState.DepthEnable = true;
		psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS; // �����̐[�x�l����������������X�V����
		psoDesc.DepthStencilState.StencilEnable = false;
		psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;

		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;//�O�p�`

		psoDesc.NumRenderTargets = 1;//�ݒ肷�郌���_�[�^�[�Q�b�g�̐�
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;//�����_�[�^�[�Q�b�g�̐��ɑΉ�����ꏊ�ɐݒ肷��

		psoDesc.SampleDesc.Count = 1;
		psoDesc.SampleDesc.Quality = 0;

		auto result = _dev->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_PipelineState));
		assert(SUCCEEDED(result));


    }
	void RootSignature::Create(ComPtr<ID3D12Device>& _dev) {
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
			assert(false); // �V���A���C�Y���s
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
    void Renderer::Init(Device* _dev,const Window* _window)
    {
		m_Device = _dev->GetPtr();
		m_CommandList = _dev->GetCommandContext()->GetList();
		m_CommandQueue = _dev->GetCommandContext()->GetQueue();
		m_CommandAllocator = _dev->GetCommandContext()->GetAllocator();

        m_SwapChain = _dev->GetSwapChain()->GetPtr();
		m_RTVBuffer = make_unique<RTVBuffer>();
        m_RTVBuffer->Init(m_Device, _dev->GetSwapChain().get());
		m_DSVBuffer = make_unique<DSVBuffer>();
        m_DSVBuffer->Init(m_Device, _window);

		m_Fence = make_unique<Fence>(m_Device);
		m_Barrier = make_unique<Barrier>();

		Shader vs, ps;
		vs.LoadVS(L"SampleVertexShader.hlsl", "VSMain");
		ps.LoadPS(L"SamplePixelShader.hlsl", "PSMain");
		m_PipelineState = make_unique<PipelineState>();
		m_PipelineState->SetInputLayout(InputLayout::PUV);
		m_PipelineState->Create(m_Device, &vs, &ps);

		m_SrvCbvDescriptorHeap = make_unique<DescriptorHeap>();
		m_SrvCbvDescriptorHeap->Init(m_Device, 4096, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);

		m_ViewPort = make_unique<Viewport>();
		m_Sicssor = make_unique<SicssorRect>();
		m_ViewPort->Create(_window);
		m_Sicssor->Create(m_ViewPort.get());
        //�D�F�ɏ�����
		m_ClearColor = { 0.5f,0.5f,0.5f,1.0f };
	}

    void Renderer::BeginRenderer() {
        auto idx = m_SwapChain->GetCurrentBackBufferIndex();
		D3D12_RESOURCE_STATES currentState = m_RTVBuffer->GetBufferState(idx);
		
		if (currentState != D3D12_RESOURCE_STATE_RENDER_TARGET) {
			m_Barrier->Transition(m_CommandList, m_RTVBuffer->GetBackBuffer(idx), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
			m_RTVBuffer->SetBufferState(idx, D3D12_RESOURCE_STATE_RENDER_TARGET);
		}

		m_CommandList->SetPipelineState(m_PipelineState->GetPtr().Get());

        auto rtvH = m_RTVBuffer->GetDecsriptorHeap()->GetCPUHandle();
        rtvH.ptr += idx * m_RTVBuffer->GetDecsriptorHeap()->GetHeapSize();

		auto dsvH = m_DSVBuffer->GetDecsriptorHeap()->GetCPUHandle();

        m_CommandList->OMSetRenderTargets(1, &rtvH, true, &dsvH);

		m_CommandList->ClearRenderTargetView(rtvH, m_ClearColor.data(), 0, nullptr);
		m_CommandList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

		m_CommandList->RSSetViewports(1, &m_ViewPort->GetViewport());
		m_CommandList->RSSetScissorRects(1, &m_Sicssor->GetRect());
		m_CommandList->SetGraphicsRootSignature(m_PipelineState->GetRootSignature()->GetPtr().Get());

		/*m_Matrix.m_World = XMMatrixRotationY(angle);
		angle += XM_PIDIV2 * 0.01f;

		m_TempConstantBuffer->Upadte(m_Matrix);
		for(auto& model : m_TempModel){
			model->Draw(m_CommandList, m_SrvCbvDescriptorHeap.get());
		}*/
		/*m_CommandList->SetDescriptorHeaps(1, m_SrvCbvDescriptorHeap->GetHeap().GetAddressOf());

		auto handle = m_SrvCbvDescriptorHeap->GetGPUHandle();
		m_CommandList->SetGraphicsRootDescriptorTable(0, handle);
		handle = CD3DX12_GPU_DESCRIPTOR_HANDLE(
			m_SrvCbvDescriptorHeap->GetGPUHandle(),
			m_TempTexture->GetSRVHandle(),
			m_SrvCbvDescriptorHeap->GetHeapSize()
		);
		m_CommandList->SetGraphicsRootDescriptorTable(1, handle);

		m_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
		m_CommandList->IASetVertexBuffers(0, 1, &m_TempVertex->m_VertexBufferView);
		m_CommandList->IASetIndexBuffer(&m_TempIndex->m_IndexBufferView);

		m_CommandList->DrawIndexedInstanced((UINT)m_TempIndex->GetIndexCount(), 1, 0, 0, 0);*/

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

	void Renderer::RegisterTextureBuffer(TextureBuffer& texBuffer) {
		auto handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(
			m_SrvCbvDescriptorHeap->GetCPUHandle(),
			m_SrvCbvDescriptorHeap->GetHeapCount(),
			m_SrvCbvDescriptorHeap->GetHeapSize()
		);
		D3D12_SHADER_RESOURCE_VIEW_DESC desc = texBuffer.GetSRV()->m_SRVDesc;
		m_Device->CreateShaderResourceView(texBuffer.GetBuffer().Get(), &desc, handle);
		texBuffer.SetSRVHandle(m_SrvCbvDescriptorHeap->GetHeapCount());
		m_SrvCbvDescriptorHeap->AddHeapCount();
	}
	void Renderer::RegisterConstantBuffer(ConstBuffer& constBuffer) {
		auto handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(
			m_SrvCbvDescriptorHeap->GetHeap()->GetCPUDescriptorHandleForHeapStart(),
			m_SrvCbvDescriptorHeap->GetHeapCount(),
			m_SrvCbvDescriptorHeap->GetHeapSize()
		);
		m_Device->CreateConstantBufferView(&constBuffer.m_CBVDesc, handle);
		constBuffer.SetCBVHandle(m_SrvCbvDescriptorHeap->GetHeapCount());
		m_SrvCbvDescriptorHeap->AddHeapCount();
	}

	void Renderer::DrawModel(shared_ptr<ModelRenderer>& renderer) {
		renderer->Draw(m_CommandList, m_SrvCbvDescriptorHeap.get());
	}
}