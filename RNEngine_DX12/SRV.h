#pragma once
#include "stdafx.h"

namespace RNEngine {
	class TextureBuffer;

	class SRVBuffer : public DescBuffer<D3D12_SHADER_RESOURCE_VIEW_DESC> {
	public:
		SRVBuffer();
		~SRVBuffer();
		void Init(ID3D12Device* _dev, TextureBuffer* texBuffer, DXGI_FORMAT format);
		void Init(ID3D12Device* _dev, DXGI_FORMAT format);
		void CreateSRVDesc(ID3D12Device* _dev, DXGI_FORMAT format);
	};
}