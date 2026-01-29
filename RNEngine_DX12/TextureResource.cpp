#include "stdafx.h"
#include "TextureResource.h"
#include "ResourceManager.h"
#include "RendererHeader.h"
#include "PipelineState.h"

#include "Descriptor.h"
#include "ConstantBuffer.h"
#include "TextureBuffer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include "RNEngine.h"
#include "Renderer.h"

#include "Component.h"
#include "Camera.h"
namespace RNEngine {

	TextureResource::TextureResource() {
		m_Mesh = ResourceManager::GetMeshData("DEFAULT_SQUARE_2D");

		m_Matrix.m_World = XMMatrixRotationZ(0);
		RegisterConstantBuffer(&m_Matrix, sizeof(m_Matrix));
	}

	void TextureResource::SetPipelineState(const wstring& key) {
		m_PipelineState = PipelineStatePool::GetPipelineState(key);
	}
	void TextureResource::SetTexture(const wstring& key) {
		m_Texture = ResourceManager::GetTextureBuffer(Util::ConvertWstrToStr(key));
	}
	void TextureResource::SetTexture(const shared_ptr<TextureBuffer>& buffer) {
		m_Texture = buffer;
	}

	void TextureResource::SetCamera(const shared_ptr<Camera>& camera) {
		if (!camera) return;
		camera->SetOrthographic(true);
		m_Matrix.m_ViewProjection = camera->GetViewProjectionMatrix();
		camera->SetOrthographic(false);
	}

	void TextureResource::UpdateWorldMatrix(const shared_ptr<Transform>& transform) {
		UpdateWorldMatrix(transform->GetPosition(), transform->GetScale(), transform->GetRotation());
	}
	void TextureResource::UpdateWorldMatrix(const Vector3& position, const Vector3& scale, const Vector3& rotation) {
		m_Matrix.m_World = XMMatrixScaling(scale.x, scale.y, scale.z);
		m_Matrix.m_World *= XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
		m_Matrix.m_World *= XMMatrixTranslation(position.x, position.y, position.z);
	}

	void TextureResource::RegisterConstantBuffer(void* data, size_t size) {
		auto constantBuffer = make_unique<ConstantBuffer>();
		auto dev = Engine::GetID3D12Device();
		constantBuffer->Create(dev, data);

		m_ConstantBuffers.push_back(move(constantBuffer));
		m_ConstantDates.push_back({ data,size });
	}
	void TextureResource::CopyToGPU(shared_ptr<ConstantBuffer>& buffer, ConstantBufferData& data) {
		buffer->Update(data.m_Data, data.m_DataSize);
	}

	void TextureResource::Draw(ComPtr<ID3D12GraphicsCommandList> cmdList, DescriptorHeap* heap) {
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

		cmdList->SetDescriptorHeaps(1, heap->GetHeapAddress());

		auto startHandle = heap->GetGPUHandle();
		auto handle = renderer->GetSRVDescriptorGPUHandle(m_ConstantBuffers[0]->GetHandle());
		cmdList->SetGraphicsRootDescriptorTable(0, handle);

		handle = renderer->GetSRVDescriptorGPUHandle(m_Texture->GetSRVHandle());
		cmdList->SetGraphicsRootDescriptorTable(1, handle);

		cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		cmdList->IASetVertexBuffers(0, 1, &m_Mesh.m_VertexBuffer->GetBufferView());
		cmdList->IASetIndexBuffer(&m_Mesh.m_IndexBuffer->GetBufferView());

		cmdList->DrawIndexedInstanced((UINT)m_Mesh.m_IndexBuffer->GetIndexCount(), 1, 0, 0, 0);
	}
}
