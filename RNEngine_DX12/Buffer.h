#pragma once
#include "stdafx.h"
namespace RNEngine {

	class DescriptorHeap {
		ComPtr<ID3D12DescriptorHeap> m_Heap;
		UINT m_HeapSize;
	public:
		DescriptorHeap() :m_HeapSize(0) {}
		~DescriptorHeap(){}

		bool Init(ComPtr<ID3D12Device>& _dev,UINT _frameBufferCount,D3D12_DESCRIPTOR_HEAP_TYPE _type);
		ComPtr<ID3D12DescriptorHeap> GetHeap()const { return m_Heap; }
		UINT GetHeapSize()const { return m_HeapSize; }
	};
	class RTVBuffer {
		DescriptorHeap* m_RTVHeap;
		vector<ComPtr<ID3D12Resource>> m_BackBuffer;
		vector<D3D12_RESOURCE_STATES> m_BufferStates;
	public:
		RTVBuffer() {}
		RTVBuffer(ComPtr<ID3D12Device>& _dev, unique_ptr<SwapChain>& _swapChain) { Init(_dev,_swapChain); }
		~RTVBuffer() { delete m_RTVHeap; }

		void Init(ComPtr<ID3D12Device>& _dev, unique_ptr<SwapChain>& _swapChain);

		ComPtr<ID3D12Resource> GetBackBuffer(size_t index) {
			if (m_BackBuffer.size() <= index) throw;
			return m_BackBuffer[index];
		}
		D3D12_RESOURCE_STATES GetBufferState(size_t index) {
			if (m_BufferStates.size() <= index) throw;
			return m_BufferStates[index];
		}
		void SetBufferState(size_t index, D3D12_RESOURCE_STATES state) {
			if (m_BufferStates.size() <= index) throw;
			m_BufferStates[index] = state;

		}
		DescriptorHeap* GetDecsriptorHeap() { return m_RTVHeap; }
	};
	class DSVBuffer {
		DescriptorHeap* m_DSVHeap;
	public:
		DSVBuffer() {}
		DSVBuffer(ComPtr<ID3D12Device>& _dev,const Window* _window) { Init(_dev,_window); }
		~DSVBuffer() { delete m_DSVHeap; }

		void Init(ComPtr<ID3D12Device>& _dev, const Window* _window);

		DescriptorHeap* GetDecsriptorHeap() { return m_DSVHeap; }

	};

	class VertexBuffer {
		ComPtr<ID3D12Resource> m_VertexBuffer;

		void InitVertexBufferView(const vector<XMFLOAT3>& vertex);
		void CreateVertexBuffer(ComPtr<ID3D12Device>& _dev, const vector<XMFLOAT3>& vertex);
	public:
		D3D12_VERTEX_BUFFER_VIEW m_VertexBufferView;

		VertexBuffer() {}
		~VertexBuffer() {}

		void Create(ComPtr<ID3D12Device>& _dev, const vector<XMFLOAT3>& vertex);
		ComPtr<ID3D12Resource> GetBuffer() { return m_VertexBuffer; }
	};
	class IndexBuffer {
		ComPtr<ID3D12Resource> m_IndexBuffer;
		D3D12_INDEX_BUFFER_VIEW m_IndexBufferView;
	};
}

