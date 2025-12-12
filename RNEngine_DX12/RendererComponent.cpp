#include "stdafx.h"
#include "project.h"

namespace RNEngine {
	RendererComponent::RendererComponent(const shared_ptr<GameObject>& ptr)
		: RendererComponent(ptr,{}) { }
	RendererComponent::RendererComponent(const shared_ptr<GameObject>& ptr, const Matrix& matrix)
		: m_Matrix(matrix), Component(ptr){}

	void RendererComponent::Init(const shared_ptr<Camera>& camera) {
		m_Matrix.m_World = XMMatrixRotationY(0);

		m_Matrix.m_ViewProjection = camera->GetViewProjectionMatrix();
		m_TargetCamera = camera;

		if (m_ConstantBuffers.empty()) {
			RegisterConstantBuffer(&m_Matrix, sizeof(m_Matrix));
		}
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
		UpdateConstantBuffers();
	}

	void RendererComponent::UpdateConstantBuffers() {
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
	void RendererComponent::SetPipelineState(const wstring& key) {
		m_PipelineState = PipelineStatePool::GetPipelineState(key);
	}
	void RendererComponent::DrawMesh(ID3D12GraphicsCommandList* cmdList, Mesh& mesh) {
		auto renderer = Engine::GetRenderer();

		//テクスチャが読み込めなかったとき用のテクスチャ(エラーテクスチャ)
		string textureName = "Textures/ErrorTexture.png";
		/*if (m_Materials.size() > mesh.m_MaterialIndex &&
			m_Materials[mesh.m_MaterialIndex].m_TextureType != TextureDataType::None) {
			textureName = m_Materials[mesh.m_MaterialIndex].m_TextureName;
		}*/

		auto texture = ResourceManager::GetTextureBuffer(textureName);
		if (texture) {
			auto handle = renderer->GetSRVDescriptorGPUHandle(texture->GetSRVHandle());
			cmdList->SetGraphicsRootDescriptorTable(1, handle);
		}

		cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		cmdList->IASetVertexBuffers(0, 1, &mesh.m_VertexBuffer->m_VertexBufferView);
		cmdList->IASetIndexBuffer(&mesh.m_IndexBuffer->m_IndexBufferView);

		cmdList->DrawIndexedInstanced((UINT)mesh.m_IndexBuffer->GetIndexCount(), 1, 0, 0, 0);
	}


	void ModelRenderer::SetModel(const string& filename) {
		m_Model = ResourceManager::GetModelData(filename);
	}
	shared_ptr<Model>& ModelRenderer::GetModel() {
		return m_Model;
	}

	void ModelRenderer::Update() {
		if (auto camera = m_TargetCamera.lock()) {
			m_Matrix.m_ViewProjection = camera->GetViewProjectionMatrix();
		}
		auto gameObject = GetOwner();
		if (gameObject) {
			auto transform = gameObject->GetComponent<Transform>();
			UpdateWorldMatrix(transform->GetPosition(), transform->GetScale(), transform->GetRotation());
		}
		float defaultScale = m_Model->GetDefaultScale();
		Vector3 defaultRotation = m_Model->GetDefaultRotation();
		XMMATRIX defaultTransform = XMMatrixScaling(defaultScale, defaultScale, defaultScale);
		defaultTransform *= XMMatrixRotationRollPitchYaw(defaultRotation.x, defaultRotation.y, defaultRotation.z);

		m_Matrix.m_World = defaultTransform * m_Matrix.m_World;
		UpdateConstantBuffers();
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