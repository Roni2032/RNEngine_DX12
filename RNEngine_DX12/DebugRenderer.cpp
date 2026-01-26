#include "stdafx.h"
#include "DebugRenderer.h"
#include "project.h"
namespace RNEngine {
	vector<DebugCommand> DebugRenderer::g_Commands = {};
	unique_ptr<ConstantBuffer> DebugRenderer::g_ConstantBuffer = make_unique<ConstantBuffer>();
	WireFrameCB DebugRenderer::g_FrameCB = {};
	Matrix DebugRenderer::g_Matrix = {};

	void DebugRenderer::Init() {
		ResourceManager::RegisterTexture("Textures/WireFrameTexture.png");

		Shader vs, ps;
		vs.LoadVS(L"SampleVertexShader.hlsl", "VSMain");
		ps.LoadPS(L"SamplePixelShader.hlsl", "PSMain");

		RasterizerState wireRasterizerState = RasterizerState();
		wireRasterizerState.SetFillMode(FillMode::WIREFRAME);

		PipelineStatePool::RegisterPipelineState(L"WireFrame", InputLayout::PUV, &vs, &ps, &wireRasterizerState);

		auto dev = Engine::GetID3D12Device();
		g_ConstantBuffer->Create(dev, &g_Matrix);
	}
	void DebugRenderer::Flush() {
		auto scene = Engine::GetCurrentScene();
		auto renderer = Engine::GetRenderer();
		auto cmdList = renderer->GetCommandList();
		auto heap = renderer->GetSrvDescriptorHeap();

		auto pipelineState = PipelineStatePool::GetPipelineState(L"WireFrame");
		cmdList->SetPipelineState(pipelineState->GetPtr());
		cmdList->SetGraphicsRootSignature(pipelineState->GetRootSignature()->GetPtr());


		string textureName = "Textures/WireFrameTexture.png";
		for (auto& command : g_Commands){

			auto camera = scene->GetCamera(command.camera);
			if (!camera)continue;

			g_Matrix.m_World = XMMatrixScaling(command.scale.x, command.scale.y, command.scale.z);
			g_Matrix.m_World *= XMMatrixRotationRollPitchYaw(command.rotation.x, command.rotation.y, command.rotation.z);
			g_Matrix.m_World *= XMMatrixTranslation(command.position.x, command.position.y, command.position.z);

			g_Matrix.m_ViewProjection = camera->GetViewProjectionMatrix();

			g_FrameCB.m_Matrix = g_Matrix;
			g_FrameCB.m_Color = Vector4(1, 1, 1, 1);
			g_ConstantBuffer->Update(&g_Matrix, sizeof(g_Matrix));


			cmdList->SetDescriptorHeaps(1, heap->GetHeapAddress());
			auto startHandle = heap->GetGPUHandle();
			auto constHandle = renderer->GetSRVDescriptorGPUHandle(g_ConstantBuffer->GetHandle());
			cmdList->SetGraphicsRootDescriptorTable(HeapType::CBV, constHandle);

			auto mesh = ResourceManager::GetMeshData(command.mesh);

			auto texture = ResourceManager::GetTextureBuffer(textureName);
			if (texture) {
				auto handle = renderer->GetSRVDescriptorGPUHandle(texture->GetSRVHandle());
				cmdList->SetGraphicsRootDescriptorTable(HeapType::SRV, handle);
			}

			cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			cmdList->IASetVertexBuffers(0, 1, &mesh.m_VertexBuffer->GetBufferView());
			cmdList->IASetIndexBuffer(&mesh.m_IndexBuffer->GetBufferView());

			cmdList->DrawIndexedInstanced((UINT)mesh.m_IndexBuffer->GetIndexCount(), 1, 0, 0, 0);

		}
		g_Commands.clear();
	}

	void DebugRenderer::DrawCubeWireFrame(Vector3 position, Vector3 size) {
		DebugCommand command;
		command.mesh = "DEFAULT_SQUARE_3D";
		command.camera = "Game";
		command.position = position;
		command.scale = size * 1.001f;
		command.rotation = Vector3();

		g_Commands.push_back(command);
	}
}