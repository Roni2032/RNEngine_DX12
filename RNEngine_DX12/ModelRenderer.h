#pragma once
#include "stdafx.h"
#include "RendererComponent.h"

namespace RNEngine
{
	class GameObject;
	class ModelResource;
	class DescriptorHeap;

	class ModelRenderer : public RendererComponent{
		shared_ptr<ModelResource> m_Model;
	public:
		ModelRenderer(const shared_ptr<GameObject>& ptr);
		~ModelRenderer();

		void SetModel(const string& filename);

		shared_ptr<ModelResource>& GetModel();
		virtual void Update()override;
		virtual void Draw(ID3D12GraphicsCommandList* cmdList, DescriptorHeap* heap)override;

		REGISTER_NAME(ModelRenderer)
	};
}