#include "stdafx.h"
#include "ConstantBuffer.h"
#include "RNEngine.h"
#include "Renderer.h"

namespace RNEngine {

	ConstantBuffer::ConstantBuffer() :m_Handle(0), m_MappedData(nullptr), m_BufferSize(0) {}
	ConstantBuffer::~ConstantBuffer() {}

	void ConstantBuffer::Create(ID3D12Device* _dev, void* data) {
		m_BufferSize = (sizeof(data) + 0xff) & ~0xff;
		auto heap = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		auto resDesc = CD3DX12_RESOURCE_DESC::Buffer(m_BufferSize);
		auto result = _dev->CreateCommittedResource(
			&heap,
			D3D12_HEAP_FLAG_NONE,
			&resDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(m_Buffer.GetAddressOf()));
		assert(SUCCEEDED(result));

		CD3DX12_RANGE readRange(0, 0);
		result = m_Buffer->Map(0, &readRange, (void**)&m_MappedData);
		if (data) memcpy(m_MappedData, data, sizeof(data));

		ZeroMemory(&m_Desc, sizeof(m_Desc));
		m_Desc.BufferLocation = m_Buffer->GetGPUVirtualAddress();
		m_Desc.SizeInBytes = (UINT)m_Buffer->GetDesc().Width;
		auto renderer = Engine::GetRenderer();
		renderer->RegisterConstantBuffer(this);
	}

	void ConstantBuffer::Update(void* data, size_t size) {
		memcpy(m_MappedData, data, size);
	}
}