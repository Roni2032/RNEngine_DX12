#pragma once
#include "stdafx.h"

namespace RNEngine {
	class DescriptorHeap {
		ComPtr<ID3D12DescriptorHeap> m_Heap;
		UINT m_HeapSize;
		UINT m_HeapCount; //ŽŸ‚É’Ç‰Á‚·‚éƒq[ƒv”Ô†(-1‚µ‚½•ª‚¾‚¯“o˜^Ï‚Ý)
	public:
		DescriptorHeap();
		~DescriptorHeap();
		void Reset() { m_HeapCount = 0; }

		bool Init(ID3D12Device* _dev, UINT _frameBufferCount, D3D12_DESCRIPTOR_HEAP_TYPE _type, D3D12_DESCRIPTOR_HEAP_FLAGS _flags);
		ID3D12DescriptorHeap* GetHeap()const { return m_Heap.Get(); }
		ID3D12DescriptorHeap** GetHeapAddress() { return m_Heap.GetAddressOf(); }
		UINT GetHeapSize()const { return m_HeapSize; }
		D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle()const { return m_Heap->GetGPUDescriptorHandleForHeapStart(); }
		D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle()const { return m_Heap->GetCPUDescriptorHandleForHeapStart(); }

		void AddHeapCount() { m_HeapCount++; }
		UINT GetHeapCount() { return m_HeapCount; }
	};
}