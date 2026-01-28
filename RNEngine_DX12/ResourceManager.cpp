#include "stdafx.h"
#include "ResourceManager.h"
#include "RNEngine.h"

#include "TextureBuffer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include "Model.h"


namespace RNEngine {
	string ResourceManager::m_DefaultFilePath = "";

	unordered_map<string, Mesh> ResourceManager::m_MeshMap = {};
	unordered_map<string, shared_ptr<ModelResource>> ResourceManager::m_ModelMap = {};
	unordered_map<string, shared_ptr<TextureBuffer>> ResourceManager::m_TextureBufferMap = {};
	unordered_map<string, shared_ptr<Material>> ResourceManager::m_MaterialMap = {};

	shared_ptr<TextureBuffer> ResourceManager::RegisterTexture(const string& filename) {
		if (filename.empty()) return nullptr;

		string filePath = m_DefaultFilePath + filename;
		if (IsExistMap(filePath, m_TextureBufferMap)) {
			return m_TextureBufferMap[filePath];
		}
		
		auto dev = Engine::GetID3D12Device();
		shared_ptr<TextureBuffer> texture = make_shared<TextureBuffer>();

		wstring result = Util::ConvertStrToWstr(filePath);
		texture->Create(dev, result);
		m_TextureBufferMap[filePath] = texture;

		return texture;
	}
	void ResourceManager::RegisterTexture(const string& name, const shared_ptr<TextureBuffer>& texture) {
		if (!texture->IsExistsTexture()) return;
		string filePath = m_DefaultFilePath + name;

		if (IsExistMap(filePath, m_TextureBufferMap)) {
			return;
		}
		m_TextureBufferMap[filePath] = texture;
	}
	shared_ptr<TextureBuffer> ResourceManager::RegisterTexture(const string& name, const uint8_t* data, size_t dataSize) {
		string filePath = m_DefaultFilePath + name;
		if (IsExistMap(filePath, m_TextureBufferMap)) {
			return m_TextureBufferMap[filePath];
		}

		auto dev = Engine::GetID3D12Device();

		shared_ptr<TextureBuffer> texture = make_shared<TextureBuffer>();
		texture->Create(dev, reinterpret_cast<const uint8_t*>(data), dataSize);

		if (!texture->IsExistsTexture()) return nullptr;
		m_TextureBufferMap[filePath] = texture;
		return m_TextureBufferMap[filePath];
	}
	shared_ptr<TextureBuffer> ResourceManager::GetTextureBuffer(const string& filename) {
		string filePath = m_DefaultFilePath + filename;

		if (IsExistMap(filePath, m_TextureBufferMap)) {
			auto& texture = m_TextureBufferMap[filePath];
			if (texture->IsExistsTexture()) return texture;
		}
		return nullptr;
	}

	shared_ptr<ModelResource> ResourceManager::RegisterModel(const string& filename,const string& key) {
		return RegisterModel(filename, key, {});
	}
	shared_ptr<ModelResource> ResourceManager::RegisterModel(const string& filename, const DefaultModelTransform& defaultTransform = {}) {
		return RegisterModel(filename, "", defaultTransform);
	}
	shared_ptr<ModelResource> ResourceManager::RegisterModel(const string& filename, const string& key, const DefaultModelTransform& defaultTransform) {
		if (filename.empty()) return nullptr;

		string filePath = m_DefaultFilePath + filename;

		string registryKey = filePath;
		if (!key.empty()) {
			registryKey = m_DefaultFilePath + key;
		}
		if (IsExistMap(registryKey, m_ModelMap)) {
			return m_ModelMap[registryKey];
		}
		
		auto dev = Engine::GetID3D12Device();

		cout << filePath << ":モデル読み込み中..." << endl;
		auto model = make_shared<ModelResource>();
		model->Load(dev, filePath);
		model->SetDefaultTransform(defaultTransform);
		//model->SetDefaultScale(defaultTransform.m_Scale);
		//model->SetDefaultRotation(defaultTransform.m_Rotation);
		m_ModelMap[registryKey] = model;

		return m_ModelMap[registryKey];
	}
	shared_ptr<ModelResource> ResourceManager::GetModelResource(const string& filename) {
		string filePath = m_DefaultFilePath + filename;
		if (IsExistMap(filePath, m_ModelMap)) {
			return m_ModelMap[filePath];
		}
		return nullptr;
	}

	shared_ptr<ModelResource> ResourceManager::RegisterMesh(const string& name, vector<Vertex>& vertices, vector<uint32_t>& indices) {
		string registryKey = m_DefaultFilePath + name;
		if (IsExistMap(registryKey, m_ModelMap)) {
			return m_ModelMap[registryKey];
		}
		Mesh mesh;
		mesh.m_Vertices = vertices;
		mesh.m_Indices = indices;

		cout << name << ":メッシュ作成中..." << endl;
		auto dev = Engine::GetID3D12Device();
		DxUtil::CreateMeshBuffer(dev, mesh);

		auto model = make_shared<ModelResource>();
		model->Load(mesh);
		m_ModelMap[registryKey] = model;
		m_MeshMap[registryKey] = mesh;
		return m_ModelMap[registryKey];
	}
	Mesh ResourceManager::GetMeshData(const string& name) {
		string registryKey = m_DefaultFilePath + name;

		auto it = m_MeshMap.find(registryKey);
		if (it != m_MeshMap.end()) {
			return (*it).second;
		}
		return {};
	}
	//-----------------ここから下はメッシュテンプレート作成----------------------------

	shared_ptr<ModelResource> ResourceManager::CreateSquare() {
		vector<Vertex> vertices = {
			{{-0.5f, 0.5f,0.0f},{0.0f,1.0f}},
			{{ 0.5f, 0.5f,0.0f},{1.0f,1.0f}},
			{{-0.5f,-0.5f,0.0f},{0.0f,0.0f}},
			{{ 0.5f,-0.5f,0.0f},{1.0f,0.0f}}
		};
		vector<uint32_t> indices{
			0,1,2,2,1,3
		};
		return RegisterMesh("DEFAULT_SQUARE_2D", vertices, indices);

	}
	shared_ptr<ModelResource> ResourceManager::CreateCube() {
		vector<Vertex> vertices = {
			// 前面
			{{-0.5f,  0.5f, 0.5f}, {0.0f, 0.0f}},
			{{ 0.5f,  0.5f, 0.5f}, {1.0f, 0.0f}},
			{{ 0.5f, -0.5f, 0.5f}, {1.0f, 1.0f}},
			{{-0.5f, -0.5f, 0.5f}, {0.0f, 1.0f}},

			// 右面
			{{ 0.5f,  0.5f, 0.5f}, {0.0f, 0.0f}},
			{{ 0.5f,  0.5f,-0.5f}, {1.0f, 0.0f}},
			{{ 0.5f, -0.5f,-0.5f}, {1.0f, 1.0f}},
			{{ 0.5f, -0.5f, 0.5f}, {0.0f, 1.0f}},

			// 背面
			{{ 0.5f,  0.5f,-0.5f}, {0.0f, 0.0f}},
			{{-0.5f,  0.5f,-0.5f}, {1.0f, 0.0f}},
			{{-0.5f, -0.5f,-0.5f}, {1.0f, 1.0f}},
			{{ 0.5f, -0.5f,-0.5f}, {0.0f, 1.0f}},

			// 左面
			{{-0.5f,  0.5f,-0.5f}, {0.0f, 0.0f}},
			{{-0.5f,  0.5f, 0.5f}, {1.0f, 0.0f}},
			{{-0.5f, -0.5f, 0.5f}, {1.0f, 1.0f}},
			{{-0.5f, -0.5f,-0.5f}, {0.0f, 1.0f}},

			// 上面
			{{-0.5f, 0.5f,-0.5f}, {0.0f, 0.0f}},
			{{ 0.5f, 0.5f,-0.5f}, {1.0f, 0.0f}},
			{{ 0.5f, 0.5f, 0.5f}, {1.0f, 1.0f}},
			{{-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f}},

			// 下面
			{{-0.5f,-0.5f, 0.5f}, {0.0f, 0.0f}},
			{{ 0.5f,-0.5f, 0.5f}, {1.0f, 0.0f}},
			{{ 0.5f,-0.5f,-0.5f}, {1.0f, 1.0f}},
			{{-0.5f,-0.5f,-0.5f}, {0.0f, 1.0f}},
		};
		vector<uint32_t> indices = {
			0,1,2,  0,2,3,      // 前
			4,5,6,  4,6,7,      // 右
			8,9,10, 8,10,11,    // 後
			12,13,14, 12,14,15, // 左
			16,17,18, 16,18,19, // 上
			20,21,22, 20,22,23  // 下
		};
		return RegisterMesh("DEFAULT_SQUARE_3D", vertices, indices);

	}

	shared_ptr<ModelResource> ResourceManager::CreateSphere(const int sliceCount, const int stackCount) {
		vector<Vertex> vertices;
		vector<uint32_t> indices;

		// 頂点生成
		for (int stack = 0; stack <= stackCount; ++stack) {
			float phi = XM_PI * stack / stackCount;
			for (int slice = 0; slice <= sliceCount; ++slice) {
				float theta = XM_2PI * slice / sliceCount;
				float x = sinf(phi) * cosf(theta);
				float y = cosf(phi);
				float z = sinf(phi) * sinf(theta);
				float u = static_cast<float>(slice) / sliceCount;
				float v = static_cast<float>(stack) / stackCount;
				vertices.push_back({ {x * 0.5f, y * 0.5f, z * 0.5f}, {u, v} });
			}
		}
		// インデックス生成
		for (int stack = 0; stack < stackCount; ++stack) {
			for (int slice = 0; slice < sliceCount; ++slice) {
				int first = (stack * (sliceCount + 1)) + slice;
				int second = first + sliceCount + 1;
				indices.push_back(first);
				indices.push_back(second);
				indices.push_back(first + 1);
				indices.push_back(second);
				indices.push_back(second + 1);
				indices.push_back(first + 1);
			}
		}
		return RegisterMesh("DEFAULT_SPHERE", vertices, indices);
	}
	Mesh& ResourceManager::CreateLine() {
		vector<Vertex> vertices = {
			{{0.0f,0.0f,0.0f},{0.0f,0.0f}},
			{{1.0f,1.0f,1.0f},{1.0f,1.0f}}
		};
		vector<uint32_t> indices{
			0,1
		};
		auto mesh = RegisterMesh("DEFAULT_LINE", vertices, indices);
		return m_MeshMap["DEFAULT_LINE"];
	}
}