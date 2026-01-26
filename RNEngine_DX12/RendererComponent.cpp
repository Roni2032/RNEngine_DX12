#include "stdafx.h"
#include "project.h"

namespace RNEngine {
	RendererComponent::RendererComponent(const shared_ptr<GameObject>& ptr)
		: Component(ptr) { }

	void RendererComponent::Init(const shared_ptr<Camera>& camera) {
		m_TargetCamera = camera;
	}

	void ModelRenderer::SetModel(const string& filename) {
		m_Model = ResourceManager::GetModelResource(filename);
	}
	shared_ptr<ModelResource>& ModelRenderer::GetModel() {
		return m_Model;
	}

	void ModelRenderer::Update() {
		//基本情報を更新
		m_Model->SetCamera(m_TargetCamera.lock());

		auto gameObject = GetOwner();
		if (gameObject) {
			auto transform = gameObject->GetComponent<Transform>();
			m_Model->UpdateWorldMatrix(transform);
		}
	}
	void ModelRenderer::Draw(ID3D12GraphicsCommandList* cmdList, DescriptorHeap* heap) {
		//描画
		m_Model->Draw(cmdList, heap);
	}

	ImageRenderer::ImageRenderer(const shared_ptr<GameObject>& ptr) :RendererComponent(ptr) {
		m_Texture = make_shared<TextureResource>();
	}

	void ImageRenderer::SetTexture(const wstring& filename){
		m_Texture->SetTexture(filename);
	}
	void ImageRenderer::Update() {
		//基本情報を更新
		m_Texture->SetCamera(m_TargetCamera.lock());

		auto gameObject = GetOwner();
		if (gameObject) {
			auto transform = gameObject->GetComponent<Transform>();
			Vector3 scale = transform->GetScale();
			Vector3 position = transform->GetPosition() + Vector3(-m_Pivot) * scale;
			Vector3 rotation = transform->GetRotation();
			m_Texture->UpdateWorldMatrix(position,scale,rotation);
		}
	}
	void ImageRenderer::Draw(ID3D12GraphicsCommandList* cmdList, DescriptorHeap* heap) {
		m_Texture->Draw(cmdList, heap);
	}

	void ImageRenderer::SetPivot(const Vector2& pivot) {
		m_Pivot = pivot;
	}
	void ImageRenderer::SetPivot(Anchor anchor) {
		m_Pivot = GetAnchorNormalize(anchor);
	}
	Vector2 ImageRenderer::GetPivot() {
		return m_Pivot;
	}

	Vector2 ImageRenderer::GetAnchorNormalize(Anchor anchor) {
		Vector2 pivot = Vector2();
		switch (anchor) {
		case Anchor::TopLeft:	pivot = Vector2(-0.5f, -0.5f); break;
		case Anchor::Top:		pivot = Vector2( 0.0f, -0.5f); break;
		case Anchor::TopRight:	pivot = Vector2( 0.5f, -0.5f); break;
		case Anchor::Left:		pivot = Vector2(-0.5f,  0.0f); break;
		case Anchor::Center:	pivot = Vector2( 0.0f,  0.0f); break;
		case Anchor::Right:		pivot = Vector2( 0.5f,  0.0f); break;
		case Anchor::ButtomLeft:pivot = Vector2(-0.5f,  0.5f); break;
		case Anchor::Buttom:	pivot = Vector2( 0.0f,  0.5f); break;
		case Anchor::ButtomRight:pivot = Vector2(0.5f,  0.5f); break;
		}
		return pivot;
	}
}