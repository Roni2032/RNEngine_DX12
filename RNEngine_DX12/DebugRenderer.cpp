#include "stdafx.h"
#include "DebugRenderer.h"

#include "RNEngine.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "RendererHeader.h"

#include "Descriptor.h"
#include "ConstantBuffer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "TextureBuffer.h"

#include "PipelineState.h"
#include "Shader.h"

#include "Scene.h"
#include "Camera.h"

namespace RNEngine {
	DebugRenderer::DebugRenderer() {}
	DebugRenderer::~DebugRenderer() {}

	void DebugRenderer::DrawCubeWireFrame(const Vector3& position, const Vector3& size) {
		DebugCommand command;
		command.mesh = "DEFAULT_SQUARE_3D";
		command.camera = "Game";
		command.position = position;
		command.scale = size * 1.001f;
		command.rotation = Vector3();

		m_Commands.push_back(command);
	}
	void DebugRenderer::Initialize() {
		ResourceManager::RegisterTexture("Textures/WireFrameTexture.png");

		PipelineStateSetup setup = {};
		setup.m_Vs = new Shader();
		setup.m_Vs->LoadVS(L"SampleVertexShader.hlsl", "VSMain");
		setup.m_Ps = new Shader();
		setup.m_Ps->LoadPS(L"SamplePixelShader.hlsl", "PSMain");

		setup.m_RasterizerState = new RasterizerState();
		setup.m_RasterizerState->SetFillMode(FillMode::WIREFRAME);
		setup.m_RasterizerState->SetCullMode(CullMode::NONE);

		setup.m_DepthEnable = false;
		//setup.m_DepthMask = D3D12_DEPTH_WRITE_MASK_ZERO;

		PipelineStatePool::RegisterPipelineState(L"WireFrame", InputLayout::PUV, setup);
	}
	void DebugRenderer::Flush() {
		auto scene = Engine::GetCurrentScene();
		auto renderer = Engine::GetRenderer();
		auto cmdList = renderer->GetCommandList();
		auto heap = renderer->GetSrvDescriptorHeap();

		auto pipelineState = PipelineStatePool::GetPipelineState(L"WireFrame");
		cmdList->SetPipelineState(pipelineState->GetPtr());
		cmdList->SetGraphicsRootSignature(pipelineState->GetRootSignature()->GetPtr());


		string textureName = "Textures/ErrorTexture.png";
		for (int i = 0; i < m_Commands.size(); ++i) {
			auto& command = m_Commands[i];
			if (m_Matrices.size() <= i) {
				m_Matrices.push_back(Matrix());
			}
			if (m_ConstantBuffers.size() <= i) {
				auto dev = Engine::GetID3D12Device();
				auto constantBuffer = make_unique<ConstantBuffer>();
				constantBuffer->Create(dev, &m_Matrices[i]);
				m_ConstantBuffers.push_back(move(constantBuffer));
			}

			auto camera = scene->GetCamera(command.camera);
			if (!camera)continue;

			auto& matrix = m_Matrices[i];

			matrix.m_World = XMMatrixScaling(command.scale.x, command.scale.y, command.scale.z);
			matrix.m_World *= XMMatrixRotationRollPitchYaw(command.rotation.x, command.rotation.y, command.rotation.z);
			matrix.m_World *= XMMatrixTranslation(command.position.x, command.position.y, command.position.z);

			matrix.m_ViewProjection = camera->GetViewProjectionMatrix();
			m_ConstantBuffers[i]->Update(&matrix, sizeof(matrix));

			cmdList->SetDescriptorHeaps(1, heap->GetHeapAddress());
			auto startHandle = heap->GetGPUHandle();
			auto constHandle = renderer->GetSRVDescriptorGPUHandle(m_ConstantBuffers[i]->GetHandle());
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
		m_Commands.clear();
	}
}