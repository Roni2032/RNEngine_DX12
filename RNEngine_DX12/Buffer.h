#pragma once
#include "stdafx.h"
#include "Shader.h"
namespace RNEngine {
	class DescriptorHeap {
		ComPtr<ID3D12DescriptorHeap> m_Heap;
		UINT m_HeapSize;
	public:
		DescriptorHeap() :m_HeapSize(0) {}
		~DescriptorHeap(){}

		bool Init(ComPtr<ID3D12Device>& _dev,UINT _frameBufferCount,D3D12_DESCRIPTOR_HEAP_TYPE _type,D3D12_DESCRIPTOR_HEAP_FLAGS _flags);
		ComPtr<ID3D12DescriptorHeap> GetHeap()const { return m_Heap; }
		UINT GetHeapSize()const { return m_HeapSize; }
	};

	class RTVBuffer {
		unique_ptr<DescriptorHeap> m_RTVHeap;
		vector<ComPtr<ID3D12Resource>> m_BackBuffer;
		vector<D3D12_RESOURCE_STATES> m_BufferStates;
	public:
		RTVBuffer() {}
		RTVBuffer(ComPtr<ID3D12Device>& _dev, unique_ptr<SwapChain>& _swapChain) { Init(_dev,_swapChain); }
		~RTVBuffer() { }

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
		unique_ptr<DescriptorHeap>& GetDecsriptorHeap() { return m_RTVHeap; }
	};
	class DSVBuffer {
		unique_ptr<DescriptorHeap> m_DSVHeap;
	public:
		DSVBuffer() {}
		DSVBuffer(ComPtr<ID3D12Device>& _dev,const Window* _window) { Init(_dev,_window); }
		~DSVBuffer() { }

		void Init(ComPtr<ID3D12Device>& _dev, const Window* _window);

		unique_ptr<DescriptorHeap>& GetDecsriptorHeap() { return m_DSVHeap; }

	};
	class SRVBuffer {
		unique_ptr<DescriptorHeap> m_SRVHeap;
		UINT m_DescriptorCount;

	public:
		D3D12_SHADER_RESOURCE_VIEW_DESC m_SRVDesc;
		SRVBuffer():m_DescriptorCount(0){}
		~SRVBuffer() {}
		void Init(ComPtr<ID3D12Device>& _dev, TextureBuffer& texBuffer, UINT descriptorCount,DXGI_FORMAT format);
		void CreateSRVDesc(ComPtr<ID3D12Device>& _dev, TextureBuffer& texBuffer, DXGI_FORMAT format);
		unique_ptr<DescriptorHeap>& GetDecsriptorHeap() { return m_SRVHeap; }
	};
	class ConstBuffer {
		ComPtr<ID3D12Resource> m_ConstBuffer;
		UINT m_BufferSize;
	public:
		ConstBuffer() :m_BufferSize(0) {}
		~ConstBuffer() {}

		void Create(ComPtr<ID3D12Device>& _dev, UINT bufferSize);

		ComPtr<ID3D12Resource> GetBuffer() { return m_ConstBuffer; }
	};
	class VertexBuffer {
		ComPtr<ID3D12Resource> m_VertexBuffer;
		vector<Vertex> m_VertexData;
		void InitVertexBufferView(const vector<Vertex>& vertex);
		void CreateVertexBuffer(ComPtr<ID3D12Device>& _dev, const vector<Vertex>& vertex);
	public:
		D3D12_VERTEX_BUFFER_VIEW m_VertexBufferView;

		VertexBuffer() {}
		~VertexBuffer() {}

		void Create(ComPtr<ID3D12Device>& _dev, const vector<Vertex>& vertex);
		ComPtr<ID3D12Resource> GetBuffer() { return m_VertexBuffer; }

		size_t GetVertexCount()const { return m_VertexData.size(); }
	};
	class IndexBuffer {
		ComPtr<ID3D12Resource> m_IndexBuffer;
		vector<UINT> m_IndexData;
		void InitIndexBufferView(const vector<UINT>& index);
		void CreateIndexBuffer(ComPtr<ID3D12Device>& _dev, const vector<UINT>& index);

	public:
		IndexBuffer() {}
		~IndexBuffer() {}
		D3D12_INDEX_BUFFER_VIEW m_IndexBufferView;

		void Create(ComPtr<ID3D12Device>& _dev, const vector<UINT>& index);
		size_t GetIndexCount()const { return m_IndexData.size(); }
	};

	class TextureBuffer {
		ComPtr<ID3D12Resource> m_TextureBuffer;
		UINT m_SRVHandle;
		//unique_ptr<SRVBuffer> m_SRV;
	public:

		TextureBuffer() {}
		~TextureBuffer() {}

		void Create(ComPtr<ID3D12Device>& _dev,const wstring& filename );
		ComPtr<ID3D12Resource> GetBuffer()const { return m_TextureBuffer; }
		void SetSRVHandle(UINT handle) { m_SRVHandle = handle; }
		UINT GetSRVHandle()const { return m_SRVHandle; }
		//unique_ptr<SRVBuffer>& GetSRV() { return m_SRV; }
	};
}

