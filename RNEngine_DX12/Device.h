#pragma once
#include "stdafx.h"
namespace RNEngine {
	class Window;
	///----------------------------------------------------------------
	/// Device 関係ヘッダ
	/// 
	/// Device クラス
	/// CommandContext クラス
	/// SwapChain クラス
	/// RenderTargetView クラス
	/// DescriptorHeap クラス
	///     :
	/// ----------------------------------------------------------------
	class SwapChain {
		ComPtr<IDXGISwapChain4> m_SwapChain;

		void Init(IDXGIFactory6* _factory, ID3D12CommandQueue* _queue, const Window* _window);
	public:
        /// <summary>
		/// デフォルトコンストラクタ
		/// </summary>
		SwapChain();
		/// <summary>
		/// 引数付きコンストラクタ（初期化を行う）
		/// </summary>
		SwapChain(IDXGIFactory6* _factory, ID3D12CommandQueue* _queue, const Window* _window);
		/// <summary>
		/// デストラクタ
		/// </summary>
		~SwapChain();

		/// <summary>
		/// 内部の SwapChain COM オブジェクトのスマートポインタを取得する。
		/// </summary>
		/// <returns>ComPtr&lt;IDXGISwapChain4&gt; を返します。</returns>
		ComPtr<IDXGISwapChain4> GetPtr() { return m_SwapChain; }
	};
	class CommandContext {
		ComPtr<ID3D12CommandAllocator> m_CmdAllocator;
		ComPtr<ID3D12GraphicsCommandList> m_CmdList;
		ComPtr<ID3D12CommandQueue> m_CmdQueue;

		void Init(ID3D12Device* _dev);
	public:
		CommandContext();
		CommandContext(ID3D12Device* _dev);
		~CommandContext();


		ID3D12CommandAllocator* GetAllocator() { return m_CmdAllocator.Get(); }
		ID3D12GraphicsCommandList* GetList() { return m_CmdList.Get(); }
		ID3D12CommandQueue* GetQueue() { return m_CmdQueue.Get(); }
	};

	class Fence {
		ComPtr<ID3D12Fence> m_Fence;
		UINT64 m_FenceVal;
		HANDLE m_FenceEvent;
	public:
        /// <summary>
		/// デフォルトコンストラクタ
		/// </summary>
		Fence();
		/// <summary>
		/// 初期化付きコンストラクタ
		/// </summary>
		/// <param name="_dev">D3D12 デバイス</param>
		Fence(ID3D12Device* _dev);
		/// <summary>
		/// デストラクタ
		/// </summary>
		~Fence();

		/// <summary>
		/// フェンスを初期化する。
		/// </summary>
		/// <param name="_dev">D3D12 デバイス</param>
		void Init(ID3D12Device* _dev);
		/// <summary>
		/// GPU の完了を待機するユーティリティ。
		/// </summary>
		/// <param name="_queue">待機対象のコマンドキュー</param>
		void WaitGPU(ID3D12CommandQueue* _queue);

		/// <summary>
		/// 内部のフェンスの生ポインタを取得する。
		/// </summary>
		/// <returns>ID3D12Fence の生ポインタを返します。</returns>
		ID3D12Fence* GetPtr() { return m_Fence.Get(); }
	};

	class Barrier {
		D3D12_RESOURCE_BARRIER m_Barrier;

	public:
		Barrier() noexcept;
		~Barrier();

      /// <summary>
		/// リソースバリア（状態遷移）をコマンドリストに発行する。
		/// </summary>
		/// <param name="_list">使用するコマンドリスト</param>
		/// <param name="_backBuffer">状態遷移するリソース</param>
		/// <param name="before">遷移前のリソースステート</param>
		/// <param name="after">遷移後のリソースステート</param>
		void Transition(ID3D12GraphicsCommandList* _list, ID3D12Resource* _backBuffer, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after);
	};


	/// <summary>
	/// デバイス関係を管理するクラス
	/// </summary>
	class Device {
		ComPtr<ID3D12Device> m_Device;
		ComPtr<IDXGIFactory6> m_Factory;

		unique_ptr<SwapChain> m_SwapChain;
		unique_ptr<CommandContext> m_CommandContext;
		D3D_FEATURE_LEVEL m_FeatureLevel;

		void InitFeatureLevel();
	public:
       /// <summary>
		/// デバイスマネージャクラスのデフォルトコンストラクタ
		/// </summary>
		Device();
		/// <summary>
		/// デバイスマネージクラスのデストラクタ
		/// </summary>
		~Device();

		/// <summary>
		/// デバイスを初期化する。
		/// </summary>
		/// <param name="_window">レンダリング対象ウィンドウの情報</param>
		void Init(const Window* _window);

		/// <summary>
		/// 定期的な更新処理
		/// </summary>
		void Update();

		/// <summary>
		/// 内部の D3D12 デバイスの生ポインタを取得する。
		/// </summary>
		/// <returns>ID3D12Device の生ポインタを返します。</returns>
		ID3D12Device* GetPtr()const{ return m_Device.Get(); }
		/// <summary>
		/// 使用している DXGI ファクトリを取得する。
		/// </summary>
		/// <returns>IDXGIFactory6 の生ポインタを返します。</returns>
		IDXGIFactory6* GetFactory() const{ return m_Factory.Get(); }

		/// <summary>
		/// 内部で管理している SwapChain オブジェクトを取得する。
		/// </summary>
		/// <returns>SwapChain の生ポインタを返します（所有権は移動しません）。</returns>
		SwapChain* GetSwapChain() { return m_SwapChain.get(); }
		/// <summary>
		/// 内部のコマンドコンテキストを取得する。
		/// </summary>
		/// <returns>CommandContext の生ポインタを返します。</returns>
		CommandContext* GetCommandContext() { return m_CommandContext.get(); }
	};
}