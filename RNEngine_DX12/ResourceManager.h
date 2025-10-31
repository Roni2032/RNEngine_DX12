#pragma once
#include "stdafx.h"
namespace RNEngine
{
	class TextureBuffer;
	class Model;

	class ResourceManager
	{
		static string m_DefaultFilePath;
		static unordered_map<string, shared_ptr<Model>> m_ModelMap;
		static unordered_map<string, shared_ptr<TextureBuffer>> m_TextureBufferMap;
	public:
		static void SetDefaultFilePath(const string& path) { m_DefaultFilePath = path; }

		static shared_ptr<TextureBuffer> RegisterTexture(const string& filename);
		static shared_ptr<TextureBuffer> GetTextureBuffer(const string& filename);

		static shared_ptr<Model> RegisterModel(const string& filename);
		static shared_ptr<Model> GetModelData(const string& filename);
	};
}

