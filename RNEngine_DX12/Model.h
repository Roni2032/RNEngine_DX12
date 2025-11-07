#pragma once
#include "stdafx.h"
#include "RendererHeader.h"
namespace RNEngine
{

	class Model
	{
		wstring m_Filename;

		vector<Mesh> m_Meshes;
		vector<string> m_MaterialTextureName;

		bool m_IsDebug;	//trueの場合は読み込みの際にtxtファイルでデータを書きだす
		string m_DebugName;
	public:
		Model():m_IsDebug(false){}
		Model(bool isDebug,const string& name): m_IsDebug(isDebug), m_DebugName(name){}
		~Model(){}

		void Load(ID3D12Device* _dev, const string& filename);

		void Draw(ComPtr<ID3D12GraphicsCommandList> cmdList, DescriptorHeap* heap, const ConstBuffer* constantBuffer);

		void OutputDebug(const aiScene* scene);

		shared_ptr<Model> Clone() {
			shared_ptr<Model> newModel = make_shared<Model>();
			newModel->m_Filename = m_Filename;
			newModel->m_Meshes = m_Meshes;
			newModel->m_MaterialTextureName = m_MaterialTextureName;
			
			return newModel;
		}
	};


	
}