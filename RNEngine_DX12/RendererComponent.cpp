#include "stdafx.h"
#include "project.h"

namespace RNEngine {
	void RendererComponent::Init(const shared_ptr<Camera>& camera) {
		m_Matrix.m_World = XMMatrixRotationY(0);

		auto viewMat = camera->GetViewMatrix();
		auto projMat = XMMatrixPerspectiveFovLH(XM_PIDIV2,//âÊäpÇÕ90Åã
			static_cast<float>(1280) / static_cast<float>(720),//ÉAÉXî‰
			0.1f,//ãﬂÇ¢ï˚
			1000.0f//âìÇ¢ï˚
		);
		m_Matrix.m_ViewProjection = viewMat * projMat;
		m_TargetCamera = camera;

		RegisterConstantBuffer(&m_Matrix, sizeof(m_Matrix));
	}

	void RendererComponent::Update() {
		if (auto camera = m_TargetCamera.lock()) {
			m_Matrix.m_ViewProjection = camera->GetViewProjectionMatrix();
		}
		auto gameObject = GetOwner();
		if (gameObject) {
			auto transform = gameObject->GetComponent<Transform>();
			UpdateWorldMatrix(transform->GetPosition(), transform->GetScale(), transform->GetRotation());
		}
		for (size_t i = 0, size = m_ConstantBuffers.size(); i < size; ++i) {
			m_ConstantBuffers[i]->Upadte(m_ConstantDatas[i].m_Data, m_ConstantDatas[i].m_DataSize);
		}
	}

	void RendererComponent::UpdateWorldMatrix(Vector3 position, Vector3 scale, Vector3 rotation) {
		m_Matrix.m_World = XMMatrixScaling(scale.x, scale.y, scale.z);
		m_Matrix.m_World *= XMMatrixRotationRollPitchYaw(rotation.x,rotation.y,rotation.z);
		m_Matrix.m_World *= XMMatrixTranslation(position.x, position.y, position.z);
	}

	void RendererComponent::RegisterConstantBuffer(void* data, size_t size) {
		auto constantBuffer = make_unique<ConstBuffer>();
		auto dev = Engine::GetID3D12Device();
		constantBuffer->Create(dev, data);

		m_ConstantBuffers.push_back(move(constantBuffer));
		m_ConstantDatas.push_back({ data,size });
	}

	void ModelRenderer::SetModel(const string& filename) {
		m_Model = ResourceManager::GetModelData(filename);
	}

	void ModelRenderer::Draw(ID3D12GraphicsCommandList* cmdList, DescriptorHeap* heap) {
		m_Model->Draw(cmdList, heap, m_ConstantBuffers[0].get());
	}

	ImageRenderer::ImageRenderer(const shared_ptr<GameObject>& ptr) :RendererComponent(ptr) {
		m_Image = make_shared<Image>();
	}
	void ImageRenderer::Init(const shared_ptr<Camera>& camera) {
		m_Matrix.m_World = XMMatrixRotationZ(0);
		if (camera->IsOrthographic()) {
			camera->SetOrthographic(true);
		}
		m_Matrix.m_ViewProjection = camera->GetProjectionMatrix();
		m_TargetCamera = camera;
		RegisterConstantBuffer(&m_Matrix, sizeof(m_Matrix));
	}
	void ImageRenderer::SetTexture(const string& filename){
		m_Image->SetTexture(filename);
	}
	void ImageRenderer::Draw(ID3D12GraphicsCommandList* cmdList, DescriptorHeap* heap) {
		m_Image->Draw(cmdList, heap, m_ConstantBuffers[0].get());
	}
}