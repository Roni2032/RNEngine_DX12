#pragma once
#include "stdafx.h"
#include "RendererHeader.h"
#include "Loders.h"

namespace RNEngine
{
	class DescriptorHeap;
	class PipelineState;
	class ConstantBuffer;
	class Transform;
	class Camera;

	class ModelResource
	{
		wstring m_Filename;
		Model m_ModelData;
		shared_ptr<PipelineState> m_PipelineState;

		DefaultModelTransform m_DefaultTransform;

		Matrix m_Matrix;
		vector<shared_ptr<ConstantBuffer>> m_ConstantBuffers;
		vector<ConstantBufferData> m_ConstantDates;

	public:
        ModelResource();
		~ModelResource(){}

		/// <summary>
		/// ファイルからモデルを読み込む。
		/// </summary>
		/// <param name="_dev">D3D12 デバイス</param>
		/// <param name="filename">モデルファイル名（マルチバイト文字列）</param>
		void Load(ID3D12Device* _dev, const string& filename);
		/// <summary>
		/// Mesh オブジェクトからモデルデータを読み込む。
		/// </summary>
		/// <param name="mesh">読み込む Mesh</param>
		void Load(const Mesh& mesh);

		/// <summary>
		/// モデルを描画する。
		/// </summary>
		/// <param name="cmdList">コマンドリスト</param>
		/// <param name="heap">ディスクリプタヒープ</param>
		void Draw(ComPtr<ID3D12GraphicsCommandList> cmdList, DescriptorHeap* heap);

		/// <summary>
		/// デフォルトのトランスフォームを設定する。
		/// </summary>
		/// <param name="transform">デフォルトトランスフォーム</param>
		void SetDefaultTransform(const DefaultModelTransform& transform) { m_DefaultTransform = transform; }
		/// <summary>
		/// デフォルト位置を設定する。
		/// </summary>
		/// <param name="position">位置</param>
		void SetDefaultPosition(const Vector3& position) { m_DefaultTransform.m_Position = position; }
		/// <summary>
		/// デフォルトスケールを設定する。
		/// </summary>
		/// <param name="scale">スケール値</param>
		void SetDefaultScale(float scale) { m_DefaultTransform.m_Position = scale; }
		/// <summary>
		/// デフォルト回転を設定する。
		/// </summary>
		/// <param name="rotation">回転（オイラー角）</param>
		void SetDefaultRotation(const Vector3& rotation) { m_DefaultTransform.m_Position = rotation; }

		/// <summary>
		/// モデルデータへの参照を取得する。
		/// </summary>
		/// <returns>モデルデータの参照を返します。</returns>
		Model& GetModelData() { return m_ModelData; }

		/// <summary>
		/// デフォルトトランスフォームを取得する。
		/// </summary>
		/// <returns>DefaultModelTransform の参照を返します。</returns>
		DefaultModelTransform& GetDefaultTransform() { return m_DefaultTransform; }
		/// <summary>
		/// デフォルト位置を取得する。
		/// </summary>
		/// <returns>位置ベクトルを返します。</returns>
		Vector3 GetDefaultPosition() { return m_DefaultTransform.m_Position; }
		/// <summary>
		/// デフォルトスケールを取得する。
		/// </summary>
		/// <returns>スケール値を返します。</returns>
		float GetDefaultScale() { return m_DefaultTransform.m_Scale; }
		/// <summary>
		/// デフォルト回転を取得する。
		/// </summary>
		/// <returns>回転ベクトルを返します。</returns>
		Vector3 GetDefaultRotation() { return m_DefaultTransform.m_Rotation; }

		/// <summary>
		/// マテリアルを設定する。
		/// </summary>
		/// <param name="key">マテリアルキー</param>
		/// <param name="index">マテリアルインデックス（デフォルト 0）</param>
		void SetMaterial(const string& key,int index = 0);
		/// <summary>
		/// 指定インデックスのマテリアルを取得する。
		/// </summary>
		/// <param name="index">マテリアルインデックス</param>
		/// <returns>Material の参照を返します。</returns>
		Material& GetMaterial(int index);

		/// <summary>
		/// 使用するパイプラインステートを設定する。
		/// </summary>
		/// <param name="key">パイプラインステートキー</param>
		void SetPipelineState(const wstring& key);

		/// <summary>
		/// 使用するカメラを設定する。
		/// </summary>
		/// <param name="camera">カメラの shared_ptr</param>
		void SetCamera(const shared_ptr<Camera>& camera);
		/// <summary>
		/// Transform からワールド行列を更新する。
		/// </summary>
		/// <param name="transform">Transform の shared_ptr</param>
		void UpdateWorldMatrix(const shared_ptr<Transform>& transform);
		/// <summary>
		/// 位置・スケール・回転からワールド行列を更新する。
		/// </summary>
		/// <param name="position">位置</param>
		/// <param name="scale">スケール</param>
		/// <param name="rotation">回転（クォータニオン）</param>
		void UpdateWorldMatrix(const Vector3& position, const Vector3& scale, const Quaternion& rotation);
		/// <summary>
		/// 定数バッファ用データを登録する。
		/// </summary>
		/// <param name="data">データポインタ</param>
		/// <param name="size">データサイズ</param>
		void RegisterConstantBuffer(void* data, size_t size);
		/// <summary>
		/// 定数バッファデータを GPU に転送するヘルパー。
		/// </summary>
		/// <param name="buffer">転送先バッファ</param>
		/// <param name="data">転送データ</param>
		void CopyToGPU(shared_ptr<ConstantBuffer>& buffer, ConstantBufferData& data);

		/// <summary>
		/// このモデルリソースのクローンを作成する。
		/// </summary>
		/// <returns>新しい ModelResource の shared_ptr を返します。</returns>
		shared_ptr<ModelResource> Clone() {
			shared_ptr<ModelResource> newModel = make_shared<ModelResource>();
			newModel->m_Filename = m_Filename;
			newModel->m_ModelData = m_ModelData;
			newModel->m_DefaultTransform = m_DefaultTransform;
			return newModel;
		}
	};


	
}