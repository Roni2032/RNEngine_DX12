#pragma once
#include "stdafx.h"

namespace RNEngine
{
	class Material {

	};
	struct Mesh {
		vector<Vertex> m_Verteces;//頂点データ
		vector<uint32_t> m_Indeces;//頂点インデックス

		shared_ptr<VertexBuffer> m_VertexBuffer;
		shared_ptr<IndexBuffer> m_IndexBuffer;
		uint32_t m_MaterialIndex;
	};

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


	string GetModelNameFromPath(const string& filepath);
	void SaveBainaryModel(const string& filename, BainaryModelDeta& modelData);
	BainaryModelDeta LoadBainaryModel(const string& filename);

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

		void Load(ComPtr<ID3D12Device>& _dev, const string& filename);

		void Draw(ComPtr<ID3D12GraphicsCommandList> cmdList, const DescriptorHeap* heap, const ConstBuffer* constantBuffer);

		void OutputDebug(const aiScene* scene);

		shared_ptr<Model> Clone() {
			shared_ptr<Model> newModel = make_shared<Model>();
			newModel->m_Filename = m_Filename;
			newModel->m_Meshes = m_Meshes;
			newModel->m_MaterialTextureName = m_MaterialTextureName;
			
			return newModel;
		}
	};


	class ModelRenderer {
		unique_ptr<ConstBuffer> m_ConstantBuffer;
		Matrix m_Matrix;

		shared_ptr<Model> m_Model;
	public:
		ModelRenderer() :m_Matrix{} {}
		~ModelRenderer(){}
		void Init(XMFLOAT3 eye,XMFLOAT3 target);

		void SetMatrix(const Matrix& matrix) {
			m_Matrix = matrix;
		}
		void SetModel(const string& filename) {
			m_Model = ResourceManager::GetModelData(filename);
		}

		void SetMatrixWorld(XMFLOAT3 position, XMFLOAT3 rotation, XMFLOAT3 scale);

		void Draw(ComPtr<ID3D12GraphicsCommandList> cmdList, const DescriptorHeap* heap) {
			m_Model->Draw(cmdList, heap,m_ConstantBuffer.get());
		}
	};
}