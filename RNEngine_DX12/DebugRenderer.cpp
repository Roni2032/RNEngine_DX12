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

	void DebugRenderer::DrawCubeWireFrame(const Vector3& position, const Vector3& size, const Color& color) {
		DebugCommand command;
		command.mesh = "DEFAULT_SQUARE_3D";
		command.camera = "Game";
		command.position = position;
		command.scale = size * 1.001f;
		command.rotation = Vector3();
		command.color = color;

		m_Commands.push_back(command);
	}
	void DebugRenderer::DrawSphereWireFrame(const Vector3& position, const Vector3& size, const Color& color) {
		DebugCommand command;
		command.mesh = "DEFAULT_SPHERE";
		command.camera = "Game";
		command.position = position;
		command.scale = size * 1.001f;
		command.rotation = Vector3();
		command.color = color;

		m_Commands.push_back(command);
	}
	void DebugRenderer::DrawLine(const Vector3& start, const Vector3& end, const float& scale, const Color& color) {
		DebugCommand command;
		command.mesh = "DEFAULT_LINE";
		command.camera = "Game";
		command.position = start;
		Vector3 dist = end - start;
		command.scale = Vector3(dist.Length(), scale, scale);

		auto look = XMMatrixLookAtLH(start, end, Vector3(0, 1, 0));
		auto quat = XMQuaternionRotationMatrix(look);

		command.rotation = Vector3();
		command.topology = D3D_PRIMITIVE_TOPOLOGY_LINELIST;
		command.color = color;

		m_Commands.push_back(command);
	}
	void DebugRenderer::DrawMeshWireFrame(const string& meshName, const Vector3& position, const Vector3& scale, const Vector3& rotation) {

	}

	void DebugRenderer::DrawWorldGridFrame(const Vector3& min, const Vector3& max, const float gridSize) {
		float width = max.x - min.x;
		float height = max.y - min.y;
		float depth = max.z - min.z;

		int wireCountX = (int)(width / gridSize);
		int wireCountY = (int)(height / gridSize);
		int wireCountZ = (int)(depth / gridSize);

		//Y軸
		for (int y = 0; y < wireCountY; ++y) {
			Vector3 start = min + Vector3(0.0f, gridSize * y, 0.0f);
			Vector3 end;
			//X軸
			for (int x = 0; x < wireCountX; ++x) {
				end = start + Vector3(0.0f, 0.0f, depth);
				DrawLine(start, end, 0.1f);
				start.x += gridSize;
			}
			start = min + Vector3(0.0f, gridSize * y, 0.0f);
			//Z軸
			for (int z = 0; z < wireCountZ; ++z) {
				end = start + Vector3(width, 0.0f, 0.0f);
				DrawLine(start, end, 0.1f);
				start.z += gridSize;
			}
		}


	}

	void DebugRenderer::Initialize() {
		ResourceManager::RegisterTexture("Textures/WireFrameTexture.png");

		PipelineStateSetup setup = {};
		setup.m_Vs = new Shader();
		setup.m_Vs->LoadVS(L"WireFrameVertexShader.hlsl", "VSMain");
		setup.m_Ps = new Shader();
		setup.m_Ps->LoadPS(L"WireFramePixelShader.hlsl", "PSMain");

		setup.m_RasterizerState = new RasterizerState();
		setup.m_RasterizerState->SetFillMode(FillMode::WIREFRAME);//ワイヤーフレーム描画
		setup.m_RasterizerState->SetCullMode(CullMode::NONE);//両面表示
		setup.m_RasterizerState->SetDepthBias(-1);//少し手前に表示
		setup.m_RasterizerState->SetSlopeScaledDepthBias(-1.0f);//少し手前に表示
		//setup.m_DepthEnable = false;
		setup.m_DepthMask = D3D12_DEPTH_WRITE_MASK_ZERO;//深度書き込みしない

		PipelineStatePool::RegisterPipelineState(L"WireFrame", InputLayout::PUV, setup);

		setup.m_RasterizerState->SetFillMode(FillMode::SOLID);
		setup.m_TopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
		PipelineStatePool::RegisterPipelineState(L"WireLine", InputLayout::PUV, setup);

	}
	void DebugRenderer::FlushWireFrames() {
		auto scene = Engine::GetCurrentScene();
		auto renderer = Engine::GetRenderer();
		auto cmdList = renderer->GetCommandList();
		auto heap = renderer->GetSrvDescriptorHeap();

		shared_ptr<PipelineState> pipelineState;

		for (int i = 0; i < m_Commands.size(); ++i) {
			auto& command = m_Commands[i];

			if (command.topology != D3D_PRIMITIVE_TOPOLOGY_LINELIST) {
				pipelineState = PipelineStatePool::GetPipelineState(L"WireFrame");
			}
			else {
				pipelineState = PipelineStatePool::GetPipelineState(L"WireLine");
			}
			cmdList->SetPipelineState(pipelineState->GetPtr());
			cmdList->SetGraphicsRootSignature(pipelineState->GetRootSignature()->GetPtr());

			if (m_Matrices.size() <= i) {
				m_Matrices.push_back(WireFrameCB());
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

			matrix.m_Matrix.m_World = XMMatrixScaling(command.scale.x, command.scale.y, command.scale.z);
			matrix.m_Matrix.m_World *= XMMatrixRotationRollPitchYaw(command.rotation.x, command.rotation.y, command.rotation.z);
			matrix.m_Matrix.m_World *= XMMatrixTranslation(command.position.x, command.position.y, command.position.z);

			matrix.m_Matrix.m_ViewProjection = camera->GetViewProjectionMatrix();
			matrix.m_Color = command.color;

			m_ConstantBuffers[i]->Update(&matrix, sizeof(matrix));

			cmdList->SetDescriptorHeaps(1, heap->GetHeapAddress());
			auto startHandle = heap->GetGPUHandle();
			auto constHandle = renderer->GetSRVDescriptorGPUHandle(m_ConstantBuffers[i]->GetHandle());
			cmdList->SetGraphicsRootDescriptorTable(HeapType::CBV, constHandle);

			auto mesh = ResourceManager::GetMeshData(command.mesh);

			cmdList->IASetPrimitiveTopology(command.topology);

			cmdList->IASetVertexBuffers(0, 1, &mesh.m_VertexBuffer->GetBufferView());
			if (command.topology != D3D_PRIMITIVE_TOPOLOGY_LINELIST) {
				cmdList->IASetIndexBuffer(&mesh.m_IndexBuffer->GetBufferView());

				cmdList->DrawIndexedInstanced((UINT)mesh.m_IndexBuffer->GetIndexCount(), 1, 0, 0, 0);
			}
			else {
				D3D12_INDEX_BUFFER_VIEW nullBuffer = {};
				cmdList->IASetIndexBuffer(&nullBuffer);
				cmdList->DrawInstanced((UINT)mesh.m_VertexBuffer->GetVertexCount(), 1, 0, 0);
			}
		}
		m_Commands.clear();
	}
}