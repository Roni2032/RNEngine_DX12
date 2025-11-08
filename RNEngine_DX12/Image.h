#pragma once
#include "stdafx.h"
#include "RendererHeader.h"
namespace RNEngine {

	class Image
	{
		Mesh m_Mesh;
		string m_TextureFilename;

		void Init();
	public:
		Image() { Init(); }
		~Image(){}

		void SetTexture(const string& filename) {
			m_TextureFilename = filename;
		}

		void Draw(ComPtr<ID3D12GraphicsCommandList> cmdList, DescriptorHeap* heap, const ConstBuffer* constantBuffer);
	};
}


