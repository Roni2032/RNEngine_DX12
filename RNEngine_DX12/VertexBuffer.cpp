#include "VertexBuffer.h"

namespace RNEngine {
	VertexBuffer::VertexBuffer() {}
	VertexBuffer::~VertexBuffer() {}

	void VertexBuffer::InitVertexBufferView() {
		m_VertexBufferView.BufferLocation = m_Buffer->GetGPUVirtualAddress();
		m_VertexBufferView.SizeInBytes = sizeof(Vertex) * (UINT)m_Vertex.size();
		m_VertexBufferView.StrideInBytes = sizeof(m_Vertex[0]);
	}
	void VertexBuffer::CreateVertexBuffer(ID3D12Device* _dev, const vector<Vertex>& vertex) {
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
			IID_PPV_ARGS(m_Buffer.GetAddressOf()));

		if (FAILED(result)) {
			_com_error err(result);
			wprintf(L"CreateCommittedResource failed: 0x%08X (%s)\n", result, err.ErrorMessage());
		}
		assert(SUCCEEDED(result));

		Vertex* vertMap = nullptr;

		result = m_Buffer->Map(0, nullptr, (void**)&vertMap);
		copy(vertex.begin(), vertex.end(), vertMap);
		m_Buffer->Unmap(0, nullptr);
	}

	void VertexBuffer::Create(ID3D12Device* _dev, const vector<Vertex>& vertex) {
		CreateVertexBuffer(_dev, vertex);
		m_Vertex = vertex;
		InitVertexBufferView();
	}
}