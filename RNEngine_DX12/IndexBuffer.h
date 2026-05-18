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
      /// <summary>
		/// コンストラクタ
		/// </summary>
		IndexBuffer();
		/// <summary>
		/// デストラクタ
		/// </summary>
		~IndexBuffer();

		/// <summary>
		/// インデックスバッファを作成する。
		/// </summary>
		/// <param name="_dev">D3D12 デバイス</param>
		/// <param name="index">インデックス配列</param>
		void Create(ID3D12Device* _dev, const vector<UINT>& index);
		/// <summary>
		/// インデックス数を取得する。
		/// </summary>
		/// <returns>インデックスの個数を返します。</returns>
		size_t GetIndexCount()const { return m_Indices.size(); }
		/// <summary>
		/// インデックスバッファビューを取得する。
		/// </summary>
		/// <returns>D3D12_INDEX_BUFFER_VIEW の参照を返します。</returns>
		const D3D12_INDEX_BUFFER_VIEW& GetBufferView() { return m_IndexBufferView; }
	};
}