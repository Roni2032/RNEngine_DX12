#include "stdafx.h"
#include "DebugRenderer.h"
#include "project.h"
namespace RNEngine {
	vector<DebugCommand> DebugRenderer::g_Commands = {};
	unique_ptr<ConstBuffer> DebugRenderer::g_ConstantBuffer = make_unique<ConstBuffer>();
	Matrix DebugRenderer::g_Matrix = {};

	void DebugRenderer::Init() {
		auto dev = Engine::GetID3D12Device();
		g_ConstantBuffer->Create(dev, &g_Matrix);
	}
	void DebugRenderer::Flush() {
		auto scene = Engine::GetCurrentScene();
		auto renderer = Engine::GetRenderer();
		auto cmdList = renderer->GetCommandList();

		auto pipelineState = PipelineStatePool::GetPipelineState(L"WireFrame");
		cmdList->SetPipelineState(pipelineState->GetPtr());
		cmdList->SetGraphicsRootSignature(pipelineState->GetRootSignature()->GetPtr());

		for (auto& command : g_Commands) {
			auto camera = scene->GetCamera(command.camera);
			if (!camera)continue;

			g_Matrix.m_World = XMMatrixScaling(command.scale.x, command.scale.y, command.scale.z);
			g_Matrix.m_World *= XMMatrixRotationRollPitchYaw(command.rotation.x, command.rotation.y, command.rotation.z);
			g_Matrix.m_World *= XMMatrixTranslation(command.position.x, command.position.y, command.position.z);

			g_Matrix.m_ViewProjection = camera->GetViewProjectionMatrix();

			g_ConstantBuffer->Upadte(&g_Matrix, sizeof(g_Matrix));

			auto mesh = ResourceManager::GetMeshData(command.mesh);
			string textureName = "Textures/ErrorTexture.png";

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
		g_Commands.clear();
	}

	void DebugRenderer::DrawCubeWireFrame(Vector3 position, Vector3 size) {
		DebugCommand command;
		command.mesh = "DEFAULT_SQUARE_3D";
		command.camera = "Game";
		command.position = position;
		command.scale = size;
		command.rotation = Vector3();

		g_Commands.push_back(command);
	}
}