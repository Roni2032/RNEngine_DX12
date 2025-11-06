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
		SwapChain() {}
		SwapChain(IDXGIFactory6* _factory, ID3D12CommandQueue* _queue, const Window* _window) { Init(_factory, _queue, _window); }
		~SwapChain() {}

		ComPtr<IDXGISwapChain4> GetPtr() { return m_SwapChain; }
	};
	class CommandContext {
		ComPtr<ID3D12CommandAllocator> m_CmdAllocator;
		ComPtr<ID3D12GraphicsCommandList> m_CmdList;
		ComPtr<ID3D12CommandQueue> m_CmdQueue;

		void Init(ID3D12Device* _dev);
	public:
		CommandContext() {}
		CommandContext(ID3D12Device* _dev) { Init(_dev); }
		~CommandContext() {}


		ID3D12CommandAllocator* GetAllocator() { return m_CmdAllocator.Get(); }
		ID3D12GraphicsCommandList* GetList() { return m_CmdList.Get(); }
		ID3D12CommandQueue* GetQueue() { return m_CmdQueue.Get(); }
	};

	class Fence {
		ComPtr<ID3D12Fence> m_Fence;
		UINT64 m_FenceVal;
		HANDLE m_FenceEvent;
	public:
		Fence() :m_FenceVal(0), m_FenceEvent(0){}
		Fence(ID3D12Device* _dev) :m_FenceVal(0) { Init(_dev); }
		~Fence() {}

		void Init(ID3D12Device* _dev);
		void WaitGPU(ID3D12CommandQueue* _queue);

		ComPtr<ID3D12Fence> GetPtr() { return m_Fence; }
	};

	class Barrier {
		D3D12_RESOURCE_BARRIER m_Barrier;

	public:
		Barrier() noexcept { ZeroMemory(&m_Barrier, sizeof(m_Barrier)); }
		~Barrier() {}

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
		Device() : m_SwapChain(),m_CommandContext(), m_FeatureLevel(D3D_FEATURE_LEVEL_12_1){}
		~Device() {}

		void Init(const Window* _window);

		void Update();

		ID3D12Device* GetPtr()const{ return m_Device.Get(); }
		IDXGIFactory6* GetFactory() const{ return m_Factory.Get(); }

		unique_ptr<SwapChain>& GetSwapChain() { return m_SwapChain; }
		unique_ptr<CommandContext>& GetCommandContext() { return m_CommandContext; }
	};
}