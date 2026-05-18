#pragma once
#include "stdafx.h"

namespace RNEngine {
	class TextureBuffer;

	class SRVBuffer : public DescBuffer<D3D12_SHADER_RESOURCE_VIEW_DESC> {
	public:
        /// <summary>
		/// コンストラクタ
		/// </summary>
		SRVBuffer();
		/// <summary>
		/// デストラクタ
		/// </summary>
		~SRVBuffer();
		/// <summary>
		/// テクスチャバッファを指定して SRV を初期化する。
		/// </summary>
		/// <param name="_dev">D3D12 デバイス</param>
		/// <param name="texBuffer">使用するテクスチャバッファ</param>
		/// <param name="format">SRV 用のフォーマット</param>
		void Init(ID3D12Device* _dev, TextureBuffer* texBuffer, DXGI_FORMAT format);
		/// <summary>
		/// SRV を初期化する（テクスチャ未指定）。
		/// </summary>
		/// <param name="_dev">D3D12 デバイス</param>
		/// <param name="format">SRV 用のフォーマット</param>
		void Init(ID3D12Device* _dev, DXGI_FORMAT format);
		/// <summary>
		/// SRV のディスクリプタ記述を作成する。
		/// </summary>
		/// <param name="_dev">D3D12 デバイス</param>
		/// <param name="format">フォーマット</param>
		void CreateSRVDesc(ID3D12Device* _dev, DXGI_FORMAT format);
	};
}