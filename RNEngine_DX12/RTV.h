#pragma once
#include "stdafx.h"
#include "Device.h"

namespace RNEngine {
	class DescriptorHeap;
	class SwapChain;


	class RTVBuffer {
		unique_ptr<DescriptorHeap> m_RTVHeap;
		vector<ComPtr<ID3D12Resource>> m_BackBuffer;
		vector<D3D12_RESOURCE_STATES> m_BufferStates;

		void CreateRTV(ID3D12Device* _dev, UINT count);
	public:
		D3D12_RENDER_TARGET_VIEW_DESC m_RTVDesc{};
		RTVBuffer();
		RTVBuffer(ID3D12Device* _dev, SwapChain* _swapChain);
		~RTVBuffer();

		void InitFrameBuffer(ID3D12Device* _dev, SwapChain* _swapChain);
		void Init(ID3D12Device* _dev);
		DescriptorHeap* GetDescriptorHeap();

		ID3D12Resource* GetBackBuffer(size_t index) {
			if (m_BackBuffer.size() <= index) throw;
			return m_BackBuffer[index].Get();
		}
		D3D12_RESOURCE_STATES GetBufferState(size_t index) {
			if (m_BufferStates.size() <= index) throw;
			return m_BufferStates[index];
		}
		void SetBufferState(size_t index, D3D12_RESOURCE_STATES state) {
			if (m_BufferStates.size() <= index) throw;
			m_BufferStates[index] = state;

		}
	};
}