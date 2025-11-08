#include "stdafx.h"
#include "project.h"

namespace RNEngine {

	void Image::Init() {
		m_Mesh = ResourceManager::GetMeshData("DEFAULT_SQUARE_2D");
	}
	void Image::Draw(ComPtr<ID3D12GraphicsCommandList> cmdList, DescriptorHeap* heap, const ConstBuffer* constantBuffer) {
		auto pipelineState = PipelineStatePool::GetPipelineState(L"Sample1");
		cmdList->SetPipelineState(pipelineState->GetPtr());
		cmdList->SetGraphicsRootSignature(pipelineState->GetRootSignature()->GetPtr());
		auto renderer = Engine::GetRenderer();

		cmdList->SetDescriptorHeaps(1, heap->GetHeapAddress());
		auto startHandle = heap->GetGPUHandle();
		auto handle = renderer->GetSRVDescriptorHandle(constantBuffer->GetCBVHandle());
		cmdList->SetGraphicsRootDescriptorTable(0, handle);

		auto texture = ResourceManager::GetTextureBuffer(m_TextureFilename);

		handle = renderer->GetSRVDescriptorHandle(texture->GetSRVHandle());
		cmdList->SetGraphicsRootDescriptorTable(1, handle);

		cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		cmdList->IASetVertexBuffers(0, 1, &m_Mesh.m_VertexBuffer->m_VertexBufferView);
		cmdList->IASetIndexBuffer(&m_Mesh.m_IndexBuffer->m_IndexBufferView);

		cmdList->DrawIndexedInstanced((UINT)m_Mesh.m_IndexBuffer->GetIndexCount(), 1, 0, 0, 0);
	}
}
