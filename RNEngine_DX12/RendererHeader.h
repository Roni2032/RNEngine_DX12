#pragma once
/// -------------------------------------------------
///   描画に必要なデータまとめ
/// -------------------------------------------------
namespace RNEngine{
	struct Vertex {
		XMFLOAT3 m_Position;
		XMFLOAT2 m_Uv;
	};

	struct Matrix {
		XMMATRIX m_World;
		XMMATRIX m_ViewProjection;
	};

	class VertexBuffer;
	class IndexBuffer;

	struct Mesh {
		vector<Vertex> m_Verteces;//頂点データ
		vector<uint32_t> m_Indeces;//頂点インデックス

		shared_ptr<VertexBuffer> m_VertexBuffer;
		shared_ptr<IndexBuffer> m_IndexBuffer;
		uint32_t m_MaterialIndex;
	};
}