#pragma once
#include "stdafx.h"
namespace RNEngine
{
	class TextureBuffer;
	class Model;
	struct Mesh;
	struct Vertex;

	class ResourceManager
	{
		static string m_DefaultFilePath;

		static unordered_map<string, Mesh> m_MeshMap;
		static unordered_map<string, shared_ptr<Model>> m_ModelMap;
		static unordered_map<string, shared_ptr<TextureBuffer>> m_TextureBufferMap;
	public:

		static string GetDefaultFilePath() { return m_DefaultFilePath; }
		static void SetDefaultFilePath(const string& path) { m_DefaultFilePath = path; }

		static shared_ptr<TextureBuffer> RegisterTexture(const string& filename);
		static shared_ptr<TextureBuffer> GetTextureBuffer(const string& filename);

		static shared_ptr<Model> RegisterModel(const string& filename);
		static shared_ptr<Model> GetModelData(const string& filename);

		static Mesh RegisterMesh(const string& name,vector<Vertex>& vertices,vector<uint32_t>& indices);
		static Mesh GetMeshData(const string& name);

		static Mesh CreateSquare2D();
		static Mesh CreateSquare3D();
	};
}

