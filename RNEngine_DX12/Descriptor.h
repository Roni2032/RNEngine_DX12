#pragma once
#include "stdafx.h"

namespace RNEngine {
	class DescriptorHeap {
		ComPtr<ID3D12DescriptorHeap> m_Heap;
		UINT m_HeapSize;
		UINT m_HeapCount; //次に追加するヒープ番号(-1した分だけ登録済み)
	public:
       /// <summary>
		/// コンストラクタ
		/// </summary>
		DescriptorHeap();
      /// <summary>
		/// デストラクタ
		/// </summary>
		~DescriptorHeap();
       /// <summary>
		/// 内部カウンタをリセットする。
		/// </summary>
		void Reset() { m_HeapCount = 0; }

        /// <summary>
		/// ディスクリプタヒープを初期化する。
		/// </summary>
		/// <param name="_dev">D3D12 デバイス</param>
		/// <param name="_frameBufferCount">フレームバッファ数</param>
		/// <param name="_type">ヒープのタイプ（SRV/CBV/RTV など）</param>
		/// <param name="_flags">ヒープフラグ</param>
		/// <returns>初期化に成功したら true を返します。</returns>
		bool Init(ID3D12Device* _dev, UINT _frameBufferCount, D3D12_DESCRIPTOR_HEAP_TYPE _type, D3D12_DESCRIPTOR_HEAP_FLAGS _flags);
       /// <summary>
		/// 内部のディスクリプタヒープのポインタを取得する。
		/// </summary>
		/// <returns>ID3D12DescriptorHeap の生ポインタを返します。</returns>
		ID3D12DescriptorHeap* GetHeap()const { return m_Heap.Get(); }
       /// <summary>
		/// 内部のディスクリプタヒープのアドレスを取得する。
		/// </summary>
		/// <returns>ID3D12DescriptorHeap** を返します。</returns>
		ID3D12DescriptorHeap** GetHeapAddress() { return m_Heap.GetAddressOf(); }
      /// <summary>
		/// ヒープサイズを取得する。
		/// </summary>
		/// <returns>ヒープに割り当てられたサイズを返します。</returns>
		UINT GetHeapSize()const { return m_HeapSize; }
        /// <summary>
		/// ヒープの先頭の GPU ハンドルを取得する。
		/// </summary>
		/// <returns>GPU 用のディスクリプタハンドルを返します。</returns>
		D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle()const { return m_Heap->GetGPUDescriptorHandleForHeapStart(); }
        /// <summary>
		/// ヒープの先頭の CPU ハンドルを取得する。
		/// </summary>
		/// <returns>CPU 用のディスクリプタハンドルを返します。</returns>
		D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle()const { return m_Heap->GetCPUDescriptorHandleForHeapStart(); }

      /// <summary>
		/// ヒープ使用カウンタをインクリメントする。
		/// </summary>
		void AddHeapCount() { m_HeapCount++; }
     /// <summary>
		/// 現在のヒープ使用カウンタを取得する。
		/// </summary>
		/// <returns>使用済みヒープ数を返します。</returns>
		UINT GetHeapCount() { return m_HeapCount; }
	};
}