#pragma once
#include "stdafx.h"
namespace RNEngine {

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
}

