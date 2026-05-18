#pragma once
#include "stdafx.h"
#include "Buffer.h"
namespace RNEngine {
	class ConstantBuffer : public DescBuffer<D3D12_CONSTANT_BUFFER_VIEW_DESC> {
		UINT m_Handle;

		uint32_t* m_MappedData;
		size_t m_BufferSize;
	public:
       /// <summary>
		/// コンストラクタ
		/// </summary>
		ConstantBuffer();
		/// <summary>
		/// デストラクタ
		/// </summary>
		~ConstantBuffer();

		/// <summary>
		/// 定数バッファを作成して初期データを書き込む。
		/// </summary>
		/// <param name="_dev">D3D12 デバイス</param>
		/// <param name="data">初期データへのポインタ</param>
		void Create(ID3D12Device* _dev, void* data);

		/// <summary>
		/// 内部ハンドルを設定する。
		/// </summary>
		/// <param name="handle">ヒープ上のハンドルインデックス</param>
		void SetHandle(UINT handle) { m_Handle = handle; }
		/// <summary>
		/// 設定されているハンドルを取得する。
		/// </summary>
		/// <returns>ハンドルインデックスを返します。</returns>
		UINT GetHandle()const { return m_Handle; }

		/// <summary>
		/// バッファ内容を更新する。
		/// </summary>
		/// <param name="data">更新データへのポインタ</param>
		/// <param name="size">更新サイズ（バイト）</param>
		void Update(void* data, size_t size);
	};
}