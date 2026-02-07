#pragma once
/// -------------------------------------------------
///   描画に必要なデータまとめ
/// -------------------------------------------------

#include "Vector.h"
namespace RNEngine{
	//GPU用頂点データ
	struct Vertex {
		Vector3 m_Position;
		Vector2 m_Uv;
	};
	//GPU用行列データ
	struct Matrix {
		XMMATRIX m_World;
		XMMATRIX m_ViewProjection;
	};

	class VertexBuffer;
	class IndexBuffer;
	// メッシュデータ
	struct Mesh {
		vector<Vertex> m_Vertices;//頂点データ
		vector<uint32_t> m_Indices;//頂点インデックス

		shared_ptr<VertexBuffer> m_VertexBuffer;
		shared_ptr<IndexBuffer> m_IndexBuffer;
		uint32_t m_MaterialIndex = 0;
	};

	enum class TextureDataType {
		None,//未使用
		File,//ファイルパス
		Embedded//埋め込みデータ
	};
	// 埋め込みテクスチャデータ
	struct EmbeddedTexture {
		string m_Name;
		string m_Format;
		uint32_t m_Size = 0;
		vector<uint8_t> m_Data;

		EmbeddedTexture() = default;
		EmbeddedTexture(const string& name,aiTexture* texture) {
			m_Name = "embedded_texture_" + name;
			m_Format = texture->achFormatHint;
			m_Size = texture->mWidth;
			m_Data.resize(texture->mWidth);
			::CopyMemory(m_Data.data(), texture->pcData, texture->mWidth);
		}
	};
	struct Material {
		TextureDataType m_TextureType;

		string m_TextureName;
		EmbeddedTexture m_EmbeddedTexture;

		bool IsEmpty() {
			return m_TextureType == TextureDataType::None;
		}
		Material(TextureDataType type, const string& textureName, const EmbeddedTexture& embeddedTexture) :
			m_TextureType(type), m_TextureName(textureName), m_EmbeddedTexture(embeddedTexture) {
		}

		Material() :Material(TextureDataType::None, "", {}) {}
		Material(const string& filename) :Material(TextureDataType::File, filename, {}) {}
		Material(EmbeddedTexture& texture) :Material(TextureDataType::Embedded, texture.m_Name, texture) {}
	};

	struct Model {
		vector<Mesh> m_Meshes;
		vector<Material> m_Materials;

		XMMATRIX m_AdjustMatrix;
	};

	//モデル初期行列データ
	struct DefaultModelTransform {
		float m_Scale;
		Vector3 m_Rotation;
		Vector3 m_Position;
		DefaultModelTransform();
		DefaultModelTransform(float scale, Vector3 rotation, Vector3 position);
	};

	struct ConstantBufferData {
		void* m_Data;
		size_t m_DataSize;
	};
}