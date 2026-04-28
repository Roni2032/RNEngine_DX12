#pragma once
#include "stdafx.h"
#include "Singleton.h"
namespace RNEngine
{
	class TextureBuffer;
	class ModelResource;
	struct Mesh;
	struct Vertex;
	struct DefaultModelTransform;
	struct Material;

	class ResourceManager
	{
		static string m_DefaultFilePath;

		static unordered_map<string, Mesh> m_MeshMap;
		static unordered_map<string, shared_ptr<ModelResource>> m_ModelMap;
		static unordered_map<string, shared_ptr<TextureBuffer>> m_TextureBufferMap;
		static unordered_map<string, shared_ptr<Material>> m_MaterialMap;

		template<class T>
		static bool IsExistMap(const string& key, unordered_map<string, shared_ptr<T>>& map) {
			auto it = map.find(key);
			return it != map.end();
		}
	public:

		static string GetDefaultFilePath() { return m_DefaultFilePath; }
		static void SetDefaultFilePath(const string& path) { m_DefaultFilePath = path; }

		static void LoadAssets(const string& folder = "");
		static void UnLoadAssets(const string& folder = "");

		static shared_ptr<TextureBuffer> RegisterTexture(const string& filename);
		static void RegisterTexture(const string& name,const shared_ptr<TextureBuffer>& texture);
		static shared_ptr<TextureBuffer> RegisterTexture(const string& name, const uint8_t* data, size_t dataSize);
		static shared_ptr<TextureBuffer> GetTextureBuffer(const string& filename);

		static shared_ptr<ModelResource> RegisterModel(const string& filename,const string& key = "");
		static shared_ptr<ModelResource> RegisterModel(const string& filename,const DefaultModelTransform& defaultTransform);
		static shared_ptr<ModelResource> RegisterModel(const string& filename,const string& key, const DefaultModelTransform& defaultTransform);
		static shared_ptr<ModelResource> GetModelResource(const string& filename);

		static shared_ptr<ModelResource> RegisterMesh(const string& name,vector<Vertex>& vertices,vector<uint32_t>& indices);
		static Mesh GetMeshData(const string& name);

		static shared_ptr<ModelResource> CreateSquare();
		static shared_ptr<ModelResource> CreateCube();

		static shared_ptr<ModelResource> CreateSphere(const int sliceCount, const int stackCount);
		static Mesh& CreateLine();

		static shared_ptr<ModelResource> CreateCylinder(const int sliceCount);
		static shared_ptr<ModelResource> CreateCone(const int slineCount);

	};
}

