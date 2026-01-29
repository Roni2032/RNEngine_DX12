#include "stdafx.h"
#include "Model.h"
#include "Timer.h"
#include "Loders.h"
#include "RNEngine.h"
#include "Renderer.h"

#include "Descriptor.h"
#include "ConstantBuffer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "TextureBuffer.h"

#include "PipelineState.h"

#include "Camera.h"
#include "Component.h"

namespace RNEngine {
	ModelResource::ModelResource() :
		m_Filename(L""),m_Matrix({}),
		m_DefaultTransform({}) { }

	void ModelResource::Load(ID3D12Device* _dev, const string& filename) {
		Timer modelTimer = Timer();
		modelTimer.Init();

		AssimpLoader::Import(_dev, m_ModelData, Util::ConvertStrToWstr(filename));
		m_Filename = Util::ConvertStrToWstr(filename);

		RegisterConstantBuffer(&m_Matrix, sizeof(m_Matrix));
	}
	void ModelResource::Load(const Mesh& mesh) {
		m_ModelData.m_Meshes.resize(1);
		m_ModelData.m_Materials.resize(1);
		m_ModelData.m_Meshes[0] = mesh;

		RegisterConstantBuffer(&m_Matrix, sizeof(m_Matrix));
	}

	void ModelResource::Draw(ComPtr<ID3D12GraphicsCommandList> cmdList, DescriptorHeap* heap) {
		//描画前にデータをGPUに送信
		for (int i = 0; i < m_ConstantBuffers.size(); ++i) {
			CopyToGPU(m_ConstantBuffers[i], m_ConstantDates[i]);
		}
		if (!m_PipelineState) {
			//パイプラインステートが未設定の時は基本のやつを設定
			m_PipelineState = PipelineStatePool::GetPipelineState(L"Sample1");
		}

		cmdList->SetPipelineState(m_PipelineState->GetPtr());
		cmdList->SetGraphicsRootSignature(m_PipelineState->GetRootSignature()->GetPtr());

		//描画に必要なデータを設定・取得
		auto renderer = Engine::GetRenderer();
		auto& materials = m_ModelData.m_Materials;

		cmdList->SetDescriptorHeaps(1, heap->GetHeapAddress());

		//メッシュを一つずつ描画
		for (auto& mesh : m_ModelData.m_Meshes) {
			//コンスタントバッファの設定
			auto handle = renderer->GetSRVDescriptorGPUHandle(m_ConstantBuffers[0]->GetHandle());
			cmdList->SetGraphicsRootDescriptorTable(HeapType::CBV, handle);

			//テクスチャが読み込めなかったとき用のテクスチャ(エラーテクスチャ)
			string textureName = "Textures/ErrorTexture.png";
			if (materials.size() > mesh.m_MaterialIndex && !materials[mesh.m_MaterialIndex].IsEmpty()){
				//マテリアルが存在するときはマテリアルのテクスチャを使用
				textureName = materials[mesh.m_MaterialIndex].m_TextureName;
			}

			auto texture = ResourceManager::GetTextureBuffer(textureName);
			if (texture) {
				handle = renderer->GetSRVDescriptorGPUHandle(texture->GetSRVHandle());
				cmdList->SetGraphicsRootDescriptorTable(HeapType::SRV, handle);
			}

			cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			cmdList->IASetVertexBuffers(0, 1, &mesh.m_VertexBuffer->GetBufferView());
			cmdList->IASetIndexBuffer(&mesh.m_IndexBuffer->GetBufferView());

			cmdList->DrawIndexedInstanced((UINT)mesh.m_IndexBuffer->GetIndexCount(), 1, 0, 0, 0);
		}
	}
	void ModelResource::SetMaterial(const string& key, int index) {
		if (m_ModelData.m_Materials.size() <= index) {
			return;
		}
		m_ModelData.m_Materials[index] = Material(key);
	}
	Material& ModelResource::GetMaterial(int index) {
		return m_ModelData.m_Materials[index];
	}

	void ModelResource::SetPipelineState(const wstring& key) {
		m_PipelineState = PipelineStatePool::GetPipelineState(key);
	}

	void ModelResource::SetCamera(const shared_ptr<Camera>& camera) {
		if (!camera) return;
		m_Matrix.m_ViewProjection = camera->GetViewProjectionMatrix();
	}

	void ModelResource::UpdateWorldMatrix(const shared_ptr<Transform>& transform) {
		UpdateWorldMatrix(transform->GetPosition(), transform->GetScale(), transform->GetRotation());
	}
	void ModelResource::UpdateWorldMatrix(const Vector3& position, const Vector3& scale, const Vector3& rotation) {
		XMMATRIX defaultMatrix = XMMatrixScaling(m_DefaultTransform.m_Scale, m_DefaultTransform.m_Scale, m_DefaultTransform.m_Scale);
		defaultMatrix *= XMMatrixRotationRollPitchYaw(m_DefaultTransform.m_Rotation.x, m_DefaultTransform.m_Rotation.y, m_DefaultTransform.m_Rotation.z);
		defaultMatrix *= XMMatrixTranslation(m_DefaultTransform.m_Position.x, m_DefaultTransform.m_Position.y, m_DefaultTransform.m_Position.z);

		m_Matrix.m_World = XMMatrixScaling(scale.x, scale.y, scale.z); 
		m_Matrix.m_World *= XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z); 
		m_Matrix.m_World *= XMMatrixTranslation(position.x, position.y, position.z); 

		m_Matrix.m_World = defaultMatrix * m_Matrix.m_World;
	}

	void ModelResource::RegisterConstantBuffer(void* data, size_t size) {
		auto constantBuffer = make_unique<ConstantBuffer>();
		auto dev = Engine::GetID3D12Device();
		constantBuffer->Create(dev, data);

		m_ConstantBuffers.push_back(move(constantBuffer));
		m_ConstantDates.push_back({ data,size });
	}
	void ModelResource::CopyToGPU(shared_ptr<ConstantBuffer>& buffer, ConstantBufferData& data) {
		buffer->Update(data.m_Data, data.m_DataSize);
	}
}