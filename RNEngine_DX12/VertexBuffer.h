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
     /// <summary>
		/// コンストラクタ
		/// </summary>
		VertexBuffer();
		/// <summary>
		/// デストラクタ
		/// </summary>
		~VertexBuffer();

		/// <summary>
		/// 頂点バッファを作成する。
		/// </summary>
		/// <param name="_dev">D3D12 デバイス</param>
		/// <param name="vertex">頂点配列</param>
		void Create(ID3D12Device* _dev, const vector<Vertex>& vertex);

		/// <summary>
		/// 頂点数を取得する。
		/// </summary>
		/// <returns>頂点の個数を返します。</returns>
		size_t GetVertexCount()const { return m_Vertex.size(); }
		/// <summary>
		/// 頂点バッファビューを取得する。
		/// </summary>
		/// <returns>D3D12_VERTEX_BUFFER_VIEW の参照を返します。</returns>
		const D3D12_VERTEX_BUFFER_VIEW& GetBufferView() { return m_VertexBufferView; }
	};
}