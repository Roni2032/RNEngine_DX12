#include "stdafx.h"
#include "project.h"

namespace RNEngine {
	Model::Model(bool isDebug, const string& name) :
		m_IsDebug(isDebug), m_DebugName(name),
		m_Filename(L""),
		m_DefaultScale(1.0f), m_DefaultRotation(Vector3::Zero){}

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
	void Model::SaveBinaryModel(const string& filename, vector<Mesh>& mesh, vector<shared_ptr<Material>>& materials) {
		Header header;
		header.m_MeshCount = (uint32_t)mesh.size();
		header.m_MaterialCount = (uint32_t)materials.size();
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
			DeleteDefaultFilePath(materials[i]->m_TextureName);
			materialNameLength[i] = (uint32_t)materials[i]->m_TextureName.size();
		}

		string fullPath = GetModelNameFromPath(filename);
		ofstream ofs(fullPath, ios_base::binary);

		if (ofs) {
			File::SaveBinary(ofs, &header);
			File::SaveBinary(ofs, &meshHeaders[0], header.m_MeshCount);
			for (uint32_t i = 0; i < header.m_MeshCount; i++) {
				File::SaveBinary(ofs, &mesh[i].m_Vertices[0], meshHeaders[i].m_VertexCount);
				File::SaveBinary(ofs, &mesh[i].m_Indices[0], meshHeaders[i].m_IndexCount);
				File::SaveBinary(ofs, &mesh[i].m_MaterialIndex);
			}

			for (uint32_t i = 0; i < header.m_MaterialCount; i++) {
				auto& material = materials[i];
				File::SaveBinary(ofs, &material->m_TextureType);
				File::SaveBinary(ofs, &materialNameLength[i]);
				File::SaveBinary(ofs, material->m_TextureName.data(), materialNameLength[i], false);

				if(material->m_TextureType == TextureDataType::Embedded){
					uint32_t formatLength = (uint32_t)material->m_EmbeddedTexture.m_Format.size();
					File::SaveBinary(ofs, &formatLength);
					File::SaveBinary(ofs, material->m_EmbeddedTexture.m_Format.data(), formatLength, false);
					File::SaveBinary(ofs, &material->m_EmbeddedTexture.m_Size);
					File::SaveBinary(ofs, &material->m_EmbeddedTexture.m_Data[0], material->m_EmbeddedTexture.m_Size);
				}
			}
		}
	}
	void Model::LoadBinaryModel(const string& filename, vector<Mesh>& mesh, vector<shared_ptr<Material>>& materials) {
		Header header;
		vector<MeshHeader> meshHeaders;
		vector<uint32_t> materialNameLength;

		string fullPath = GetModelNameFromPath(filename);

		ifstream ifs(fullPath, ios_base::binary);
		if (ifs) {
			File::ReadBinary(ifs, &header);
			meshHeaders.resize(header.m_MeshCount);
			mesh.resize(header.m_MeshCount);
			materials.resize(header.m_MaterialCount);

			File::ReadBinary(ifs, &meshHeaders[0], header.m_MeshCount);
			for (uint32_t i = 0; i < header.m_MeshCount; i++) {
				mesh[i].m_Vertices.resize(meshHeaders[i].m_VertexCount);
				mesh[i].m_Indices.resize(meshHeaders[i].m_IndexCount);
				File::ReadBinary(ifs, &mesh[i].m_Vertices[0], meshHeaders[i].m_VertexCount);
				File::ReadBinary(ifs, &mesh[i].m_Indices[0], meshHeaders[i].m_IndexCount);
				File::ReadBinary(ifs, &mesh[i].m_MaterialIndex);
			}

			materialNameLength.resize(header.m_MaterialCount);
			for (uint32_t i = 0; i < header.m_MaterialCount; i++) {
				materials[i] = make_shared<Material>();
				File::ReadBinary(ifs, &materials[i]->m_TextureType);
				File::ReadBinary(ifs, &materialNameLength[i]);
				 materials[i]->m_TextureName.resize(materialNameLength[i]);
				 File::ReadBinary(ifs, materials[i]->m_TextureName.data(), materialNameLength[i], false);

				if (materials[i]->m_TextureType == TextureDataType::Embedded) {
					materials[i]->m_EmbeddedTexture.m_Name = materials[i]->m_TextureName;
					uint32_t formatLength = 0;
					File::ReadBinary(ifs, &formatLength);
					File::ReadBinary(ifs, materials[i]->m_EmbeddedTexture.m_Format.data(), formatLength, false);
					File::ReadBinary(ifs, &materials[i]->m_EmbeddedTexture.m_Size);
					 materials[i]->m_EmbeddedTexture.m_Data.resize(materials[i]->m_EmbeddedTexture.m_Size);
					File::ReadBinary(ifs, &materials[i]->m_EmbeddedTexture.m_Data[0], materials[i]->m_EmbeddedTexture.m_Size);
				}
			}
		}
	}
	void Model::CreateBuffer(ID3D12Device* _dev,Mesh& mesh) {
		mesh.m_VertexBuffer = make_shared<VertexBuffer>();
		mesh.m_IndexBuffer = make_shared<IndexBuffer>();

		mesh.m_VertexBuffer->Create(_dev, mesh.m_Vertices);
		mesh.m_IndexBuffer->Create(_dev, mesh.m_Indices);
	}

	void Model::Load(ID3D12Device* _dev, const string& filename) {
		Timer modelTimer = Timer();
		modelTimer.Init();
		if (File::IsExistFile(Util::ConvertStrToWstr(GetModelNameFromPath(filename)))) {
			Timer timer = Timer();
			timer.Init();
			LoadBinaryModel(filename, m_Meshes, m_Materials);
			timer.Update();

			string meshFile = GetModelNameFromPath(filename);
			//DebugLog::Log(meshFile + u8"バイナリ読み込み: " + to_string(timer.GetDeltaTime()));

			for (UINT i = 0; i < m_Meshes.size(); i++) {
				CreateBuffer(_dev, m_Meshes[i]);
			}
			timer.Update();
			//DebugLog::Log(meshFile + u8"バッファ作成: " + to_string(timer.GetDeltaTime()));
			for (auto& material : m_Materials) {
				if (material->m_TextureType != TextureDataType::Embedded) {
					ResourceManager::RegisterTexture(material->m_TextureName);
				}
				else {
					EmbeddedTexture embeddedTexture = material->m_EmbeddedTexture;
					ResourceManager::RegisterTexture(
						embeddedTexture.m_Name, 
						reinterpret_cast<const uint8_t*>(embeddedTexture.m_Data.data()), embeddedTexture.m_Size);
				}
			}
			timer.Update();
			//DebugLog::Log(meshFile + u8"マテリアルテクスチャ作成: " + to_string(timer.GetDeltaTime()));
			m_Filename = Util::ConvertStrToWstr(filename);
			modelTimer.Update();
			//DebugLog::Log(meshFile + u8"モデル読み込み全体: " + to_string(modelTimer.GetDeltaTime()));
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

			auto& indices = newMesh.m_Indices;
			auto faceSize = mesh->mNumFaces;
			indices.reserve(faceSize * 3);
			for (UINT j = 0; j < faceSize; j++) {
				for (UINT k = 0; k < 3; k++) {
					indices.push_back(mesh->mFaces[j].mIndices[k]);
				}
			}
			CreateBuffer(_dev, newMesh);

			newMesh.m_MaterialIndex = mesh->mMaterialIndex;

			m_Meshes.push_back(newMesh);
		}

		auto numMaterials = scene->mNumMaterials;
		m_Materials.reserve(numMaterials);

		for (unsigned int i = 0; i < numMaterials; i++) {
			auto& material = scene->mMaterials[i];
			shared_ptr<Material> materialData;
			aiString path;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
			string imageName = path.C_Str();
			if (imageName.empty()) {
				m_Materials.push_back(make_shared<Material>());
				continue;
			}
			if (imageName[0] != '*') {
				string filePath = filename;
				size_t pos = filePath.find_last_of("/");
				if (pos != string::npos) {
					filePath = filePath.substr(0, pos + 1);
				}
				filePath += imageName;

				ResourceManager::RegisterTexture(filePath);
				materialData = make_shared<Material>(filePath);
			}
			else {
				int index = stoi(imageName.substr(1));
				auto embeddedTex = scene->mTextures[index];

				EmbeddedTexture embeddedTexture = EmbeddedTexture("embedded_" + to_string(i), embeddedTex);

				ResourceManager::RegisterTexture(
					embeddedTexture.m_Name,
					reinterpret_cast<const uint8_t*>(embeddedTexture.m_Data.data()), embeddedTexture.m_Size);

				materialData = make_shared<Material>(embeddedTexture);
			}
			m_Materials.push_back(materialData);
		}

		SaveBinaryModel(filename, m_Meshes, m_Materials);
		m_Filename = Util::ConvertStrToWstr(filename);

		if (m_IsDebug) OutputDebug(scene);
	}
	void Model::Load(const Mesh& mesh) {
		m_Meshes.resize(1);
		m_Materials.resize(1);
		m_Meshes[0] = mesh;
	}

	void Model::Draw(ComPtr<ID3D12GraphicsCommandList> cmdList, DescriptorHeap* heap, const ConstBuffer* constantBuffer) {

		auto pipelineState = PipelineStatePool::GetPipelineState(L"Sample1");
		cmdList->SetPipelineState(pipelineState->GetPtr());
		cmdList->SetGraphicsRootSignature(pipelineState->GetRootSignature()->GetPtr());
		auto renderer = Engine::GetRenderer();
		for (auto& mesh : m_Meshes) {
			cmdList->SetDescriptorHeaps(1, heap->GetHeapAddress());
			auto startHandle = heap->GetGPUHandle();
			auto handle = renderer->GetSRVDescriptorGPUHandle(constantBuffer->GetCBVHandle());
			cmdList->SetGraphicsRootDescriptorTable(0, handle);
			//テクスチャが読み込めなかったとき用のテクスチャ(エラーテクスチャ)
			string textureName = "Textures/ErrorTexture.png";
			if (m_Materials.size() > mesh.m_MaterialIndex && m_Materials[mesh.m_MaterialIndex]){
				if (m_Materials[mesh.m_MaterialIndex]->m_TextureType != TextureDataType::None) {
					textureName = m_Materials[mesh.m_MaterialIndex]->m_TextureName;
				}
			}

			auto texture = ResourceManager::GetTextureBuffer(textureName);
			if (texture) {
				handle = renderer->GetSRVDescriptorGPUHandle(texture->GetSRVHandle());
				cmdList->SetGraphicsRootDescriptorTable(1, handle);
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

	void Model::SetMaterial(const string& key, int index) {
		if (m_Materials.size() <= index) {
			return;
		}
		m_Materials[index] = make_shared<Material>(key);
	}
	shared_ptr<Material> Model::GetMaterial(int index) {
		return m_Materials[index];
	}
}