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
		UINT m_HeapCount; //次に追加するヒープ番号(-1した分だけ登録済み)
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

	class BufferBase {
	protected:
		ComPtr<ID3D12Resource> m_Buffer;
	public:
		ID3D12Resource* GetBuffer() { return m_Buffer.Get(); }
	};
	template<class DESC>
	class DescBuffer : public BufferBase {
	protected:
		DESC m_Desc;

		void InitDesc(DESC& desc) { m_Desc = desc; }
	public:
		const DESC& GetDesc() { return m_Desc; }
	};


	class RTVBuffer {
		unique_ptr<DescriptorHeap> m_RTVHeap;
		vector<ComPtr<ID3D12Resource>> m_BackBuffer;
		vector<D3D12_RESOURCE_STATES> m_BufferStates;

		void CreateRTV(ID3D12Device* _dev, UINT count);
	public:
		D3D12_RENDER_TARGET_VIEW_DESC m_RTVDesc{};
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
		DescriptorHeap* GetDescriptorHeap() { return m_RTVHeap.get(); }
	};
	class DSVBuffer : public DescBuffer<D3D12_DEPTH_STENCIL_VIEW_DESC> {
		unique_ptr<DescriptorHeap> m_DSVHeap;

		void CreateDSVDesc(ID3D12Device* _dev);
	public:
		DSVBuffer() {}
		DSVBuffer(ID3D12Device* _dev, const Window* _window) { Init(_dev, _window); }

		void Init(ID3D12Device* _dev, const Window* _window);

		DescriptorHeap* GetDescriptorHeap() { return m_DSVHeap.get(); }
	};
	class SRVBuffer : public DescBuffer<D3D12_SHADER_RESOURCE_VIEW_DESC> {
	public:
		SRVBuffer() {}
		~SRVBuffer() {}
		void Init(ID3D12Device* _dev, TextureBuffer& texBuffer, DXGI_FORMAT format);
		void Init(ID3D12Device* _dev, DXGI_FORMAT format);
		void CreateSRVDesc(ID3D12Device* _dev, DXGI_FORMAT format);
	};
	class ConstantBuffer : public DescBuffer<D3D12_CONSTANT_BUFFER_VIEW_DESC>{
		UINT m_Handle;

		uint32_t* m_MappedData;
		size_t m_BufferSize;
	public:
		ConstantBuffer() :m_Handle(0), m_MappedData(nullptr), m_BufferSize(0) {}
		~ConstantBuffer() {}

		void Create(ID3D12Device* _dev, void* matrix);

		void SetHandle(UINT handle) { m_Handle = handle; }
		UINT GetHandle()const { return m_Handle; }

		void Update(void* data, size_t size);
	};
	/*template<class VertexData>
	class VertexBuffer : public BufferBase {
		vector<VertexData> m_Vertex;
		D3D12_VERTEX_BUFFER_VIEW m_VertexBufferView{};

		void InitBufferView() {
			m_VertexBufferView.BufferLocation = m_VertexBuffer->GetGPUVirtualAddress();
			m_VertexBufferView.SizeInBytes = sizeof(Vertex) * (UINT)m_Vertex.size();
			m_VertexBufferView.StrideInBytes = sizeof(m_Vertex[0]);
		}
		void CreateBuffer(ID3D12Device* _dev, const vector<VertexData>& vertex) {
			D3D12_HEAP_PROPERTIES heapProp = {};
			heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
			heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
			heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

			D3D12_RESOURCE_DESC resDesc = {};
			resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
			resDesc.Width = sizeof(VertexData) * vertex.size();
			resDesc.Height = 1;
			resDesc.DepthOrArraySize = 1;
			resDesc.MipLevels = 1;
			resDesc.SampleDesc.Count = 1;
			resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
			resDesc.Format = DXGI_FORMAT_UNKNOWN;
			resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

			HRESULT result = _dev->CreateCommittedResource(
				&heapProp,
				D3D12_HEAP_FLAG_NONE,
				&resDesc,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(m_Buffer.GetAddressOf()));

			if (FAILED(result)) {
				_com_error err(result);
				wprintf(L"CreateCommittedResource failed: 0x%08X (%s)\n", result, err.ErrorMessage());
			}
			assert(SUCCEEDED(result));

			Vertex* vertMap = nullptr;

			result = m_Buffer->Map(0, nullptr, (void**)&vertMap);
			copy(vertex.begin(), vertex.end(), vertMap);
			m_Buffer->Unmap(0, nullptr);
		}

	public:
		VertexBuffer() {}
		~VertexBuffer() {}

		void Create(ID3D12Device* _dev, const vector<VertexData>& vertex) {
			CreateBuffer(_dev, vertex);
			InitBufferView();
			m_Vertex = vertex;
		}

		size_t GetVertexCount()const { return m_Vertex.size(); }
		const D3D12_VERTEX_BUFFER_VIEW& GetBufferView() { return m_VertexBufferView; }
	};*/
	class VertexBuffer : public BufferBase {
		vector<Vertex> m_Vertex;
		D3D12_VERTEX_BUFFER_VIEW m_VertexBufferView{};

		void InitVertexBufferView();
		void CreateVertexBuffer(ID3D12Device* _dev, const vector<Vertex>& vertex);

	public:
		VertexBuffer() {}
		~VertexBuffer() {}

		void Create(ID3D12Device* _dev, const vector<Vertex>& vertex);

		size_t GetVertexCount()const { return m_Vertex.size(); }
		const D3D12_VERTEX_BUFFER_VIEW& GetBufferView() { return m_VertexBufferView; }
	};
	class IndexBuffer : public BufferBase {
		vector<UINT> m_Indices;
		D3D12_INDEX_BUFFER_VIEW m_IndexBufferView{};

		void InitIndexBufferView();
		void CreateIndexBuffer(ID3D12Device* _dev, const vector<UINT>& index);
	public:

		void Create(ID3D12Device* _dev, const vector<UINT>& index);
		size_t GetIndexCount()const { return m_Indices.size(); }
		const D3D12_INDEX_BUFFER_VIEW& GetBufferView() { return m_IndexBufferView; }
	};
	class TextureBuffer : public BufferBase {
		UINT m_Handle;
		unique_ptr<SRVBuffer> m_Srv;
		wstring m_Filename;

		bool m_IsExistsFile;
	public:
		TextureBuffer() :m_Handle(0), m_IsExistsFile(true) {}
		~TextureBuffer() {}

		/// <summary>
		/// ファイルからテクスチャを作成
		/// </summary>
		/// <param name="_dev">DX12デバイス</param>
		/// <param name="filename">ファイルパス</param>
		void Create(ID3D12Device* _dev, const wstring& filename);

		/// <summary>
		/// 指定したサイズ、フォーマットでテクスチャを作成
		/// </summary>
		/// <param name="_dev">DX12デバイス</param>
		/// <param name="width">幅</param>
		/// <param name="height">高さ</param>
		/// <param name="format">カラーフォーマット</param>
		/// <param name="clearColor">初期カラー</param>
		void Create(ID3D12Device* _dev, UINT width, UINT height, DXGI_FORMAT format, array<float, 4> clearColor);

		/// <summary>
		/// 内部埋め込みデータからテクスチャを作成
		/// </summary>
		/// <param name="_dev">DX12デバイス</param>
		/// <param name="data">埋め込みデータ</param>
		/// <param name="dataSize">データサイズ</param>
		void Create(ID3D12Device* _dev, const uint8_t* data, size_t dataSize);

		/// <summary>
		/// リソースの作成
		/// </summary>
		/// <param name="width">幅</param>
		/// <param name="height">高さ</param>
		/// <param name="format">カラーフォーマット</param>
		/// <param name="flag">リソースフラグ</param>
		/// <param name="clearColor">初期化カラー</param>
		void CreateResource(UINT width, UINT height, DXGI_FORMAT format, D3D12_RESOURCE_FLAGS flag = D3D12_RESOURCE_FLAG_NONE, array<float, 4> clearColor = { 0,0,0,1 });

		/// <summary>
		/// バッファを取得
		/// </summary>
		/// <returns>テクスチャバッファ</returns>
		ID3D12Resource* GetBuffer()const { return m_Buffer.Get(); }

		/// <summary>
		/// テクスチャが存在するかどうか
		/// </summary>
		/// <returns></returns>
		bool IsExistsTexture() {
			return m_Buffer.Get() != nullptr;
		}

		/// <summary>
		/// SRVハンドルの設定
		/// </summary>
		/// <param name="handle">ハンドル</param>
		void SetSRVHandle(UINT handle) { m_Handle = handle; }

		/// <summary>
		/// SRVハンドルの取得
		/// </summary>
		/// <returns>SRVハンドル</returns>
		UINT GetSRVHandle()const { return m_Handle; }

		/// <summary>
		/// SRVの取得
		/// </summary>
		/// <returns>SRV</returns>
		SRVBuffer* GetSRV() { return m_Srv.get(); }
	};
	//class TextureBuffer {
	//	ComPtr<ID3D12Resource> m_TextureBuffer;	//テクスチャバッファ
	//	UINT m_SRVHandle;						//SRVのハンドル番号
	//	unique_ptr<SRVBuffer> m_SRV;			//SRV
	//	wstring m_Filename;						//ファイル名

	//	bool m_IsExistsFile;					//ファイルから作成されたかどうか
	//public:

	//	TextureBuffer():m_SRVHandle(0), m_IsExistsFile(true){}
	//	~TextureBuffer() {}

	//	/// <summary>
	//	/// ファイルからテクスチャを作成
	//	/// </summary>
	//	/// <param name="_dev">DX12デバイス</param>
	//	/// <param name="filename">ファイルパス</param>
	//	void Create(ID3D12Device* _dev,const wstring& filename );

	//	/// <summary>
	//	/// 指定したサイズ、フォーマットでテクスチャを作成
	//	/// </summary>
	//	/// <param name="_dev">DX12デバイス</param>
	//	/// <param name="width">幅</param>
	//	/// <param name="height">高さ</param>
	//	/// <param name="format">カラーフォーマット</param>
	//	/// <param name="clearColor">初期カラー</param>
	//	void Create(ID3D12Device* _dev, UINT width, UINT height, DXGI_FORMAT format, array<float, 4> clearColor);

	//	/// <summary>
	//	/// 内部埋め込みデータからテクスチャを作成
	//	/// </summary>
	//	/// <param name="_dev">DX12デバイス</param>
	//	/// <param name="data">埋め込みデータ</param>
	//	/// <param name="dataSize">データサイズ</param>
	//	void Create(ID3D12Device* _dev, const uint8_t* data, size_t dataSize);

	//	/// <summary>
	//	/// リソースの作成
	//	/// </summary>
	//	/// <param name="width">幅</param>
	//	/// <param name="height">高さ</param>
	//	/// <param name="format">カラーフォーマット</param>
	//	/// <param name="flag">リソースフラグ</param>
	//	/// <param name="clearColor">初期化カラー</param>
	//	void CreateResource(UINT width, UINT height, DXGI_FORMAT format, D3D12_RESOURCE_FLAGS flag = D3D12_RESOURCE_FLAG_NONE, array<float, 4> clearColor = { 0,0,0,1 });

	//	/// <summary>
	//	/// バッファを取得
	//	/// </summary>
	//	/// <returns>テクスチャバッファ</returns>
	//	ID3D12Resource* GetBuffer()const { return m_TextureBuffer.Get(); }

	//	/// <summary>
	//	/// テクスチャが存在するかどうか
	//	/// </summary>
	//	/// <returns></returns>
	//	bool IsExistsTexture() {
	//		return m_TextureBuffer.Get() != nullptr;
	//	}

	//	/// <summary>
	//	/// SRVハンドルの設定
	//	/// </summary>
	//	/// <param name="handle">ハンドル</param>
	//	void SetSRVHandle(UINT handle) { m_SRVHandle = handle; }

	//	/// <summary>
	//	/// SRVハンドルの取得
	//	/// </summary>
	//	/// <returns>SRVハンドル</returns>
	//	UINT GetSRVHandle()const { return m_SRVHandle; }

	//	/// <summary>
	//	/// SRVの取得
	//	/// </summary>
	//	/// <returns>SRV</returns>
	//	SRVBuffer* GetSRV() { return m_SRV.get(); }
	//};
}

