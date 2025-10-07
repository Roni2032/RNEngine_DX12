#pragma once
namespace RNEngine {

	class Buffer
	{
	};

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
		DescriptorHeap m_RTVHeap;
		vector<ComPtr<ID3D12Resource>> m_BackBuffer;
	public:
		RTVBuffer() {}
		RTVBuffer(ComPtr<ID3D12Device>& _dev,SwapChain& _swapChain) { Init(_dev,_swapChain); }
		~RTVBuffer() {}

		void Init(ComPtr<ID3D12Device>& _dev,SwapChain& _swapChain);

		ComPtr<ID3D12Resource>& GetBackBuffer(size_t index) {
			if (m_BackBuffer.size() <= index) return;
			return m_BackBuffer[index];
		}
		DescriptorHeap& GetDecsriptorHeap() { return m_RTVHeap; }
	};
	class DSVBuffer {
		DescriptorHeap m_DSVHeap;
	public:
		DSVBuffer() {}
		DSVBuffer(ComPtr<ID3D12Device>& _dev,const Window& _window) { Init(_dev,_window); }
		~DSVBuffer() {}

		void Init(ComPtr<ID3D12Device>& _dev, const Window& _window);

		DescriptorHeap& GetDecsriptorHeap() { return m_DSVHeap; }

	};
}

