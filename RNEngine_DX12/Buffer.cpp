#include "stdafx.h"
#include "project.h"

namespace RNEngine {

	bool DescriptorHeap::Init(ComPtr<ID3D12Device>& _dev, UINT _frameBufferCount, D3D12_DESCRIPTOR_HEAP_TYPE _type) {
		D3D12_DESCRIPTOR_HEAP_DESC desc = {};
		desc.NumDescriptors = _frameBufferCount;
		desc.NodeMask = 0;
		desc.Type = _type;
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		auto hr = _dev->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_Heap));
		if (FAILED(hr)) {
			//RTV用のディスクリプタヒープの作成に失敗した。
			return false;
		}
		//ディスクリプタのサイズを取得。
		m_HeapSize = _dev->GetDescriptorHandleIncrementSize(_type);

		return true;
	}

	void RTVBuffer::Init(ComPtr<ID3D12Device>& _dev,unique_ptr<SwapChain>& _swapChian) {
		UINT frameBufferCount = 2;

		m_RTVHeap = new DescriptorHeap();
		m_RTVHeap->Init(_dev, frameBufferCount, D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_RTVHeap->GetHeap()->GetCPUDescriptorHandleForHeapStart();

		DXGI_SWAP_CHAIN_DESC swapDesc;
		auto result = _swapChian->GetPtr()->GetDesc(&swapDesc);
		assert(SUCCEEDED(result));

		m_BackBuffer.resize(swapDesc.BufferCount);
		m_BufferStates.resize(swapDesc.BufferCount);
		//フロントバッファをバックバッファ用のRTVを作成。
		for (UINT n = 0; n < frameBufferCount; n++) {
			_swapChian->GetPtr()->GetBuffer(n, IID_PPV_ARGS(&m_BackBuffer[n]));
			_dev->CreateRenderTargetView(
				m_BackBuffer[n].Get(), nullptr, rtvHandle
			);
			m_BufferStates[n] = D3D12_RESOURCE_STATE_PRESENT;
			rtvHandle.ptr += m_RTVHeap->GetHeapSize();
		}
	}

	void DSVBuffer::Init(ComPtr<ID3D12Device>& _dev, const Window* _window) {
		m_DSVHeap = new DescriptorHeap();
		m_DSVHeap->Init(_dev, 1, D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	}

	void VertexBuffer::InitVertexBufferView(const vector<XMFLOAT3>& vertex) {
		m_VertexBufferView.BufferLocation = m_VertexBuffer->GetGPUVirtualAddress();
		m_VertexBufferView.SizeInBytes = sizeof(XMFLOAT3) * (UINT)vertex.size();
		m_VertexBufferView.StrideInBytes = sizeof(vertex[0]);
	}
	void VertexBuffer::CreateVertexBuffer(ComPtr<ID3D12Device>& _dev, const vector<XMFLOAT3>& vertex) {
		D3D12_HEAP_PROPERTIES heapProp = {};
		heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
		heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

		D3D12_RESOURCE_DESC resDesc = {};
		resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resDesc.Width = sizeof(XMFLOAT3) * vertex.size();
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

		XMFLOAT3* vertMap = nullptr;

		result = m_VertexBuffer->Map(0, nullptr, (void**)&vertMap);
		copy(vertex.begin(), vertex.end(), vertMap);
		m_VertexBuffer->Unmap(0, nullptr);
	}

	void VertexBuffer::Create(ComPtr<ID3D12Device>& _dev, const vector<XMFLOAT3>& vertex) {
		CreateVertexBuffer(_dev, vertex);
		InitVertexBufferView(vertex);
	}
}
