#pragma once
#include "stdafx.h"
namespace RNEngine {

	class BufferBase {
	protected:
		ComPtr<ID3D12Resource> m_Buffer;
	public:
      /// <summary>
		/// 内部が保持する D3D12 リソース（バッファ）のポインタを取得する。
		/// </summary>
		/// <returns>ID3D12Resource の生ポインタを返します（所有権は渡しません）。</returns>
		ID3D12Resource* GetBuffer() { return m_Buffer.Get(); }
	};
	template<class DESC>
	class DescBuffer : public BufferBase {
	protected:
		DESC m_Desc;

		void InitDesc(DESC& desc) { m_Desc = desc; }
	public:
        /// <summary>
		/// バッファの記述情報を取得する。
		/// </summary>
		/// <returns>内部に保持している記述情報の参照を返します。</returns>
		const DESC& GetDesc() { return m_Desc; }
	};
}

