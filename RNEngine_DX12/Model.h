#pragma once
#include "stdafx.h"
#include "RendererHeader.h"
namespace RNEngine
{
	enum class TextureDataType {
		None,//未使用
		File,//ファイルパス
		Embedded//埋め込みデータ
	};
	struct Material {
		TextureDataType m_TextureType;

		string m_TextureName;
		EmbeddedTexture m_EmbeddedTexture;

		Material(TextureDataType type, const string& textureName, const EmbeddedTexture& embeddedTexture) :
			m_TextureType(type), m_TextureName(textureName), m_EmbeddedTexture(embeddedTexture){}

		Material() :Material(TextureDataType::None,"",{}) {}
		Material(const string& filename):Material(TextureDataType::File,filename,{}) {}
		Material(EmbeddedTexture& texture):Material(TextureDataType::Embedded,texture.m_Name,texture){}
	};
	class Model
	{
		wstring m_Filename;
		vector<Mesh> m_Meshes;
		vector<shared_ptr<Material>> m_Materials;

		float m_DefaultScale;
		Vector3 m_DefaultRotation;

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
		void SaveBinaryModel(const string& filename, vector<Mesh>& mesh, vector<shared_ptr<Material>>& materials);
		void LoadBinaryModel(const string& filename, vector<Mesh>& mesh, vector<shared_ptr<Material>>& materials);

		void CreateBuffer(ID3D12Device* _dev,Mesh& mesh);
		void OutputDebug(const aiScene* scene);
	public:
		Model():Model(false,""){}
		Model(bool isDebug, const string& name);
		~Model(){}

		void Load(ID3D12Device* _dev, const string& filename);
		void Load(const Mesh& mesh);

		void Draw(ComPtr<ID3D12GraphicsCommandList> cmdList, DescriptorHeap* heap, const ConstBuffer* constantBuffer);


		shared_ptr<Model> Clone() {
			shared_ptr<Model> newModel = make_shared<Model>();
			newModel->m_Filename = m_Filename;
			newModel->m_Meshes = m_Meshes;
			newModel->m_Materials = m_Materials;
			newModel->m_DefaultScale = m_DefaultScale;
			newModel->m_DefaultRotation = m_DefaultRotation;
			return newModel;
		}

		void SetDefaultScale(float scale) {
			m_DefaultScale = scale;
		}
		void SetDefaultRotation(Vector3 rotation) {
			m_DefaultRotation = rotation;
		}

		float GetDefaultScale() {
			return m_DefaultScale;
		}
		Vector3 GetDefaultRotation() {
			return m_DefaultRotation;
		}

		void SetMaterial(const string& key,int index = 0);

		shared_ptr<Material> GetMaterial(int index);
	};


	
}