#include "stdafx.h"
#include "project.h"

namespace RNEngine {

	bool DescriptorHeap::Init(ComPtr<ID3D12Device>& _dev, UINT _frameBufferCount, D3D12_DESCRIPTOR_HEAP_TYPE _type, D3D12_DESCRIPTOR_HEAP_FLAGS _flags) {
		D3D12_DESCRIPTOR_HEAP_DESC desc = {};
		desc.NumDescriptors = _frameBufferCount;
		desc.NodeMask = 0;
		desc.Type = _type;
		desc.Flags = _flags;
		auto hr = _dev->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_Heap));
		if (FAILED(hr)) {
			//RTV�p�̃f�B�X�N���v�^�q�[�v�̍쐬�Ɏ��s�����B
			return false;
		}
		//�f�B�X�N���v�^�̃T�C�Y���擾�B
		m_HeapSize = _dev->GetDescriptorHandleIncrementSize(_type);

		return true;
	}

	void RTVBuffer::Init(ComPtr<ID3D12Device>& _dev, SwapChain* _swapChian) {
		UINT frameBufferCount = 2;

		m_RTVHeap = make_unique<DescriptorHeap>();
		m_RTVHeap->Init(_dev, frameBufferCount, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE);

		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_RTVHeap->GetHeap()->GetCPUDescriptorHandleForHeapStart();

		DXGI_SWAP_CHAIN_DESC swapDesc;
		auto result = _swapChian->GetPtr()->GetDesc(&swapDesc);
		assert(SUCCEEDED(result));

		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
		rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

		m_BackBuffer.resize(swapDesc.BufferCount);
		m_BufferStates.resize(swapDesc.BufferCount);
		//�t�����g�o�b�t�@���o�b�N�o�b�t�@�p��RTV���쐬�B
		for (UINT n = 0; n < frameBufferCount; n++) {
			_swapChian->GetPtr()->GetBuffer(n, IID_PPV_ARGS(&m_BackBuffer[n]));
			_dev->CreateRenderTargetView(
				m_BackBuffer[n].Get(), &rtvDesc, rtvHandle
			);
			m_BufferStates[n] = D3D12_RESOURCE_STATE_PRESENT;
			rtvHandle.ptr += m_RTVHeap->GetHeapSize();
		}
	}

	void DSVBuffer::CreateDSVDesc(ComPtr<ID3D12Device>& _dev) {
		ZeroMemory(&m_DSVDesc, sizeof(m_DSVDesc));
		m_DSVDesc.Format = DXGI_FORMAT_D32_FLOAT;
		m_DSVDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		m_DSVDesc.Flags = D3D12_DSV_FLAG_NONE;

		_dev->CreateDepthStencilView(
			m_DSBuffer.Get(), // ComPtr���Ǘ����Ă���f�[�^��n��
			&m_DSVDesc,
			m_DSVHeap->GetCPUHandle());
	}

	void DSVBuffer::Init(ComPtr<ID3D12Device>& _dev, const Window* _window) {

		D3D12_RESOURCE_DESC depthResDesc = {};
		depthResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D; // �񎟌��z��I�ȃo�b�t�@
		depthResDesc.Width = _window->GetWidth(); // ��ʂ́u���v�ɍ��킹��
		depthResDesc.Height = _window->GetHeight(); // ��ʂ́u�����v�ɍ��킹��
		depthResDesc.DepthOrArraySize = 1;
		depthResDesc.Format = DXGI_FORMAT_D32_FLOAT; // �X�e���V������
		depthResDesc.SampleDesc.Count = 1;
		depthResDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

		// �[�x�l�p�q�[�v�v���p�e�B
		D3D12_HEAP_PROPERTIES depthHeapProp = {};
		depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
		depthHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		depthHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

		// �N���A�o�����[��ݒ肷��
		D3D12_CLEAR_VALUE depthClearValue = {};
		depthClearValue.DepthStencil.Depth = 1.0f; // �[�x�̍ő�l
		depthClearValue.Format = DXGI_FORMAT_D32_FLOAT;

		auto result = _dev->CreateCommittedResource(
			&depthHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&depthResDesc,
			D3D12_RESOURCE_STATE_DEPTH_WRITE,
			&depthClearValue,
			IID_PPV_ARGS(m_DSBuffer.GetAddressOf()));
		assert(SUCCEEDED(result));

		m_DSVHeap = make_unique<DescriptorHeap>();
		m_DSVHeap->Init(_dev, 1, D3D12_DESCRIPTOR_HEAP_TYPE_DSV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE);

		CreateDSVDesc(_dev);
	}
	void SRVBuffer::CreateSRVDesc(ComPtr<ID3D12Device>& _dev, TextureBuffer& texBuffer, DXGI_FORMAT format) {
		ZeroMemory(&m_SRVDesc, sizeof(m_SRVDesc));
		m_SRVDesc.Format = format;
		m_SRVDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		m_SRVDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		m_SRVDesc.Texture2D.MipLevels = 1;
		
		auto& renderer = RnEngine::g_pInstance->GetRenderer();
		renderer->RegisterTextureBuffer(texBuffer);
	}
	void SRVBuffer::Init(ComPtr<ID3D12Device>& _dev, TextureBuffer& texBuffer, DXGI_FORMAT format) {
		CreateSRVDesc(_dev, texBuffer, format);
	}

	void ConstBuffer::Create(ComPtr<ID3D12Device>& _dev, Matrix& matrix) {

		auto heap = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		auto resDesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(matrix) + 0xff) & ~0xff);
		auto result = _dev->CreateCommittedResource(
			&heap,
			D3D12_HEAP_FLAG_NONE,
			&resDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(m_ConstBuffer.GetAddressOf()));

		assert(SUCCEEDED(result));
		Matrix* matrixMap;
		result = m_ConstBuffer->Map(0, nullptr, (void**)&matrixMap);

		matrixMap->m_World = matrix.m_World;
		matrixMap->m_ViewProjection = matrix.m_ViewProjection;

		ZeroMemory(&m_CBVDesc, sizeof(m_CBVDesc));
		m_CBVDesc.BufferLocation = m_ConstBuffer->GetGPUVirtualAddress();
		m_CBVDesc.SizeInBytes = (UINT)m_ConstBuffer->GetDesc().Width;

		auto& renderer = RnEngine::g_pInstance->GetRenderer();
		renderer->RegisterConstantBuffer(*this);

	}
	void ConstBuffer::Upadte(Matrix& matrix) {
		Matrix* matrixMap;
		auto result = m_ConstBuffer->Map(0, nullptr, (void**)&matrixMap);
		matrixMap->m_World = matrix.m_World;
		matrixMap->m_ViewProjection = matrix.m_ViewProjection;

	}
	void VertexBuffer::InitVertexBufferView(const vector<Vertex>& vertex) {
		m_VertexBufferView.BufferLocation = m_VertexBuffer->GetGPUVirtualAddress();
		m_VertexBufferView.SizeInBytes = sizeof(Vertex) * (UINT)vertex.size();
		m_VertexBufferView.StrideInBytes = sizeof(vertex[0]);
	}
	void VertexBuffer::CreateVertexBuffer(ComPtr<ID3D12Device>& _dev, const vector<Vertex>& vertex) {
		D3D12_HEAP_PROPERTIES heapProp = {};
		heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
		heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

		D3D12_RESOURCE_DESC resDesc = {};
		resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resDesc.Width = sizeof(Vertex) * vertex.size();
		resDesc.Height = 1;
		resDesc.DepthOrArraySize = 1;
		resDesc.MipLevels = 1;
		resDesc.SampleDesc.Count = 1;
		resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		resDesc.Format = DXGI_FORMAT_UNKNOWN;
		resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

		HRESULT result = _dev->CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&resDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(m_VertexBuffer.GetAddressOf()));

		if (FAILED(result)) {
			_com_error err(result);
			wprintf(L"CreateCommittedResource failed: 0x%08X (%s)\n", result, err.ErrorMessage());
		}
		assert(SUCCEEDED(result));

		Vertex* vertMap = nullptr;

		result = m_VertexBuffer->Map(0, nullptr, (void**)&vertMap);
		copy(vertex.begin(), vertex.end(), vertMap);
		m_VertexBuffer->Unmap(0, nullptr);
	}

	void VertexBuffer::Create(ComPtr<ID3D12Device>& _dev, const vector<Vertex>& vertex) {
		CreateVertexBuffer(_dev, vertex);
		InitVertexBufferView(vertex);
		m_VertexData = vertex;
	}


	void IndexBuffer::InitIndexBufferView(const vector<UINT>& index) {
		m_IndexBufferView.BufferLocation = m_IndexBuffer->GetGPUVirtualAddress();
		m_IndexBufferView.SizeInBytes = sizeof(UINT) * (UINT)index.size();
		m_IndexBufferView.Format = DXGI_FORMAT_R32_UINT;
	}
	void IndexBuffer::CreateIndexBuffer(ComPtr<ID3D12Device>& _dev, const vector<UINT>& index) {
		D3D12_HEAP_PROPERTIES heapProp = {};
		heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
		heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

		D3D12_RESOURCE_DESC resDesc = {};
		resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resDesc.Width = sizeof(UINT) * index.size();
		resDesc.Height = 1;
		resDesc.DepthOrArraySize = 1;
		resDesc.MipLevels = 1;
		resDesc.SampleDesc.Count = 1;
		resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		resDesc.Format = DXGI_FORMAT_UNKNOWN;
		resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

		HRESULT result = _dev->CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&resDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(m_IndexBuffer.GetAddressOf()));

		if (FAILED(result)) {
			_com_error err(result);
			wprintf(L"CreateCommittedResource failed: 0x%08X (%s)\n", result, err.ErrorMessage());
		}
		assert(SUCCEEDED(result));

		UINT* indexMap = nullptr;

		result = m_IndexBuffer->Map(0, nullptr, (void**)&indexMap);
		copy(index.begin(), index.end(), indexMap);
		m_IndexBuffer->Unmap(0, nullptr);
	}

	void IndexBuffer::Create(ComPtr<ID3D12Device>& _dev, const vector<UINT>& index) {
		CreateIndexBuffer(_dev, index);
		InitIndexBufferView(index);
		m_IndexData = index;
	}


	void TextureBuffer::Create(ComPtr<ID3D12Device>& _dev, const wstring& filename) {

		TexMetadata metadata;
		ScratchImage image;

		HRESULT result = LoadFromWICFile(
			filename.c_str(),
			WIC_FLAGS_NONE,
			&metadata,
			image
		);
		assert(SUCCEEDED(result));
		auto img = image.GetImage(0, 0, 0); 

		D3D12_HEAP_PROPERTIES heapProp = {};
		heapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
		heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
		heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
		heapProp.CreationNodeMask = 0;
		heapProp.VisibleNodeMask = 0;

		D3D12_RESOURCE_DESC resDesc = {};
		resDesc.Format = metadata.format;
		resDesc.Width = (UINT)metadata.width;
		resDesc.Height = (UINT)metadata.height;
		resDesc.DepthOrArraySize = 1;
		resDesc.SampleDesc.Quality = 0;
		resDesc.MipLevels = 1;
		resDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		resDesc.SampleDesc.Count = 1;
		resDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

		result = _dev->CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&resDesc,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			nullptr,
			IID_PPV_ARGS(m_TextureBuffer.GetAddressOf()));

		assert(SUCCEEDED(result));

		result = m_TextureBuffer->WriteToSubresource(
			0,
			nullptr,
			img->pixels,
			(UINT)img->rowPitch,
			(UINT)img->slicePitch
		);
		assert(SUCCEEDED(result));

		m_SRV = make_unique<SRVBuffer>();
		m_SRV->Init(_dev,*this,metadata.format);

		m_Filename = filename;
	}
}
