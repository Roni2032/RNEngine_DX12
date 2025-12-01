#pragma once
/// -------------------------------------------------
///   描画に必要なデータまとめ
/// -------------------------------------------------
namespace RNEngine{
	//GPU用頂点データ
	struct Vertex {
		XMFLOAT3 m_Position;
		XMFLOAT2 m_Uv;
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
}