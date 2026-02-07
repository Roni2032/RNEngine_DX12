#pragma once

#include "Model.h"
#include "stdafx.h"
#include "File.h"
#include "ResourceManager.h"
#include "RendererHeader.h"

namespace RNEngine {
	class AssimpLoader {
		//いつか普通の変数にしましょう
		static Assimp::Importer g_Importer;

		/// <summary>
		/// メッシュ読み込み
		/// </summary>
		/// <param name="_dev">D3D12デバイス</param>
		/// <param name="output">読み込み対象</param>
		/// <param name="scene">Assimpデータ</param>
		static void ImportMesh(ID3D12Device* _dev, Model& output, const aiScene* scene);

		/// <summary>
		/// マテリアル読み込み
		/// </summary>
		/// <param name="output">読み込み対象</param>
		/// <param name="scene">Assimpデータ</param>
		static void ImportMaterial(Model& output, const aiScene* scene,const string& filePath);

		/// <summary>
		///	Assimpデータの初期化
		/// </summary>
		/// <param name="filePath">モデルパス</param>
		/// <returns>Assimpデータ</returns>
		static const aiScene* InitializeScene(const string& filePath);

		/// <summary>
		/// マテリアルデータから登録されたテクスチャ名を取得
		/// </summary>
		/// <param name="material">マテリアルデータ</param>
		/// <returns>テクスチャ名</returns>
		static string GetTextureName(aiMaterial* material);
	public:
		/// <summary>
		/// 指定されたファイルパスからモデルをインポートします。
		/// </summary>
		/// <param name="_dev">Direct3D 12 デバイスへのポインタ。</param>
		/// <param name="output">インポートされたモデルを格納するための参照。</param>
		/// <param name="filePath">インポートするモデルファイルのパス。</param>
		static void Import(ID3D12Device* _dev, Model& output, const wstring& filePath);
	};

	//バイナリ用構造体
	namespace MeshFile {
		struct ModelHeader {
			uint32_t m_MeshCount;
			uint32_t m_MaterialCount;
		};
		struct MeshHeader {
			uint32_t m_VertexCount;
			uint32_t m_IndexCount;
			uint32_t m_materialIndex;
		};
	}

	class MeshIO {
	public:
		static bool IsExistMeshFile(const wstring& filePath);
		static wstring GetMeshFileName(const wstring& filePath);
	};
	
	class MeshWriter {
		static void DeleteDefaultFilePath(string& filePath);
	public:
		static void SaveMeshFile(const Model& model, const wstring& meshPath);
	};
	class MeshLoader {
		static void InitModel(ID3D12Device* _dev,Model& model);
	public:
		static void AdjustModelSizeMatrix(Model& model, const Vector3& size);
		static void LoadMeshFile(ID3D12Device* _dev,Model& model, const wstring& meshPath);

	};
}