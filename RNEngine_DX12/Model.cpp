#include "stdafx.h"
#include "project.h"

namespace RNEngine {

	void Model::Load(ID3D12Device* _dev, const string& filename) {
		Assimp::Importer importer;
		unsigned int readFlags = 0;
		readFlags |= aiProcess_FlipUVs; // UVを反転させる
		readFlags |= aiProcess_MakeLeftHanded; // 左手座標系に変換する
		readFlags |= aiProcess_Triangulate; // 三角化（読込が遅くなる）
		auto scene = importer.ReadFile(filename, readFlags);
		assert(scene);

		const UINT meshSize = scene->mNumMeshes;
		m_Meshes.reserve(meshSize);
		for (UINT i = 0; i < meshSize; i++) {
			auto& mesh = scene->mMeshes[i];
			UINT vertexSize = mesh->mNumVertices;

			Mesh newMesh;
			auto& vertices = newMesh.m_Verteces;
			vertices.reserve(vertexSize);

			for (UINT j = 0; j < vertexSize; j++) {
				// 頂点座標
				XMFLOAT3 position(0, 0, 0);
				if (mesh->HasPositions())
					::CopyMemory(&position, &mesh->mVertices[j], sizeof(position));
				//// 法線ベクトル
				//XMFLOAT3 normal(0, 0, 0);
				//if (mesh->HasNormals())
				//	::CopyMemory(&normal, &mesh->mNormals[j], sizeof(normal));
				// UV座標
				XMFLOAT2 uv(0, 0);
				if (mesh->HasTextureCoords(0))
					::CopyMemory(&uv, &mesh->mTextureCoords[0][j], sizeof(uv));
				vertices.push_back({ position, uv });
			}

			newMesh.m_VertexBuffer = make_shared<VertexBuffer>();
			newMesh.m_VertexBuffer->Create(_dev, vertices);

			auto& indices = newMesh.m_Indeces;
			auto faceSize = mesh->mNumFaces;
			indices.reserve(faceSize * 3);
			for (UINT j = 0; j < faceSize; j++) {
				for (UINT k = 0; k < 3; k++) {
					indices.push_back(mesh->mFaces[j].mIndices[k]);
				}
			}

			newMesh.m_IndexBuffer = make_shared<IndexBuffer>();
			newMesh.m_IndexBuffer->Create(_dev, indices);

			newMesh.m_MaterialIndex = mesh->mMaterialIndex;

			m_Meshes.push_back(newMesh);
		}
		
		auto numMaterials = scene->mNumMaterials;
		m_MaterialTextureName.reserve(numMaterials);
		for (unsigned int i = 0; i < numMaterials; i++) {
			auto& material = scene->mMaterials[i];
			
			aiString path;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
			string imageName = path.C_Str();

			string filePath = filename;
			size_t pos = filePath.find_last_of("/");
			if (pos != string::npos) {
				filePath = filePath.substr(0, pos + 1);
			}
			filePath += imageName;

			ResourceManager::RegisterTexture(filePath);

			m_MaterialTextureName.push_back(filePath);
		}

		if (m_IsDebug) OutputDebug(scene);
	}

	void Model::Draw(ComPtr<ID3D12GraphicsCommandList> cmdList, DescriptorHeap* heap,const ConstBuffer* constantBuffer) {
		
		auto pipelineState = PipelineStatePool::GetPipelineState(L"Sample1");
		cmdList->SetPipelineState(pipelineState->GetPtr());
		cmdList->SetGraphicsRootSignature(pipelineState->GetRootSignature()->GetPtr());
		auto renderer = Engine::GetRenderer();
		for (auto& mesh : m_Meshes) {
			cmdList->SetDescriptorHeaps(1, heap->GetHeapAddress());
			auto startHandle = heap->GetGPUHandle();
			auto handle = renderer->GetSRVDescriptorHandle(constantBuffer->GetCBVHandle());
			cmdList->SetGraphicsRootDescriptorTable(0, handle);

			auto& textureName = m_MaterialTextureName[mesh.m_MaterialIndex];

			auto texture = ResourceManager::GetTextureBuffer(textureName);

			handle = renderer->GetSRVDescriptorHandle(texture->GetSRVHandle());
			cmdList->SetGraphicsRootDescriptorTable(1, handle);

			cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			cmdList->IASetVertexBuffers(0, 1, &mesh.m_VertexBuffer->m_VertexBufferView);
			cmdList->IASetIndexBuffer(&mesh.m_IndexBuffer->m_IndexBufferView);

			cmdList->DrawIndexedInstanced((UINT)mesh.m_IndexBuffer->GetIndexCount(), 1, 0, 0, 0);
		}
	}

	void Model::OutputDebug(const aiScene* scene) {
		ofstream ofs("../Assets/" + m_DebugName + ".txt");
		if (ofs)
		{
			ofs << "----- Mesh Data -----" << std::endl;
			auto numMeshes = scene->mNumMeshes;
			ofs << "Num Meshes : " << numMeshes << std::endl;

			// 各メッシュの頂点数を出力する
			for (unsigned int i = 0; i < numMeshes; i++)
			{
				auto& mesh = scene->mMeshes[i];

				unsigned int numVertices = mesh->mNumVertices;

				// 頂点数を出力する
				ofs << "mesh " << i << " : " << numVertices << std::endl;

			}
			ofs << std::endl << "----- Material Data -----" << std::endl;

			auto numMaterials = scene->mNumMaterials;
			ofs << "Num Materials : " << numMaterials << std::endl;

			for (unsigned int i = 0; i < numMaterials; i++) {
				auto& material = scene->mMaterials[i];
				ofs << "Material " << i << " : " << material->GetName().C_Str() << "  ";

				aiString path;
				material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
				ofs << "=> " << path.C_Str() << std::endl;
			}
		}
	}
}