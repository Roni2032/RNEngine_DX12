#include "TextureBuffer.h"
#include "Buffer.h"
#include "SRV.h"
#include "EditorGUI.h"
#include "RNEngine.h"

namespace RNEngine {
	TextureBuffer::TextureBuffer()
		:m_Handle(0),
		m_IsExistsFile(false) {}

	TextureBuffer::~TextureBuffer() {}

	void TextureBuffer::Create(ID3D12Device* _dev, const wstring& filename) {

		TexMetadata metadata;
		ScratchImage image;

		HRESULT result = LoadFromWICFile(
			filename.c_str(),
			WIC_FLAGS_NONE,
			&metadata,
			image
		);
		if (FAILED(result)) {
			DebugLog::Log(u8"テクスチャの読み込みに失敗しました[" +
				Util::ConvertWstrToStr(filename) + "]",
				LogData::Type::Error);
			return;
		}
		auto img = image.GetImage(0, 0, 0);

		CreateResource((UINT)metadata.width, (UINT)metadata.height, metadata.format);

		result = m_Buffer->WriteToSubresource(
			0,
			nullptr,
			img->pixels,
			(UINT)img->rowPitch,
			(UINT)img->slicePitch
		);
		if (FAILED(result)) {
			DebugLog::Log("テクスチャのアップロードに失敗しました[" +
				Util::ConvertWstrToStr(filename) + "]",
				LogData::Type::Error);
			return;
		}

		m_Filename = filename;
		m_IsExistsFile = true;
	}
	void TextureBuffer::Create(ID3D12Device* _dev, UINT width, UINT height, DXGI_FORMAT format, array<float, 4> clearColor) {
		CreateResource(width, height, format, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET, clearColor);
		m_IsExistsFile = false;
	}
	void TextureBuffer::Create(ID3D12Device* _dev, const uint8_t* data, size_t dataSize) {
		if (dataSize == 0) return;

		TexMetadata metadata;
		ScratchImage image;
		HRESULT result = LoadFromWICMemory(
			data,
			dataSize,
			WIC_FLAGS_NONE,
			&metadata,
			image
		);

		assert(SUCCEEDED(result));
		auto img = image.GetImage(0, 0, 0);

		CreateResource((UINT)metadata.width, (UINT)metadata.height, metadata.format);

		result = m_Buffer->WriteToSubresource(
			0,
			nullptr,
			img->pixels,
			(UINT)img->rowPitch,
			(UINT)img->slicePitch
		);
		assert(SUCCEEDED(result));

		m_IsExistsFile = false;
	}
	void TextureBuffer::CreateResource(UINT width, UINT height, DXGI_FORMAT format, D3D12_RESOURCE_FLAGS flag, array<float, 4> clearColor) {

		D3D12_HEAP_PROPERTIES heapProp = {};
		heapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
		heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
		heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
		heapProp.CreationNodeMask = 0;
		heapProp.VisibleNodeMask = 0;

		CD3DX12_RESOURCE_DESC desc(
			D3D12_RESOURCE_DIMENSION_TEXTURE2D,
			0,
			width,
			height,
			1, 1,
			format,
			1, 0,
			D3D12_TEXTURE_LAYOUT_UNKNOWN,
			flag
		);


		D3D12_CLEAR_VALUE clearValue;
		clearValue.Format = format;
		clearValue.Color[0] = clearColor[0];
		clearValue.Color[1] = clearColor[1];
		clearValue.Color[2] = clearColor[2];
		clearValue.Color[3] = clearColor[3];

		auto dev = Engine::GetID3D12Device();
		if (flag != D3D12_RESOURCE_FLAG_NONE) {
			auto result = dev->CreateCommittedResource(
				&heapProp,
				D3D12_HEAP_FLAG_NONE,
				&desc,
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
				&clearValue,
				IID_PPV_ARGS(m_Buffer.GetAddressOf()));

			assert(SUCCEEDED(result));
		}
		else {
			auto result = dev->CreateCommittedResource(
				&heapProp,
				D3D12_HEAP_FLAG_NONE,
				&desc,
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
				nullptr,
				IID_PPV_ARGS(m_Buffer.GetAddressOf()));

			assert(SUCCEEDED(result));
		}

		//SRVを作成
		m_Srv = make_unique<SRVBuffer>();
		m_Srv->Init(dev, this, format);
	}

	bool TextureBuffer::IsExistsTexture() {
		return m_Buffer.Get() != nullptr;
	}
}