#pragma once
#include "Window.h"

namespace RNEngine {

	class RnEngine
	{
	private:
		Window m_Window;		// �E�B���h�E
		Renderer m_Renderer;	// �`�揈��
		Device m_Device;	// �f�o�C�X

		ComPtr<ID3D12Device> m_Device;
		ComPtr<IDXGIFactory6> m_Factory;
		ComPtr<IDXGISwapChain4> m_SwapChain;
		ComPtr<ID3D12CommandAllocator> m_CmdAllocator;
		ComPtr<ID3D12CommandList> m_CmdList;
		ComPtr<ID3D12CommandQueue> m_CmdQueue;

		void InitFeatureLevel();
		void CreateCommandList();
		void CreateSwapChain();
	public:

		static RnEngine* g_pInstance;

		RnEngine() : m_Window() { g_pInstance = this; }
		~RnEngine() { delete g_pInstance; }

		/// <summary>
		/// ������
		/// </summary>
		void Init();

		/// <summary>
		/// �j��
		/// </summary>
		void Destroy();

		/// <summary>
		/// �X�V
		/// </summary>
		void Update();

		ComPtr<ID3D12Device> GetDevice() { return m_Device; }
		ComPtr<IDXGIFactory6> GetFactory() { return m_Factory; }
		ComPtr<IDXGISwapChain4> GetSwapChain() { return m_SwapChain; }
	};
}

