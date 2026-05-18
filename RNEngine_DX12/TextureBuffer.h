#pragma once
#include "stdafx.h"

namespace RNEngine {
	class SRVBuffer;

	class TextureBuffer : public BufferBase {
		UINT m_Handle;
		unique_ptr<SRVBuffer> m_Srv;
		wstring m_Filename;

		bool m_IsExistsFile;
	public:
		TextureBuffer();
		~TextureBuffer();

		/// <summary>
		/// ファイルからテクスチャを作成
		/// </summary>
		/// <param name="_dev">DX12デバイス</param>
		/// <param name="filename">ファイルパス</param>
		void Create(ID3D12Device* _dev, const wstring& filename);

		/// <summary>
		/// 指定したサイズ、フォーマットでテクスチャを作成
		/// </summary>
		/// <param name="_dev">DX12デバイス</param>
		/// <param name="width">幅</param>
		/// <param name="height">高さ</param>
		/// <param name="format">カラーフォーマット</param>
		/// <param name="clearColor">初期カラー</param>
       /// <summary>
		/// 指定幅・高さで空のテクスチャを作成する。
		/// </summary>
		/// <param name="_dev">DX12 デバイス</param>
		/// <param name="width">幅</param>
		/// <param name="height">高さ</param>
		/// <param name="format">フォーマット</param>
		/// <param name="clearColor">クリアカラー</param>
		void Create(ID3D12Device* _dev, UINT width, UINT height, DXGI_FORMAT format, array<float, 4> clearColor);

		/// <summary>
		/// 内部埋め込みデータからテクスチャを作成
		/// </summary>
		/// <param name="_dev">DX12デバイス</param>
		/// <param name="data">埋め込みデータ</param>
		/// <param name="dataSize">データサイズ</param>
      /// <summary>
		/// メモリ上のイメージデータからテクスチャを作成する。
		/// </summary>
		/// <param name="_dev">DX12 デバイス</param>
		/// <param name="data">ピクセルデータへのポインタ</param>
		/// <param name="dataSize">データサイズ（バイト）</param>
		void Create(ID3D12Device* _dev, const uint8_t* data, size_t dataSize);

		/// <summary>
		/// リソースの作成
		/// </summary>
		/// <param name="width">幅</param>
		/// <param name="height">高さ</param>
		/// <param name="format">カラーフォーマット</param>
		/// <param name="flag">リソースフラグ</param>
		/// <param name="clearColor">初期化カラー</param>
       /// <summary>
		/// リソースを直接作成する（詳細指定）。
		/// </summary>
		/// <param name="width">幅</param>
		/// <param name="height">高さ</param>
		/// <param name="format">フォーマット</param>
		/// <param name="flag">リソースフラグ</param>
		/// <param name="clearColor">クリアカラー</param>
		void CreateResource(UINT width, UINT height, DXGI_FORMAT format, D3D12_RESOURCE_FLAGS flag = D3D12_RESOURCE_FLAG_NONE, array<float, 4> clearColor = { 0,0,0,1 });

		/// <summary>
		/// バッファを取得
		/// </summary>
		/// <returns>テクスチャバッファ</returns>
     /// <summary>
		/// 内部のリソース（テクスチャ）の生ポインタを取得する。
		/// </summary>
		/// <returns>ID3D12Resource の生ポインタを返します。</returns>
		ID3D12Resource* GetBuffer()const { return m_Buffer.Get(); }

		/// <summary>
		/// テクスチャが存在するかどうか
		/// </summary>
		/// <returns></returns>
     /// <summary>
		/// テクスチャが存在するかどうかを判定する。
		/// </summary>
		/// <returns>存在する場合は true を返します。</returns>
		bool IsExistsTexture();

		/// <summary>
		/// SRVハンドルの設定
		/// </summary>
		/// <param name="handle">ハンドル</param>
       /// <summary>
		/// SRV ハンドルを設定する。
		/// </summary>
		/// <param name="handle">ハンドル値</param>
		void SetSRVHandle(UINT handle) { m_Handle = handle; }

		/// <summary>
		/// SRVハンドルの取得
		/// </summary>
		/// <returns>SRVハンドル</returns>
       /// <summary>
		/// SRV ハンドルを取得する。
		/// </summary>
		/// <returns>ハンドル値を返します。</returns>
		UINT GetSRVHandle()const { return m_Handle; }

		/// <summary>
		/// SRVの取得
		/// </summary>
		/// <returns>SRV</returns>
     /// <summary>
		/// SRV バッファを取得する。
		/// </summary>
		/// <returns>SRVBuffer の生ポインタを返します。</returns>
		SRVBuffer* GetSRV() { return m_Srv.get(); }
	};
}