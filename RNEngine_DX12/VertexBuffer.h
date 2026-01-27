#pragma once
#include "stdafx.h"
#include "Buffer.h"

namespace RNEngine {
	class VertexBuffer : public BufferBase {
		vector<Vertex> m_Vertex;
		D3D12_VERTEX_BUFFER_VIEW m_VertexBufferView{};

		void InitVertexBufferView();
		void CreateVertexBuffer(ID3D12Device* _dev, const vector<Vertex>& vertex);

	public:
		VertexBuffer();
		~VertexBuffer();

		void Create(ID3D12Device* _dev, const vector<Vertex>& vertex);

		size_t GetVertexCount()const { return m_Vertex.size(); }
		const D3D12_VERTEX_BUFFER_VIEW& GetBufferView() { return m_VertexBufferView; }
	};
}