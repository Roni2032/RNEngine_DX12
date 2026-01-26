#pragma once
#include "stdafx.h"
#include "RendererHeader.h"
#include "Component.h"
namespace RNEngine
{
	class DescriptorHeap;
	class ModelResource;
	class ConstantBuffer;
	class TextureResource;
	class Camera;

	class RendererComponent : public Component{
	protected:
		weak_ptr<Camera> m_TargetCamera;
		vector<string> m_RenderTargetTag;
	public:
		RendererComponent(const shared_ptr<GameObject>& ptr);
		~RendererComponent(){}

		void Init(const shared_ptr<Camera>& camera);

		virtual void Update()override{}
		virtual void Draw(ID3D12GraphicsCommandList* cmdList, DescriptorHeap* heap){}

		void AddRenderTargetTag(const string& tag) {
			m_RenderTargetTag.push_back(tag);
		}
		vector<string> GetRenderTargetTag()const { return m_RenderTargetTag; }
	};

	class ModelRenderer : public RendererComponent{
		shared_ptr<ModelResource> m_Model;
	public:
		ModelRenderer(const shared_ptr<GameObject>& ptr):RendererComponent(ptr){}
		~ModelRenderer() {}

		void SetModel(const string& filename);

		shared_ptr<ModelResource>& GetModel();
		virtual void Update()override;
		virtual void Draw(ID3D12GraphicsCommandList* cmdList, DescriptorHeap* heap)override;

		REGISTER_NAME(ModelRenderer)
	};


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
		~ImageRenderer(){}
		void SetTexture(const wstring& filename);
		virtual void Update()override;
		virtual void Draw(ID3D12GraphicsCommandList* cmdList, DescriptorHeap* heap)override;

		void SetPivot(const Vector2& pivot);
		void SetPivot(Anchor anchor);
		Vector2 GetPivot();

		Vector2 GetAnchorNormalize(Anchor anchor);
	};
}