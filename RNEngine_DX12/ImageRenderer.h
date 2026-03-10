#pragma once
#include "stdafx.h"
#include "RendererComponent.h"

namespace RNEngine
{
	class DescriptorHeap;
	class TextureResource;
	class TextureBuffer;
	class GameObject;
	
	enum class Anchor {
		   TopLeft,    Top,    TopRight,
		      Left, Center,       Right,
		ButtomLeft, Buttom, ButtomRight
	};
	class ImageRenderer : public RendererComponent {
		shared_ptr<TextureResource> m_Texture;
		Vector2 m_Pivot;
	public:
		ImageRenderer(const shared_ptr<GameObject>& ptr);
		~ImageRenderer();
		void SetTexture(const wstring& filename);
		void SetTexture(const shared_ptr<TextureBuffer>& texture);

		TextureResource* GetTexture()const;
		virtual void Update()override;
		virtual void Draw(ID3D12GraphicsCommandList* cmdList, DescriptorHeap* heap)override;

		void SetPivot(const Vector2& pivot);
		void SetPivot(Anchor anchor);
		Vector2 GetPivot();

		Vector2 GetAnchorNormalize(Anchor anchor);
	};
}