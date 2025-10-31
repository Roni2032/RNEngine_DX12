#include "stdafx.h"
#include "project.h"

namespace RNEngine {
	string ResourceManager::m_DefaultFilePath = "";
	unordered_map<string, shared_ptr<Model>> ResourceManager::m_ModelMap = {};
	unordered_map<string, shared_ptr<TextureBuffer>> ResourceManager::m_TextureBufferMap = {};

	shared_ptr<TextureBuffer> ResourceManager::RegisterTexture(const string& filename) {
		string filePath = m_DefaultFilePath + filename;
		auto it = m_TextureBufferMap.find(filePath);
		if (it != m_TextureBufferMap.end()) {
			return (*it).second;
		}

		auto dev = RnEngine::g_pInstance->GetDevice()->GetPtr();
		shared_ptr<TextureBuffer> texture = make_shared<TextureBuffer>();

		wstring_convert<codecvt_utf8<wchar_t>> converter;
		texture->Create(dev, converter.from_bytes(filePath));
		m_TextureBufferMap[filePath] = texture;

		return texture;
	}
	shared_ptr<TextureBuffer> ResourceManager::GetTextureBuffer(const string& filename) {
		string filePath = m_DefaultFilePath + filename;
		auto it = m_TextureBufferMap.find(filePath);
		if (it != m_TextureBufferMap.end()) {
			return (*it).second;
		}
		return nullptr;
	}


	shared_ptr<Model> ResourceManager::RegisterModel(const string& filename) {
		string filePath = m_DefaultFilePath + filename;
		auto it = m_ModelMap.find(filePath);
		if (it != m_ModelMap.end()) {
			return (*it).second;
		}

		auto dev = RnEngine::g_pInstance->GetDevice()->GetPtr();
		auto model = make_shared<Model>();
		model->Load(dev, filePath);
		m_ModelMap[filePath] = model;

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
}