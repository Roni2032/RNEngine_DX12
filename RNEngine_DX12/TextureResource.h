#pragma once
#include "stdafx.h"
#include "RendererHeader.h"

namespace RNEngine {
	class DescriptorHeap;
	class TextureBuffer;
	class ConstantBuffer;
	class Camera;
	class Transform;
	class PipelineState;

	class TextureResource
	{
		Mesh m_Mesh;
		shared_ptr<TextureBuffer> m_Texture;
		shared_ptr<PipelineState> m_PipelineState;

		Matrix m_Matrix;
		vector<shared_ptr<ConstantBuffer>> m_ConstantBuffers;
		vector<ConstantBufferData> m_ConstantDates;
	public:
      TextureResource();
		~TextureResource(){}

		/// <summary>
		/// 使用するパイプラインステートをキーで設定する。
		/// </summary>
		/// <param name="key">パイプラインステートのキー</param>
		void SetPipelineState(const wstring& key);
		/// <summary>
		/// テクスチャをキーで設定する。
		/// </summary>
		/// <param name="key">テクスチャキー</param>
		void SetTexture(const wstring& key);
		/// <summary>
		/// テクスチャを直接設定する。
		/// </summary>
		/// <param name="buffer">テクスチャバッファの shared_ptr</param>
		void SetTexture(const shared_ptr<TextureBuffer>& buffer);
		/// <summary>
		/// 設定されているテクスチャを取得する。
		/// </summary>
		/// <returns>テクスチャの shared_ptr を返します。</returns>
		shared_ptr<TextureBuffer> GetTexture()const { return m_Texture; }

		/// <summary>
		/// 描画に使用するカメラを設定する。
		/// </summary>
		/// <param name="camera">カメラの shared_ptr</param>
		void SetCamera(const shared_ptr<Camera>& camera);
		/// <summary>
		/// Transform 情報からワールド行列を更新する。
		/// </summary>
		/// <param name="transform">更新元の Transform</param>
		void UpdateWorldMatrix(const shared_ptr<Transform>& transform);
		/// <summary>
		/// 位置・スケール・回転からワールド行列を更新する。
		/// </summary>
		/// <param name="position">位置</param>
		/// <param name="scale">スケール</param>
		/// <param name="rotation">回転（クォータニオン）</param>
		void UpdateWorldMatrix(const Vector3& position, const Vector3& scale, const Quaternion& rotation);
		/// <summary>
		/// 定数バッファ用のデータを登録する。
		/// </summary>
		/// <param name="data">データへのポインタ</param>
		/// <param name="size">データサイズ（バイト）</param>
		void RegisterConstantBuffer(void* data, size_t size);
		/// <summary>
		/// 定数バッファデータを GPU に転送するヘルパー。
		/// </summary>
		/// <param name="buffer">転送先の定数バッファ shared_ptr</param>
		/// <param name="data">転送するデータ</param>
		void CopyToGPU(shared_ptr<ConstantBuffer>& buffer, ConstantBufferData& data);

		/// <summary>
		/// 描画処理を行う。
		/// </summary>
		/// <param name="cmdList">使用するコマンドリスト</param>
		/// <param name="heap">使用するディスクリプタヒープ</param>
		void Draw(ComPtr<ID3D12GraphicsCommandList> cmdList, DescriptorHeap* heap);
	};
}


