#include "stdafx.h"
#include "project.h"

namespace RNEngine {
	string Model::GetModelNameFromPath(const string& filepath) {
		string name = filepath;
		size_t dotPos = name.find_last_of(".");
		size_t slashPos = name.find_last_of("/\\");
		if (dotPos != string::npos && slashPos != string::npos && dotPos > slashPos) {
			name = name.substr(slashPos + 1, dotPos - slashPos - 1);
		}
		name += ".mesh";

		string fullPath = "../RNEngine_DX12/mesh/" + name;

		return fullPath;
	}
	void Model::DeleteDefaultFilePath(string& filePath) {
		string defaultPath = ResourceManager::GetDefaultFilePath();
		size_t pos = filePath.find(defaultPath);
		if (pos != string::npos && pos == 0) {
			filePath.erase(0, defaultPath.length());
		}
	}
	void Model::SaveBinaryModel(const string& filename, vector<Mesh>& mesh, vector<string>& materialTextures) {
		Header header;
		header.m_MeshCount = (uint32_t)mesh.size();
		header.m_MaterialCount = (uint32_t)materialTextures.size();
		vector<MeshHeader> meshHeaders(header.m_MeshCount);
		for (uint32_t i = 0; i < header.m_MeshCount; i++) {
			MeshHeader meshHeader;
			meshHeader.m_VertexCount = (uint32_t)mesh[i].m_Vertices.size();
			meshHeader.m_IndexCount = (uint32_t)mesh[i].m_Indices.size();
			meshHeader.m_materialIndex = mesh[i].m_MaterialIndex;
			meshHeaders[i] = meshHeader;
		}
		vector<uint32_t> materialNameLength(header.m_MaterialCount);
		for (uint32_t i = 0; i < header.m_MaterialCount; i++) {
			DeleteDefaultFilePath(materialTextures[i]);
			materialNameLength[i] = (uint32_t)materialTextures[i].size();
		}

		string fullPath = GetModelNameFromPath(filename);
		ofstream ofs(fullPath, ios_base::binary);

		if (ofs) {
			ofs.write(reinterpret_cast<const char*>(&header), sizeof(Header));
			for (uint32_t i = 0; i < header.m_MeshCount; i++) {
				ofs.write(reinterpret_cast<const char*>(&meshHeaders[i]), sizeof(meshHeaders[0]));
			}
			for (uint32_t i = 0; i < header.m_MeshCount; i++) {
				for (uint32_t j = 0; j < meshHeaders[i].m_VertexCount; j++) {
					ofs.write(reinterpret_cast<const char*>(&mesh[i].m_Vertices[j]), sizeof(mesh[i].m_Vertices[0]));
				}
				for (uint32_t j = 0; j < meshHeaders[i].m_IndexCount; j++) {
					ofs.write(reinterpret_cast<const char*>(&mesh[i].m_Indices[j]), sizeof(mesh[i].m_Indices[0]));
				}
				ofs.write(reinterpret_cast<const char*>(&mesh[i].m_MaterialIndex), sizeof(mesh[i].m_MaterialIndex));
			}
			for (uint32_t i = 0; i < header.m_MaterialCount; i++) {
				ofs.write(reinterpret_cast<const char*>(&materialNameLength[i]), sizeof(materialNameLength[0]));
				ofs.write(materialTextures[i].data(), materialNameLength[i]);
			}
		}
	}
	void Model::LoadBinaryModel(const string& filename,vector<Mesh>& mesh, vector<string>& materialTextures) {
		Header header;
		vector<MeshHeader> meshHeaders;
		vector<uint32_t> materialNameLength;

		string fullPath = GetModelNameFromPath(filename);

		ifstream ifs(fullPath, ios_base::binary);
		if (ifs) {
			ifs.read(reinterpret_cast<char*>(&header), sizeof(Header));
			meshHeaders.resize(header.m_MeshCount);
			mesh.resize(header.m_MeshCount);
			materialTextures.resize(header.m_MaterialCount);

			for (uint32_t i = 0; i < header.m_MeshCount; i++) {
				ifs.read(reinterpret_cast<char*>(&meshHeaders[i]), sizeof(meshHeaders[0]));
			}
			for (uint32_t i = 0; i < header.m_MeshCount; i++) {
				mesh[i].m_Vertices.resize(meshHeaders[i].m_VertexCount);
				mesh[i].m_Indices.resize(meshHeaders[i].m_IndexCount);

				for (uint32_t j = 0; j < meshHeaders[i].m_VertexCount; j++) {
					ifs.read(reinterpret_cast< char*>(&mesh[i].m_Vertices[j]), sizeof(mesh[i].m_Vertices[0]));
				}
				for (uint32_t j = 0; j < meshHeaders[i].m_IndexCount; j++) {
					ifs.read(reinterpret_cast< char*>(&mesh[i].m_Indices[j]), sizeof(mesh[i].m_Indices[0]));
				}
				ifs.read(reinterpret_cast< char*>(&mesh[i].m_MaterialIndex), sizeof(mesh[i].m_MaterialIndex));
			}
			materialNameLength.resize(header.m_MaterialCount);
			for (uint32_t i = 0; i < header.m_MaterialCount; i++) {
				ifs.read(reinterpret_cast< char*>(&materialNameLength[i]), sizeof(materialNameLength[0]));
				materialTextures[i].resize(materialNameLength[i]);
				ifs.read(materialTextures[i].data(), materialNameLength[i]);
			}
		}
	}


	void Model::Load(ID3D12Device* _dev, const string& filename) {

		if (File::IsExistFile(Util::ConvertStrToWstr(GetModelNameFromPath(filename)))) {
			LoadBinaryModel(filename, m_Meshes, m_MaterialTextureName);

			for (UINT i = 0; i < m_Meshes.size(); i++) {
				m_Meshes[i].m_VertexBuffer = make_shared<VertexBuffer>();
				m_Meshes[i].m_VertexBuffer->Create(_dev, m_Meshes[i].m_Vertices);

				m_Meshes[i].m_IndexBuffer = make_shared<IndexBuffer>();
				m_Meshes[i].m_IndexBuffer->Create(_dev, m_Meshes[i].m_Indices);
			}
			for (auto& materialTexture : m_MaterialTextureName) {
				ResourceManager::RegisterTexture(materialTexture);
			}
			return;
		}
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
			auto& vertices = newMesh.m_Vertices;
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

			auto& indices = newMesh.m_Indices;
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

		SaveBinaryModel(filename, m_Meshes, m_MaterialTextureName);
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
			auto handle = renderer->GetSRVDescriptorGPUHandle(constantBuffer->GetCBVHandle());
			cmdList->SetGraphicsRootDescriptorTable(0, handle);

			if (m_MaterialTextureName.size() > mesh.m_MaterialIndex) {
				auto& textureName = m_MaterialTextureName[mesh.m_MaterialIndex];

				auto texture = ResourceManager::GetTextureBuffer(textureName);
				if (texture) {
					handle = renderer->GetSRVDescriptorGPUHandle(texture->GetSRVHandle());
					cmdList->SetGraphicsRootDescriptorTable(1, handle);
				}
			}

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