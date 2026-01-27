#pragma once
#include "stdafx.h"
namespace RNEngine {
	class ModelResource;
	class TextureResource;

	class RenderingEngine
	{
		vector<shared_ptr<ModelResource>> m_RenderModelResource;
		vector<shared_ptr<TextureResource>> m_RenderTextureResource;

	public:

	};
}
