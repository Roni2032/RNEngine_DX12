#include "stdafx.h"
#include "project.h"

namespace RNEngine {

	void Device::Init(const unique_ptr<Window>& _window) {
		InitFeatureLevel();
		m_CommandContext = make_unique<CommandContext>(m_Device);
		m_SwapChain = make_unique<SwapChain>(m_Factory,m_CommandContext->GetQueue(),_window);
	}
	void Device::Update() {

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
		///DXGI�n���̃G���[���o��悤��ifdef����
#ifdef _DEBUG
		result = CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(m_Factory.GetAddressOf()));
#else
		result = CreateDXGIFactory1(IID_PPV_ARGS(m_Factory.GetAddressOf()));
#endif
		assert(SUCCEEDED(result));

		// �����\GPU��D��I�Ɏg�p����
		for (int i = 0; SUCCEEDED(m_Factory->EnumAdapterByGpuPreference(
			i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(adapter.ReleaseAndGetAddressOf()))); i++)
		{
			for (auto lv : levels)
			{
				result = D3D12CreateDevice(adapter.Get(), lv, IID_PPV_ARGS(m_Device.ReleaseAndGetAddressOf()));
				if (result == S_OK)
				{
					// �f�o�C�X�̐����ɐ���
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
	void CommandContext::Init(ComPtr<ID3D12Device>& _dev) {
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
	void Fence::Init(ComPtr<ID3D12Device>& _dev) {
		m_FenceVal = 0;
		auto result = _dev->CreateFence(m_FenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(m_Fence.GetAddressOf()));
	}
	void Fence::WaitGPU(ComPtr<ID3D12CommandQueue>& _queue) {
		_queue->Signal(m_Fence.Get(), ++m_FenceVal);
		// GPU���������I����܂őҋ@
		if(m_Fence->GetCompletedValue() != m_FenceVal){
			m_FenceEvent = CreateEvent(nullptr, false, false, nullptr);
			m_Fence->SetEventOnCompletion(m_FenceVal, m_FenceEvent);
			WaitForSingleObject(m_FenceEvent, INFINITE);
			CloseHandle(m_FenceEvent);
		}
	}
	void Barrier::Transition(ComPtr<ID3D12GraphicsCommandList> _list,ComPtr<ID3D12Resource> _backBuffer, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after) {
		m_Barrier = CD3DX12_RESOURCE_BARRIER::Transition(_backBuffer.Get(), before, after);

		_list->ResourceBarrier(1, &m_Barrier);
	}
	void SwapChain::Init(ComPtr<IDXGIFactory6>& _factory, ComPtr<ID3D12CommandQueue> _queue, const unique_ptr<Window>& _window) {
		m_SwapChain.Reset();

		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};

		swapChainDesc.Width = _window->GetWidth();
		swapChainDesc.Height = _window->GetHeight();
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
			_window->GetHwnd(),
			&swapChainDesc,
			nullptr,
			nullptr,
			(IDXGISwapChain1**)m_SwapChain.GetAddressOf()
		);

		assert(SUCCEEDED(result));
	}
}