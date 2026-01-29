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
		void Create(ID3D12Device* _dev, UINT width, UINT height, DXGI_FORMAT format, array<float, 4> clearColor);

		/// <summary>
		/// 内部埋め込みデータからテクスチャを作成
		/// </summary>
		/// <param name="_dev">DX12デバイス</param>
		/// <param name="data">埋め込みデータ</param>
		/// <param name="dataSize">データサイズ</param>
		void Create(ID3D12Device* _dev, const uint8_t* data, size_t dataSize);

		/// <summary>
		/// リソースの作成
		/// </summary>
		/// <param name="width">幅</param>
		/// <param name="height">高さ</param>
		/// <param name="format">カラーフォーマット</param>
		/// <param name="flag">リソースフラグ</param>
		/// <param name="clearColor">初期化カラー</param>
		void CreateResource(UINT width, UINT height, DXGI_FORMAT format, D3D12_RESOURCE_FLAGS flag = D3D12_RESOURCE_FLAG_NONE, array<float, 4> clearColor = { 0,0,0,1 });

		/// <summary>
		/// バッファを取得
		/// </summary>
		/// <returns>テクスチャバッファ</returns>
		ID3D12Resource* GetBuffer()const { return m_Buffer.Get(); }

		/// <summary>
		/// テクスチャが存在するかどうか
		/// </summary>
		/// <returns></returns>
		bool IsExistsTexture();

		/// <summary>
		/// SRVハンドルの設定
		/// </summary>
		/// <param name="handle">ハンドル</param>
		void SetSRVHandle(UINT handle) { m_Handle = handle; }

		/// <summary>
		/// SRVハンドルの取得
		/// </summary>
		/// <returns>SRVハンドル</returns>
		UINT GetSRVHandle()const { return m_Handle; }

		/// <summary>
		/// SRVの取得
		/// </summary>
		/// <returns>SRV</returns>
		SRVBuffer* GetSRV() { return m_Srv.get(); }
	};
}