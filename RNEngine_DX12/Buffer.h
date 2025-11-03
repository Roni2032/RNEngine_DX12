#pragma once
#include "stdafx.h"
#include "Shader.h"
namespace RNEngine {
	class TextureBuffer;
	class DescriptorHeap {
		ComPtr<ID3D12DescriptorHeap> m_Heap;
		UINT m_HeapSize;
		UINT m_HeapCount; //éüÇ…í«â¡Ç∑ÇÈÉqÅ[Évî‘çÜ(-1ÇµÇΩï™ÇæÇØìoò^çœÇ›)
	public:
		DescriptorHeap() :m_HeapSize(0), m_HeapCount(0){}
		~DescriptorHeap(){}

		bool Init(ComPtr<ID3D12Device>& _dev,UINT _frameBufferCount,D3D12_DESCRIPTOR_HEAP_TYPE _type,D3D12_DESCRIPTOR_HEAP_FLAGS _flags);
		ComPtr<ID3D12DescriptorHeap> GetHeap()const { return m_Heap; }
		UINT GetHeapSize()const { return m_HeapSize; }
		D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle()const { return m_Heap->GetGPUDescriptorHandleForHeapStart(); }
		D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle()const { return m_Heap->GetCPUDescriptorHandleForHeapStart(); }

		void AddHeapCount() { m_HeapCount++; }
		UINT GetHeapCount() { return m_HeapCount; }
	};

	class RTVBuffer {
		unique_ptr<DescriptorHeap> m_RTVHeap;
		vector<ComPtr<ID3D12Resource>> m_BackBuffer;
		vector<D3D12_RESOURCE_STATES> m_BufferStates;
	public:
		RTVBuffer() {}
		RTVBuffer(ComPtr<ID3D12Device>& _dev, SwapChain* _swapChain) { Init(_dev,_swapChain); }
		~RTVBuffer() { }

		void Init(ComPtr<ID3D12Device>& _dev, SwapChain* _swapChain);

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
		ComPtr<ID3D12Resource> m_DSBuffer;

		void CreateDSVDesc(ComPtr<ID3D12Device>& _dev);
	public:
		D3D12_DEPTH_STENCIL_VIEW_DESC m_DSVDesc;
		DSVBuffer() {}
		DSVBuffer(ComPtr<ID3D12Device>& _dev,const Window* _window) { Init(_dev,_window); }
		~DSVBuffer() { }

		void Init(ComPtr<ID3D12Device>& _dev, const Window* _window);

		unique_ptr<DescriptorHeap>& GetDecsriptorHeap() { return m_DSVHeap; }

	};
	class SRVBuffer {
	public:
		D3D12_SHADER_RESOURCE_VIEW_DESC m_SRVDesc;
		SRVBuffer(){}
		~SRVBuffer() {}
		void Init(ComPtr<ID3D12Device>& _dev, TextureBuffer& texBuffer,DXGI_FORMAT format);
		void CreateSRVDesc(ComPtr<ID3D12Device>& _dev, TextureBuffer& texBuffer, DXGI_FORMAT format);
	};
	class ConstBuffer {
		ComPtr<ID3D12Resource> m_ConstBuffer;
		UINT m_CBVHandle;
	public:
		D3D12_CONSTANT_BUFFER_VIEW_DESC m_CBVDesc;

		ConstBuffer():m_CBVHandle(0){}
		~ConstBuffer() {}

		void Create(ComPtr<ID3D12Device>& _dev, Matrix& matrix);

		ComPtr<ID3D12Resource> GetBuffer() { return m_ConstBuffer; }
		void SetCBVHandle(UINT handle) { m_CBVHandle = handle; }
		UINT GetCBVHandle()const { return m_CBVHandle; }

		void Upadte(Matrix& matrix);
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
		D3D12_INDEX_BUFFER_VIEW m_IndexBufferView;

		IndexBuffer(){}
		~IndexBuffer() {}

		void Create(ComPtr<ID3D12Device>& _dev, const vector<UINT>& index);
		size_t GetIndexCount()const { return m_IndexData.size(); }
	};

	class TextureBuffer {
		ComPtr<ID3D12Resource> m_TextureBuffer;
		UINT m_SRVHandle;
		unique_ptr<SRVBuffer> m_SRV;
		wstring m_Filename;
	public:

		TextureBuffer() {}
		~TextureBuffer() {}

		void Create(ComPtr<ID3D12Device>& _dev,const wstring& filename );
		ComPtr<ID3D12Resource> GetBuffer()const { return m_TextureBuffer; }
		void SetSRVHandle(UINT handle) { m_SRVHandle = handle; }
		UINT GetSRVHandle()const { return m_SRVHandle; }
		unique_ptr<SRVBuffer>& GetSRV() { return m_SRV; }

		UINT GetGPUHandle()const {

		}
	};
}

