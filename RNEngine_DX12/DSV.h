#pragma once
#include "stdafx.h"

namespace RNEngine {
	class DescriptorHeap;
	class Window;

	class DSVBuffer : public DescBuffer<D3D12_DEPTH_STENCIL_VIEW_DESC> {
		unique_ptr<DescriptorHeap> m_DSVHeap;

		void CreateDSVDesc(ID3D12Device* _dev);
	public:
		DSVBuffer();
		DSVBuffer(ID3D12Device* _dev, const Window* _window);
		~DSVBuffer();

		void Init(ID3D12Device* _dev, const Window* _window);

		DescriptorHeap* GetDescriptorHeap();
	};
}