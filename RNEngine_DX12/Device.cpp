#include "stdafx.h"
#include "project.h"

namespace RNEngine {

	void Device::Init(const Window& _window) {
		InitFeatureLevel();
		m_CommandQueue = CommandQueue(m_Device);
		m_SwapChain = SwapChain(m_Factory,m_CommandQueue.GetQueue(),_window);
	}

	void Device::InitFeatureLevel() {
		D3D_FEATURE_LEVEL levels[] = {
			D3D_FEATURE_LEVEL_12_1,
			D3D_FEATURE_LEVEL_12_0,
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
		};

		D3D_FEATURE_LEVEL featureLevel;
		ComPtr<IDXGIAdapter> adapter;
		HRESULT result;

		result = CreateDXGIFactory1(IID_PPV_ARGS(m_Factory.GetAddressOf()));
		assert(SUCCEEDED(result));

		// 高性能GPUを優先的に使用する
		for (int i = 0; SUCCEEDED(m_Factory->EnumAdapterByGpuPreference(
			i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(adapter.ReleaseAndGetAddressOf()))); i++)
		{
			for (auto lv : levels)
			{
				result = D3D12CreateDevice(adapter.Get(), lv, IID_PPV_ARGS(m_Device.ReleaseAndGetAddressOf()));
				if (result == S_OK)
				{
					// デバイスの生成に成功
					featureLevel = lv;
					break;
				}
			}
			if (SUCCEEDED(result))
			{
				break;
			}
		}
	}
	void CommandQueue::Init(ComPtr<ID3D12Device>& _dev) {
		auto result = _dev->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(m_CmdAllocator.GetAddressOf()));
		assert(SUCCEEDED(result));

		result = _dev->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_CmdAllocator.Get(), nullptr, IID_PPV_ARGS(m_CmdList.GetAddressOf()));
		assert(SUCCEEDED(result));

		D3D12_COMMAND_QUEUE_DESC queueDesc = {};
		queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		queueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
		queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		queueDesc.NodeMask = 0;

		result = _dev->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(m_CmdQueue.GetAddressOf()));
	}

	void SwapChain::Init(ComPtr<IDXGIFactory6>& _factory, ComPtr<ID3D12CommandQueue> _queue, const Window& _window) {
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};

		swapChainDesc.Width = _window.GetWidth();
		swapChainDesc.Height = _window.GetHeight();
		swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.Stereo = false;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
		swapChainDesc.BufferCount = 2;

		swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

		swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
		swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		auto result = _factory->CreateSwapChainForHwnd(
			_queue.Get(),
			_window.GetHwnd(),
			&swapChainDesc,
			nullptr,
			nullptr,
			(IDXGISwapChain1**)m_SwapChain.GetAddressOf()
		);

		assert(SUCCEEDED(result));
	}
}