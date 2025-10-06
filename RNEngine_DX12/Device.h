#pragma once
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
	};
	class CommandQueue {
		ComPtr<ID3D12CommandAllocator> m_CmdAllocator;
		ComPtr<ID3D12CommandList> m_CmdList;
		ComPtr<ID3D12CommandQueue> m_CmdQueue;

		void Init(ComPtr<ID3D12Device>& _dev);
	public:
		CommandQueue() {}
		CommandQueue(ComPtr<ID3D12Device>& _dev) { Init(_dev); }
		~CommandQueue() {}


		ComPtr<ID3D12CommandAllocator> GetAllocator()const { return m_CmdAllocator; }
		ComPtr<ID3D12CommandList> GetList()const { return m_CmdList; }
		ComPtr<ID3D12CommandQueue> GetQueue()const { return m_CmdQueue; }
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

		ComPtr<ID3D12Device> GetDivece()const { return m_Device; }
		ComPtr<IDXGIFactory6> GetFactory()const { return m_Factory; }

		SwapChain GetSwapChain()const { return m_SwapChain; }
		CommandQueue GetCommandQueue()const { return m_CommandQueue; }
	};
}