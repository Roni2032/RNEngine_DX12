#pragma once
#include "stdafx.h"
#include "RendererHeader.h"
namespace RNEngine
{

	/// <summary>
	/// 未完成部分の忘れないように説明
	/// 読み込んだモデルデータをバイナリデータとして保存
	/// 2回目以降の読み込みを高速化する
	/// 
	/// .meshファイルとして保存する
	/// Headerにはデータの詳細(メッシュ数や頂点数などデータの数)が含まれる
	/// その後頂点、インデックス、マテリアルテクスチャ名などのデータが連続する
	///
	/// 読み込みの際はHeaderの情報をもとにデータを読み込む
	/// </summary>
	struct BainaryModelHeader {
		uint32_t m_MeshCount;
		uint32_t m_MaterialCount;
		uint32_t m_VertexCount;
		uint32_t m_IndexCount;
	};
	struct BainaryModelDeta {
		BainaryModelHeader m_Header;
		vector<string> m_MaterialTextureName;
		vector<Mesh> m_Meshes;
	};

	class Model
	{
		wstring m_Filename;

		vector<Mesh> m_Meshes;
		vector<string> m_MaterialTextureName;

		bool m_IsDebug;	//trueの場合は読み込みの際にtxtファイルでデータを書きだす
		string m_DebugName;


		struct Header {
			uint32_t m_MeshCount;
			uint32_t m_MaterialCount;
		};
		struct MeshHeader {
			uint32_t m_VertexCount;
			uint32_t m_IndexCount;
			uint32_t m_materialIndex;
		};
		string GetModelNameFromPath(const string& filepath);
		void DeleteDefaultFilePath(string& filePath);
		void SaveBinaryModel(const string& filename, vector<Mesh>& mesh, vector<string>& materialTextures);
		void LoadBinaryModel(const string& filename, vector<Mesh>& mesh, vector<string>& materialTextures);
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