#pragma once
#include "stdafx.h"
#include "Buffer.h"

namespace RNEngine {
	class IndexBuffer : public BufferBase {
		vector<UINT> m_Indices;
		D3D12_INDEX_BUFFER_VIEW m_IndexBufferView{};

		void InitIndexBufferView();
		void CreateIndexBuffer(ID3D12Device* _dev, const vector<UINT>& index);
	public:
		IndexBuffer();
		~IndexBuffer();

		void Create(ID3D12Device* _dev, const vector<UINT>& index);
		size_t GetIndexCount()const { return m_Indices.size(); }
		const D3D12_INDEX_BUFFER_VIEW& GetBufferView() { return m_IndexBufferView; }
	};
}