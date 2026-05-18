#pragma once
#include "stdafx.h"

namespace RNEngine {
	class DescriptorHeap;
	class Window;

	class DSVBuffer : public DescBuffer<D3D12_DEPTH_STENCIL_VIEW_DESC> {
		unique_ptr<DescriptorHeap> m_DSVHeap;

		void CreateDSVDesc(ID3D12Device* _dev);
	public:
        /// <summary>
		/// コンストラクタ
		/// </summary>
		DSVBuffer();
		/// <summary>
		/// 初期化付きコンストラクタ
		/// </summary>
		DSVBuffer(ID3D12Device* _dev, const Window* _window);
		/// <summary>
		/// デストラクタ
		/// </summary>
		~DSVBuffer();

		/// <summary>
		/// 深度ステンシルビューを初期化する。
		/// </summary>
		/// <param name="_dev">D3D12 デバイス</param>
		/// <param name="_window">ターゲットウィンドウ情報</param>
		void Init(ID3D12Device* _dev, const Window* _window);

		/// <summary>
		/// 内部のディスクリプタヒープを取得する。
		/// </summary>
		/// <returns>DescriptorHeap の生ポインタを返します。</returns>
		DescriptorHeap* GetDescriptorHeap();
	};
}