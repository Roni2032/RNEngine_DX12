#pragma once
#include "stdafx.h"
#include "Device.h"

namespace RNEngine {
	class DescriptorHeap;
	class SwapChain;


	class RTVBuffer {
		unique_ptr<DescriptorHeap> m_RTVHeap;
		vector<ComPtr<ID3D12Resource>> m_BackBuffer;
		vector<D3D12_RESOURCE_STATES> m_BufferStates;

		void CreateRTV(ID3D12Device* _dev, UINT count);
	public:
		D3D12_RENDER_TARGET_VIEW_DESC m_RTVDesc{};
        /// <summary>
		/// コンストラクタ
		/// </summary>
		RTVBuffer();
		/// <summary>
		/// 初期化付きコンストラクタ
		/// </summary>
		RTVBuffer(ID3D12Device* _dev, SwapChain* _swapChain);
		/// <summary>
		/// デストラクタ
		/// </summary>
		~RTVBuffer();

		/// <summary>
		/// フレームバッファ用の RTV を初期化する。
		/// </summary>
		/// <param name="_dev">D3D12 デバイス</param>
		/// <param name="_swapChain">スワップチェイン オブジェクト</param>
		void InitFrameBuffer(ID3D12Device* _dev, SwapChain* _swapChain);
		/// <summary>
		/// RTV バッファを初期化する。
		/// </summary>
		/// <param name="_dev">D3D12 デバイス</param>
		void Init(ID3D12Device* _dev);
		/// <summary>
		/// 内部のディスクリプタヒープを取得する。
		/// </summary>
		/// <returns>DescriptorHeap の生ポインタを返します。</returns>
		DescriptorHeap* GetDescriptorHeap();

		/// <summary>
		/// 指定したインデックスのバックバッファを取得する。
		/// </summary>
		/// <param name="index">バックバッファのインデックス</param>
		/// <returns>ID3D12Resource の生ポインタを返します。</returns>
		ID3D12Resource* GetBackBuffer(size_t index) {
			if (m_BackBuffer.size() <= index) throw;
			return m_BackBuffer[index].Get();
		}
		/// <summary>
		/// 指定したインデックスのバッファ状態を取得する。
		/// </summary>
		/// <param name="index">バッファインデックス</param>
		/// <returns>D3D12_RESOURCE_STATES を返します。</returns>
		D3D12_RESOURCE_STATES GetBufferState(size_t index) {
			if (m_BufferStates.size() <= index) throw;
			return m_BufferStates[index];
		}
		/// <summary>
		/// 指定したインデックスのバッファ状態を設定する。
		/// </summary>
		/// <param name="index">バッファインデックス</param>
		/// <param name="state">設定するリソースステート</param>
		void SetBufferState(size_t index, D3D12_RESOURCE_STATES state) {
			if (m_BufferStates.size() <= index) throw;
			m_BufferStates[index] = state;

		}
	};
}