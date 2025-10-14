#pragma once
#include "stdafx.h"
namespace RNEngine {

	///----------------------------------------------------------------
	/// Device 関係ヘッダ
	/// 
	/// Device クラス
	/// CommandQueue クラス
	/// SwapChain クラス
	/// RenderTargetView クラス
	/// DescriptorHeap クラス
	///     :
	/// ----------------------------------------------------------------
	class SwapChain {
		ComPtr<IDXGISwapChain4> m_SwapChain;

		void Init(ComPtr<IDXGIFactory6>& _factory, ComPtr<ID3D12CommandQueue> _queue, const Window& _window);
	public:
		SwapChain() {}
		SwapChain(ComPtr<IDXGIFactory6>& _factory, ComPtr<ID3D12CommandQueue> _queue, const Window& _window) { Init(_factory, _queue, _window); }
		~SwapChain() {}

		ComPtr<IDXGISwapChain4> GetPtr() { return m_SwapChain; }
	};
	class CommandQueue {
		ComPtr<ID3D12CommandAllocator> m_CmdAllocator;
		ComPtr<ID3D12GraphicsCommandList> m_CmdList;
		ComPtr<ID3D12CommandQueue> m_CmdQueue;

		void Init(ComPtr<ID3D12Device>& _dev);
	public:
		CommandQueue() {}
		CommandQueue(ComPtr<ID3D12Device>& _dev) { Init(_dev); }
		~CommandQueue() {}


		ComPtr<ID3D12CommandAllocator> GetAllocator() { return m_CmdAllocator; }
		ComPtr<ID3D12GraphicsCommandList> GetList() { return m_CmdList; }
		ComPtr<ID3D12CommandQueue> GetQueue() { return m_CmdQueue; }
	};

	class Fence {
		ComPtr<ID3D12Fence> m_Fence;
		UINT64 m_FenceVal;
		HANDLE m_FenceEvent;
	public:
		Fence() :m_FenceVal(0), m_FenceEvent(0){}
		Fence(ComPtr<ID3D12Device>& _dev) :m_FenceVal(0) { Init(_dev); }
		~Fence() {}

		void Init(ComPtr<ID3D12Device>& _dev);
		void Signal(ComPtr<ID3D12CommandQueue>& _queue);

		ComPtr<ID3D12Fence> GetPtr() { return m_Fence; }
	};

	class Barrier {
		D3D12_RESOURCE_BARRIER m_Barrier;

	public:
		Barrier() noexcept { ZeroMemory(&m_Barrier, sizeof(m_Barrier)); }
		~Barrier() {}

		void Init(ComPtr<ID3D12GraphicsCommandList> _list, ComPtr<ID3D12Resource> _backBuffer);

		void Transition(ComPtr<ID3D12GraphicsCommandList> _list,D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after) {
			m_Barrier.Transition.StateBefore = before;
			m_Barrier.Transition.StateAfter = after;

			_list->ResourceBarrier(1, &m_Barrier);
		}
	};


	/// <summary>
	/// デバイス関係を管理するクラス
	/// </summary>
	class Device {
		ComPtr<ID3D12Device> m_Device;
		ComPtr<IDXGIFactory6> m_Factory;

		SwapChain m_SwapChain;
		CommandQueue m_CommandQueue;
		D3D_FEATURE_LEVEL m_FeatureLevel;

		void InitFeatureLevel();
	public:
		Device() : m_SwapChain(),m_CommandQueue(), m_FeatureLevel(D3D_FEATURE_LEVEL_12_1){}
		~Device() {}

		void Init(const Window& _window);
		void Update();

		ComPtr<ID3D12Device> GetPtr(){ return m_Device; }
		ComPtr<IDXGIFactory6> GetFactory() { return m_Factory; }

		SwapChain& GetSwapChain() { return m_SwapChain; }
		CommandQueue& GetCommandQueue() { return m_CommandQueue; }
	};
}