#include "Loders.h"
#include "EditorGUI.h"
namespace RNEngine {
	Assimp::Importer AssimpLoader::g_Importer = {};

	string AssimpLoader::GetTextureName(aiMaterial* material) {
		aiString path;
		material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
		string textureName = path.C_Str();

		return textureName;
	}
	const aiScene* AssimpLoader::InitializeScene(const string& filePath) {
		unsigned int readFlags = 0;
		readFlags |= aiProcess_FlipUVs; // UVを反転させる
		readFlags |= aiProcess_MakeLeftHanded; // 左手座標系に変換する
		readFlags |= aiProcess_Triangulate; // 三角化（読込が遅くなる）
		auto scene = g_Importer.ReadFile(filePath, readFlags);
		assert(scene);

		return scene;
	}

	void AssimpLoader::ImportMesh(ID3D12Device* _dev, Model& output,const aiScene* scene) {
		const UINT meshSize = scene->mNumMeshes;
		
		vector<Mesh> meshes;
		meshes.reserve(meshSize);
		for (UINT i = 0; i < meshSize; i++) {
			Mesh newMesh;

			auto& sceneMesh = scene->mMeshes[i];

			auto& vertices = newMesh.m_Vertices;
			auto& indices = newMesh.m_Indices;

			UINT vertexSize = sceneMesh->mNumVertices;
			auto faceSize = sceneMesh->mNumFaces;

			vertices.reserve(vertexSize);
			indices.reserve(faceSize * 3);

			for (UINT v = 0; v < vertexSize; ++v) {
				//使用する頂点データによってここを増やしていく
				Vertex vertex = {};
				if(sceneMesh->HasPositions())
					::CopyMemory(&vertex.m_Position, &sceneMesh->mVertices[v], sizeof(vertex.m_Position));
				if (sceneMesh->HasNormals()){}
					//::CopyMemory(&vertex.m_Position, &sceneMesh->mVertices[v], sizeof(vertex.m_Position));
				if (sceneMesh->HasTextureCoords(0))
					::CopyMemory(&vertex.m_Uv, &sceneMesh->mTextureCoords[0][v], sizeof(vertex.m_Uv));

				vertices.push_back(vertex);
			}

			for (UINT j = 0; j < faceSize; j++) {
				auto& face = sceneMesh->mFaces[j];
				for (UINT k = 0; k < 3; k++) {
					indices.push_back(face.mIndices[k]);
				}
			}
			DxUtil::CreateMeshBuffer(_dev, newMesh);

			newMesh.m_MaterialIndex = sceneMesh->mMaterialIndex;

			meshes.push_back(newMesh);
		}
		output.m_Meshes = meshes;

	}
	void AssimpLoader::ImportMaterial(Model& output, const aiScene* scene, const string& filePath) {
		auto numMaterials = scene->mNumMaterials;

		vector<Material> materials;
		materials.reserve(numMaterials);

		for (UINT i = 0; i < numMaterials; i++) {
			auto& sceneMaterial = scene->mMaterials[i];
			Material newMaterial = {};
			string textureName = GetTextureName(sceneMaterial);
			
			if (textureName.empty()) {
				materials.push_back(Material());
				return;
			}

			if (textureName[0] != '*') {
				string registryKey = "";
				registryKey = File::SplitFilePath(filePath);
				registryKey += textureName;

				ResourceManager::RegisterTexture(registryKey);
				newMaterial = Material(registryKey);
			}
			else {
				int textureIndex = stoi(textureName.substr(1));
				aiTexture* textureData = scene->mTextures[textureIndex];

				EmbeddedTexture texture = EmbeddedTexture("texture_" + to_string(textureIndex), textureData);

				ResourceManager::RegisterTexture(texture.m_Name, reinterpret_cast<const uint8_t*>(texture.m_Data.data()), texture.m_Size);
				newMaterial = Material(texture);
			}
			materials.push_back(newMaterial);
		}
		output.m_Materials = materials;
	}
	void AssimpLoader::Import(ID3D12Device* _dev, Model& output, const wstring& filePath) {
		if (MeshIO::IsExistMeshFile(filePath)) {
			//meshファイルが存在するときはmeshファイルから抽出
			MeshLoader::LoadMeshFile(_dev, output, MeshIO::GetMeshFileName(filePath));
		}
		else {
			//meshファイルが存在しない時はAssimpからデータ抽出
			auto scene = InitializeScene(Util::ConvertWstrToStr(filePath));


			ImportMesh(_dev, output, scene);
			ImportMaterial(output, scene, Util::ConvertWstrToStr(filePath));
			MeshLoader::AdjustModelSizeMatrix(output, Vector3(1.0f, 1.0f, 1.0f));

			MeshWriter::SaveMeshFile(output, MeshIO::GetMeshFileName(filePath));
		}
	}

	bool MeshIO::IsExistMeshFile(const wstring& filePath) {
		return File::IsExistFile(GetMeshFileName(filePath));
	}
	wstring MeshIO::GetMeshFileName(const wstring& filePath) {
		string name = Util::ConvertWstrToStr(filePath);
		size_t dotPos = name.find_last_of(".");
		size_t slashPos = name.find_last_of("/\\");
		if (dotPos != string::npos && slashPos != string::npos && dotPos > slashPos) {
			name = name.substr(slashPos + 1, dotPos - slashPos - 1);
		}
		name += ".mesh";

		string fullPath = "../RNEngine_DX12/mesh/" + name;

		return Util::ConvertStrToWstr(fullPath);
	}

	void MeshWriter::DeleteDefaultFilePath(string& filePath) {
		string defaultPath = ResourceManager::GetDefaultFilePath();
		size_t pos = filePath.find(defaultPath);
		if (pos != string::npos && pos == 0) {
			filePath.erase(0, defaultPath.length());
		}
	}

	void MeshWriter::SaveMeshFile(const Model& model, const wstring& meshPath) {
		auto& meshes = model.m_Meshes;
		auto& materials = model.m_Materials;

		MeshFile::ModelHeader header;
		header.m_MeshCount = (uint32_t)meshes.size();
		header.m_MaterialCount = (uint32_t)materials.size();
		vector<MeshFile::MeshHeader> meshHeaders(header.m_MeshCount);
		for (uint32_t i = 0; i < header.m_MeshCount; i++) {
			MeshFile::MeshHeader meshHeader;
			meshHeader.m_VertexCount = (uint32_t)meshes[i].m_Vertices.size();
			meshHeader.m_IndexCount = (uint32_t)meshes[i].m_Indices.size();
			meshHeader.m_materialIndex = meshes[i].m_MaterialIndex;
			meshHeaders[i] = meshHeader;
		}
		vector<uint32_t> materialTextureNameLength(header.m_MaterialCount);
		for (uint32_t i = 0; i < header.m_MaterialCount; i++) {
			string textureName = materials[i].m_TextureName;
			DeleteDefaultFilePath(textureName);
			materialTextureNameLength[i] = (uint32_t)textureName.size();
		}

		string fullPath = Util::ConvertWstrToStr(MeshIO::GetMeshFileName(meshPath));
		ofstream ofs(fullPath, ios_base::binary);

		if (ofs) {
			File::SaveBinary(ofs, &header);
			File::SaveBinary(ofs, &meshHeaders[0], header.m_MeshCount);
			for (uint32_t i = 0; i < header.m_MeshCount; i++) {
				File::SaveBinary(ofs, &meshes[i].m_Vertices[0], meshHeaders[i].m_VertexCount);
				File::SaveBinary(ofs, &meshes[i].m_Indices[0], meshHeaders[i].m_IndexCount);
				File::SaveBinary(ofs, &meshes[i].m_MaterialIndex);
			}

			for (uint32_t i = 0; i < header.m_MaterialCount; i++) {
				auto& material = materials[i];
				string textureName = materials[i].m_TextureName;
				DeleteDefaultFilePath(textureName);
				File::SaveBinary(ofs, &material.m_TextureType);
				File::SaveBinary(ofs, &materialTextureNameLength[i]);
				File::SaveBinary(ofs, textureName.data(), materialTextureNameLength[i], false);

				if (material.m_TextureType == TextureDataType::Embedded) {
					uint32_t formatLength = (uint32_t)material.m_EmbeddedTexture.m_Format.size();
					File::SaveBinary(ofs, &formatLength);
					File::SaveBinary(ofs, material.m_EmbeddedTexture.m_Format.data(), formatLength, false);
					File::SaveBinary(ofs, &material.m_EmbeddedTexture.m_Size);
					File::SaveBinary(ofs, material.m_EmbeddedTexture.m_Data.data(), material.m_EmbeddedTexture.m_Size);
				}
			}
		}
	}

	void MeshLoader::AdjustModelSizeMatrix(Model& model, const Vector3& size) {
		AABB aabb = AABB(Vector3(-FLT_MAX), Vector3(FLT_MAX));
		for (auto& mesh : model.m_Meshes) {
			for (auto& vertex : mesh.m_Vertices) {
				Vector3 position = vertex.m_Position;

				aabb.m_Max = aabb.m_Max.Max(position);
				aabb.m_Min = aabb.m_Min.Min(position);
			}
		}

		Vector3 aabbSize = aabb.GetSize();
		Vector3 adjustSize = size / aabbSize;

		Vector3 aabbCenter = aabb.GetCenter();

		Vector3 bottom = Vector3(aabbCenter.x, aabb.m_Min.y, aabbCenter.z);
		bottom = Vector3();
		model.m_AdjustMatrix = XMMatrixScaling(adjustSize.x, adjustSize.y, adjustSize.z);
		model.m_AdjustMatrix *= XMMatrixRotationQuaternion(Quaternion::Identity);
		model.m_AdjustMatrix *= XMMatrixTranslation(-bottom.x, -bottom.y, -bottom.z);
	}

	void MeshLoader::InitModel(ID3D12Device* _dev,Model& model) {
		//モデルの情報を基にバッファ作成
		for (UINT i = 0; i < model.m_Meshes.size(); i++) {
			DxUtil::CreateMeshBuffer(_dev, model.m_Meshes[i]);
		}
		//マテリアルからSRVを作成
		for (auto& material : model.m_Materials) {
			if (material.m_TextureType != TextureDataType::Embedded) {
				ResourceManager::RegisterTexture(material.m_TextureName);
			}
			else {
				EmbeddedTexture embeddedTexture = material.m_EmbeddedTexture;
				ResourceManager::RegisterTexture(
					embeddedTexture.m_Name,
					reinterpret_cast<const uint8_t*>(embeddedTexture.m_Data.data()), embeddedTexture.m_Size);
			}
		}
		AdjustModelSizeMatrix(model, Vector3(1.0f, 1.0f, 1.0f));
	}
	void MeshLoader::LoadMeshFile(ID3D12Device* _dev,Model& model, const wstring& meshPath) {
		//モデル全体の情報ヘッダー
		MeshFile::ModelHeader modelHeader;
		//メッシュごとの情報ヘッダー
		vector<MeshFile::MeshHeader> meshHeaders;
		//マテリアルに設定されているテクスチャの名前の長さ
		vector<uint32_t> materialTextureNameLength;

		//meshファイルパスを取得
		string fullPath = Util::ConvertWstrToStr(MeshIO::GetMeshFileName(meshPath));

		ifstream ifs(fullPath, ios_base::binary);

		auto& meshes = model.m_Meshes;
		auto& materials = model.m_Materials;
		if (ifs) {
			//モデル全体のデータを読み込む
			File::ReadBinary(ifs, &modelHeader);
			//読み込んだ情報から配列サイズを設定
			meshHeaders.resize(modelHeader.m_MeshCount);
			model.m_Meshes.resize(modelHeader.m_MeshCount);
			model.m_Materials.resize(modelHeader.m_MaterialCount);

			//メッシュのデータを読み込む
			File::ReadBinary(ifs, &meshHeaders[0], modelHeader.m_MeshCount);
			//読み込んだ情報からメッシュデータを読み込む
			for (uint32_t i = 0; i < modelHeader.m_MeshCount; i++) {
				meshes[i].m_Vertices.resize(meshHeaders[i].m_VertexCount);
				meshes[i].m_Indices.resize(meshHeaders[i].m_IndexCount);
				File::ReadBinary(ifs, &meshes[i].m_Vertices[0], meshHeaders[i].m_VertexCount);
				File::ReadBinary(ifs, &meshes[i].m_Indices[0], meshHeaders[i].m_IndexCount);
				File::ReadBinary(ifs, &meshes[i].m_MaterialIndex);
			}

			//マテリアルデータの読み込み
			materialTextureNameLength.resize(modelHeader.m_MaterialCount);
			for (uint32_t i = 0; i < modelHeader.m_MaterialCount; i++) {
				materials[i] = Material();
				File::ReadBinary(ifs, &materials[i].m_TextureType);
				File::ReadBinary(ifs, &materialTextureNameLength[i]);
				materials[i].m_TextureName.resize(materialTextureNameLength[i]);
				File::ReadBinary(ifs, materials[i].m_TextureName.data(), materialTextureNameLength[i], false);

				if (materials[i].m_TextureType == TextureDataType::Embedded) {
					materials[i].m_EmbeddedTexture.m_Name = materials[i].m_TextureName;
					uint32_t formatLength = 0;
					File::ReadBinary(ifs, &formatLength);
					File::ReadBinary(ifs, materials[i].m_EmbeddedTexture.m_Format.data(), formatLength, false);
					File::ReadBinary(ifs, &materials[i].m_EmbeddedTexture.m_Size);
					materials[i].m_EmbeddedTexture.m_Data.resize(materials[i].m_EmbeddedTexture.m_Size);
					File::ReadBinary(ifs, &materials[i].m_EmbeddedTexture.m_Data[0], materials[i].m_EmbeddedTexture.m_Size);
				}
			}
		}

		//読み込んだデータを基に初期化
		InitModel(_dev, model);
	}
}