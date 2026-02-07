#include "stdafx.h"
#include "ModelRenderer.h"
#include "ResourceManager.h"
#include "Model.h"
#include "GameObject.h"
#include "Camera.h"
#include "Component.h"

namespace RNEngine {
	ModelRenderer::ModelRenderer(const shared_ptr<GameObject>& ptr) :RendererComponent(ptr) {}
	ModelRenderer::~ModelRenderer() = default;

	void ModelRenderer::SetModel(const string& filename) {
		m_Model = ResourceManager::GetModelResource(filename);
	}
	shared_ptr<ModelResource>& ModelRenderer::GetModel() {
		return m_Model;
	}

	void ModelRenderer::Update() {
		auto gameObject = GetOwner();
		if (gameObject) {
			auto transform = gameObject->GetTransform();
			m_Model->UpdateWorldMatrix(transform);
		}
	}
	void ModelRenderer::Draw(ID3D12GraphicsCommandList* cmdList, DescriptorHeap* heap) {
		//Šî–{î•ñ‚ðXV
		m_Model->SetCamera(m_TargetCamera.lock());

		//•`‰æ
		m_Model->Draw(cmdList, heap);
	}
}