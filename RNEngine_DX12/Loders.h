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

		static void ImportBone(Mesh& output, const aiMesh* mesh, unordered_map<string, uint32_t>& boneIndexMap);
		static void ImportAnimations(Model& output, const aiScene* scene);
		static void ImportAnimation(Model& output, const aiAnimation* animation, const aiNode* node);
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
	namespace BinaryHeaer {
		struct ModelBinaryHeader {
			uint32_t m_MeshCount;
			uint32_t m_MaterialCount;
		};
		struct MeshBinaryHeader {
			uint32_t m_VertexCount;
			uint32_t m_IndexCount;
			uint32_t m_materialIndex;
		};

		struct  BoneBinary {
			uint32_t nameLength;
			XMFLOAT4X4 offsetMatrix;
			XMFLOAT4X4 defaultMatrix;
		};
		struct AnimationBinary {
			XMFLOAT4X4 globalInverseMatrix;
			uint32_t clipCount;
			vector<AnimationClipBinaryHeader> clipHeaders;
		};
		struct AnimationClipBinaryHeader {
			float tickPerSecond;
			float duration;

			uint32_t positionKeyFrameCount;
			uint32_t quaternionKeyFrameCount;
			uint32_t scalingKeyFrameCount;
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

	//現在の.meshのデータ構造
	// 
	//(テクスチャタイプが埋め込みの場合)
	//[(uint32_t)メッシュ数,(uint32_t)マテリアル数]
	//[(Vector3)頂点データ,(uint32_t)インデックスデータ,(uint32_t)マテリアルインデックス]
	//    :
	//[(enum)テクスチャタイプ,(uint32_t)テクスチャ名の文字数,(string)テクスチャ名,
	// (uint32_t)テクスチャフォーマット名の文字数,(string)テクスチャフォーマット名,
	// (uint32_t)テクスチャデータサイズ,(vector<uint32_t>)テクスチャデータ]
	//    :
	//
	// 
	//(テクスチャタイプがファイル参照の場合)
	//[(uint32_t)メッシュ数,(uint32_t)マテリアル数]
	//[(Vector3)頂点データ,(uint32_t)インデックスデータ,(uint32_t)マテリアルインデックス]
	//    :
	//[(enum)テクスチャタイプ,(uint32_t)テクスチャ名の文字数,(string)テクスチャ名]
	//    :
	//
	//
}