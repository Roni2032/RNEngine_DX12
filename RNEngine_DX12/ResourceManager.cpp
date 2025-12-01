#include "stdafx.h"
#include "project.h"

namespace RNEngine {
	string ResourceManager::m_DefaultFilePath = "";
	unordered_map<string, Mesh> ResourceManager::m_MeshMap = {};
	unordered_map<string, shared_ptr<Model>> ResourceManager::m_ModelMap = {};
	unordered_map<string, shared_ptr<TextureBuffer>> ResourceManager::m_TextureBufferMap = {};

	shared_ptr<TextureBuffer> ResourceManager::RegisterTexture(const string& filename) {
		string filePath = m_DefaultFilePath + filename;
		auto it = m_TextureBufferMap.find(filePath);
		if (it != m_TextureBufferMap.end()) {
			return (*it).second;
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

		auto it = m_TextureBufferMap.find(filePath);
		if (it != m_TextureBufferMap.end()) {
			return;
		}
		m_TextureBufferMap[filePath] = texture;
	}
	shared_ptr<TextureBuffer> ResourceManager::GetTextureBuffer(const string& filename) {
		string filePath = m_DefaultFilePath + filename;
		auto it = m_TextureBufferMap.find(filePath);
		if (it != m_TextureBufferMap.end()) {
			return (*it).second;
		}
		return nullptr;
	}

	shared_ptr<Model> ResourceManager::RegisterModel(const string& filename,const string& key) {
		string filePath = m_DefaultFilePath + filename;

		string registryKey = filePath;
		if(!key.empty()) {
			registryKey = m_DefaultFilePath + key;
		}
		auto it = m_ModelMap.find(registryKey);
		if (it != m_ModelMap.end()) {
			return (*it).second;
		}

		auto dev = Engine::GetID3D12Device();
		auto model = make_shared<Model>();
		model->Load(dev, filePath);
		m_ModelMap[registryKey] = model;

		return model;
	}
	shared_ptr<Model> ResourceManager::GetModelData(const string& filename) {
		string filePath = m_DefaultFilePath + filename;
		auto it = m_ModelMap.find(filePath);
		if (it != m_ModelMap.end()) {
			return (*it).second->Clone();
		}
		return nullptr;
	}

	Mesh ResourceManager::RegisterMesh(const string& name, vector<Vertex>& vertices, vector<uint32_t>& indices) {
		auto it = m_MeshMap.find(name);
		if (it != m_MeshMap.end()) {
			return (*it).second;
		}
		Mesh mesh;
		mesh.m_Vertices = vertices;
		mesh.m_Indices = indices;

		auto dev = Engine::GetID3D12Device();
		mesh.m_VertexBuffer = make_shared<VertexBuffer>();
		mesh.m_VertexBuffer->Create(dev, mesh.m_Vertices);

		mesh.m_IndexBuffer = make_shared<IndexBuffer>();
		mesh.m_IndexBuffer->Create(dev, mesh.m_Indices);

		m_MeshMap[name] = mesh;
		return mesh;
	}
	Mesh ResourceManager::GetMeshData(const string& name) {
		auto it = m_MeshMap.find(name);
		if (it != m_MeshMap.end()) {
			return (*it).second;
		}
		return {};
	}

	//-----------------ここから下はメッシュテンプレート作成----------------------------

	Mesh ResourceManager::CreateSquare2D() {
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
	Mesh ResourceManager::CreateSquare3D() {
		vector<Vertex> vertices = {
			{{-0.5f, 0.5f,0.0f},{0.0f,0.0f}},
			{{ 0.5f, 0.5f,0.0f},{1.0f,0.0f}},
			{{-0.5f,-0.5f,0.0f},{0.0f,1.0f}},
			{{ 0.5f,-0.5f,0.0f},{1.0f,1.0f}}
		};
		vector<uint32_t> indices{
			0,1,2,2,1,3
		};
		return RegisterMesh("DEFAULT_SQUARE_3D", vertices, indices);

	}
}