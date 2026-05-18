#include "stdafx.h"
#include "Descriptor.h"

namespace RNEngine {
	DescriptorHeap::DescriptorHeap() :m_HeapSize(0), m_HeapCount(0) {}
	DescriptorHeap::~DescriptorHeap() {}

	bool DescriptorHeap::Init(ID3D12Device* _dev, UINT _frameBufferCount, D3D12_DESCRIPTOR_HEAP_TYPE _type, D3D12_DESCRIPTOR_HEAP_FLAGS _flags) {
		D3D12_DESCRIPTOR_HEAP_DESC desc = {};
		desc.NumDescriptors = _frameBufferCount;
		desc.NodeMask = 0;
		desc.Type = _type;
		desc.Flags = _flags;
		auto hr = _dev->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_Heap));
		if (FAILED(hr)) {
			//RTV用のディスクリプタヒープの作成に失敗した。
			return false;
		}
		//ディスクリプタのサイズを取得。
		m_HeapSize = _dev->GetDescriptorHandleIncrementSize(_type);

		return true;
	}
}