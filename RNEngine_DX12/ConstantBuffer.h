#pragma once
#include "stdafx.h"
#include "Buffer.h"
namespace RNEngine {
	class ConstantBuffer : public DescBuffer<D3D12_CONSTANT_BUFFER_VIEW_DESC> {
		UINT m_Handle;

		uint32_t* m_MappedData;
		size_t m_BufferSize;
	public:
		ConstantBuffer();
		~ConstantBuffer();

		void Create(ID3D12Device* _dev, void* data);

		void SetHandle(UINT handle) { m_Handle = handle; }
		UINT GetHandle()const { return m_Handle; }

		void Update(void* data, size_t size);
	};
}