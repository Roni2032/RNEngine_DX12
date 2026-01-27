#include "stdafx.h"
#include "ImageRenderer.h"

#include "TextureResource.h"
namespace RNEngine {
	ImageRenderer::ImageRenderer(const shared_ptr<GameObject>& ptr) :RendererComponent(ptr) {
		m_Texture = make_shared<TextureResource>();
		m_Pivot = GetAnchorNormalize(Anchor::Center);
	}
	ImageRenderer::~ImageRenderer() {}

	void ImageRenderer::SetTexture(const wstring& filename){
		m_Texture->SetTexture(filename);
	}
	void ImageRenderer::Update() {
		//Šî–{î•ñ‚ðXV
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