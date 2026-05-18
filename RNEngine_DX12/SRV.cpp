#include "stdafx.h"
#include "SRV.h"
#include "RNEngine.h"
#include "Renderer.h"
#include "TextureBuffer.h"

namespace RNEngine {
	SRVBuffer::SRVBuffer() {}
	SRVBuffer::~SRVBuffer() {}

	void SRVBuffer::CreateSRVDesc(ID3D12Device* _dev, DXGI_FORMAT format) {
		ZeroMemory(&m_Desc, sizeof(m_Desc));
		m_Desc.Format = format;
		m_Desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		m_Desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		m_Desc.Texture2D.MipLevels = 1;
	}
	void SRVBuffer::Init(ID3D12Device* _dev, TextureBuffer* texBuffer, DXGI_FORMAT format) {
		CreateSRVDesc(_dev, format);

		auto renderer = Engine::GetRenderer();
		renderer->RegisterTextureBuffer(texBuffer);
	}
	void SRVBuffer::Init(ID3D12Device* _dev, DXGI_FORMAT format) {
		CreateSRVDesc(_dev, format);
	}
}