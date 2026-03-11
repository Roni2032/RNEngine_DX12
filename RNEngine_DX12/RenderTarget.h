#pragma once
#include "stdafx.h"

namespace RNEngine {
	class TextureResource;
	class RTVBuffer;
	class DSVBuffer;

	class RenderTarget {
		shared_ptr<TextureResource> m_RenderTargetTexture;
		unique_ptr<RTVBuffer> m_Rtv;
		unique_ptr<DSVBuffer> m_Dsv;

		float m_Width;
		float m_Height;

		DXGI_FORMAT m_Format{};
		array<float, 4> m_ClearColor;
	public:
		RenderTarget();
		~RenderTarget();
		void Create(Vector2 renderSize, DXGI_FORMAT format, array<float, 4> clearColor = { 1.0f,1.0f,1.0f,0.0f });

		void DrawBegin(ID3D12GraphicsCommandList* cmdList);
		void DrawEnd(ID3D12GraphicsCommandList* cmdList);
		void Draw(vector<shared_ptr<RendererComponent>>& renderers);

		void SetClearColor(const Color& color);

		RTVBuffer* GetRTVBuffer() { return m_Rtv.get(); }

		shared_ptr<TextureResource> GetRenderTargetTexture();
	};
}


