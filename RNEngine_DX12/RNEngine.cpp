#include "stdafx.h"
#include "RNEngine.h"

namespace RNEngine {

	RnEngine* RnEngine::g_pInstance = nullptr;

	void RnEngine::InitFeatureLevel() {
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

	void RnEngine::CreateCommandList() {
		auto result = m_Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(m_CmdAllocator.GetAddressOf()));
		assert(SUCCEEDED(result));

		result = m_Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_CmdAllocator.Get(), nullptr, IID_PPV_ARGS(m_CmdList.GetAddressOf()));
		assert(SUCCEEDED(result));

		D3D12_COMMAND_QUEUE_DESC queueDesc = {};
		queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		queueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
		queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		queueDesc.NodeMask = 0;

		result = m_Device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(m_CmdQueue.GetAddressOf()));
	}
	void RnEngine::Init() {
		m_Window = Window(TEXT("RNEngine"), 1280, 720);

		InitFeatureLevel();
		CreateCommandList();
	}
	void RnEngine::Destroy() {
		//UnregisterClass()
	}

	void RnEngine::Update() {
		MSG msg{};

		while (true) {
			if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			if(msg.message == WM_QUIT) {
				break;
			}
		}
		Destroy();
	}
}
