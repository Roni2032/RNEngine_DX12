#pragma once
#include "stdafx.h"
#include "RendererHeader.h"
namespace RNEngine {
	class TextureBuffer;
	class SwapChain;
	class Window;

	class DescriptorHeap {
		ComPtr<ID3D12DescriptorHeap> m_Heap;
		UINT m_HeapSize;
		UINT m_HeapCount; //ŽŸ‚É’Ç‰Á‚·‚éƒq[ƒv”Ô†(-1‚µ‚½•ª‚¾‚¯“o˜^Ï‚Ý)
	public:
		DescriptorHeap() :m_HeapSize(0), m_HeapCount(0){}
		~DescriptorHeap(){}
		void Reset() { m_HeapCount = 0; }

		bool Init(ID3D12Device* _dev,UINT _frameBufferCount,D3D12_DESCRIPTOR_HEAP_TYPE _type,D3D12_DESCRIPTOR_HEAP_FLAGS _flags);
		ID3D12DescriptorHeap* GetHeap()const { return m_Heap.Get(); }
		ID3D12DescriptorHeap**  GetHeapAddress(){ return m_Heap.GetAddressOf(); }
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

		void CreateRTV(ID3D12Device* _dev, UINT count);
	public:
		D3D12_RENDER_TARGET_VIEW_DESC m_RTVDesc;
		RTVBuffer() {}
		RTVBuffer(ID3D12Device* _dev, SwapChain* _swapChain) { InitFrameBuffer(_dev,_swapChain); }
		~RTVBuffer() { }

		void InitFrameBuffer(ID3D12Device* _dev, SwapChain* _swapChain);
		void Init(ID3D12Device* _dev);

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
		DescriptorHeap* GetDecsriptorHeap() { return m_RTVHeap.get(); }
	};
	class DSVBuffer {
		unique_ptr<DescriptorHeap> m_DSVHeap;
		ComPtr<ID3D12Resource> m_DSBuffer;

		void CreateDSVDesc(ID3D12Device* _dev);
	public:
		D3D12_DEPTH_STENCIL_VIEW_DESC m_DSVDesc{};
		DSVBuffer() {}
		DSVBuffer(ID3D12Device* _dev,const Window* _window) { Init(_dev,_window); }
		~DSVBuffer() { }

		void Init(ID3D12Device* _dev, const Window* _window);

		DescriptorHeap* GetDecsriptorHeap() { return m_DSVHeap.get(); }

	};
	class SRVBuffer {
	public:
		D3D12_SHADER_RESOURCE_VIEW_DESC m_SRVDesc{};
		SRVBuffer(){}
		~SRVBuffer() {}
		void Init(ID3D12Device* _dev, TextureBuffer& texBuffer,DXGI_FORMAT format);
		void Init(ID3D12Device* _dev, DXGI_FORMAT format);
		void CreateSRVDesc(ID3D12Device* _dev, DXGI_FORMAT format);
	};
	class ConstBuffer {
		ComPtr<ID3D12Resource> m_ConstBuffer;
		UINT m_CBVHandle;

		uint8_t* m_MappedData;
		size_t m_BufferSize;
	public:
		D3D12_CONSTANT_BUFFER_VIEW_DESC m_CBVDesc{};

		ConstBuffer():m_CBVHandle(0), m_MappedData(nullptr), m_BufferSize(0){}
		~ConstBuffer() {}

		void Create(ID3D12Device* _dev, void* matrix);

		ComPtr<ID3D12Resource> GetBuffer() { return m_ConstBuffer; }
		void SetCBVHandle(UINT handle) { m_CBVHandle = handle; }
		UINT GetCBVHandle()const { return m_CBVHandle; }

		void Upadte(void* data,size_t size);
	};

	class VertexBuffer {
		ComPtr<ID3D12Resource> m_VertexBuffer;
		vector<Vertex> m_VertexData;
		void InitVertexBufferView(const vector<Vertex>& vertex);
		void CreateVertexBuffer(ID3D12Device* _dev, const vector<Vertex>& vertex);
	public:
		D3D12_VERTEX_BUFFER_VIEW m_VertexBufferView{};

		VertexBuffer() {}
		~VertexBuffer() {}

		void Create(ID3D12Device* _dev, const vector<Vertex>& vertex);
		ComPtr<ID3D12Resource> GetBuffer() { return m_VertexBuffer; }

		size_t GetVertexCount()const { return m_VertexData.size(); }
	};
	class IndexBuffer {
		ComPtr<ID3D12Resource> m_IndexBuffer;
		vector<UINT> m_IndexData;
		void InitIndexBufferView(const vector<UINT>& index);
		void CreateIndexBuffer(ID3D12Device* _dev, const vector<UINT>& index);

	public:
		D3D12_INDEX_BUFFER_VIEW m_IndexBufferView{};

		IndexBuffer(){}
		~IndexBuffer() {}

		void Create(ID3D12Device* _dev, const vector<UINT>& index);
		size_t GetIndexCount()const { return m_IndexData.size(); }
	};

	class TextureBuffer {
		ComPtr<ID3D12Resource> m_TextureBuffer;
		UINT m_SRVHandle;
		unique_ptr<SRVBuffer> m_SRV;
		wstring m_Filename;
	public:

		TextureBuffer():m_SRVHandle(0){}
		~TextureBuffer() {}

		void Create(ID3D12Device* _dev,const wstring& filename );
		void Create(ID3D12Device* _dev, UINT width, UINT height, DXGI_FORMAT format, array<float, 4> clearColor);
		void CreateResource(UINT width, UINT height, DXGI_FORMAT format, D3D12_RESOURCE_FLAGS flag = D3D12_RESOURCE_FLAG_NONE, array<float, 4> clearColor = { 0,0,0,1 });

		ID3D12Resource* GetBuffer()const { return m_TextureBuffer.Get(); }
		void SetSRVHandle(UINT handle) { m_SRVHandle = handle; }
		UINT GetSRVHandle()const { return m_SRVHandle; }
		SRVBuffer* GetSRV() { return m_SRV.get(); }
	};
}

