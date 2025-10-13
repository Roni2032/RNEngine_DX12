#include "stdafx.h"
#include "project.h"

namespace RNEngine {
    void Renderer::Init(Device& _dev,const Window& _window)
    {
		m_CommandList = _dev.GetCommandQueue().GetList();
		m_CommandQueue = _dev.GetCommandQueue().GetQueue();
		m_CommandAllocator = _dev.GetCommandQueue().GetAllocator();

        m_SwapChain = _dev.GetSwapChain().GetSwapChain();
        m_RTVBuffer.Init(_dev.GetDivece(), _dev.GetSwapChain());
        //m_DSVBuffer.Init(_dev.GetDivece(), _window);

		m_Fence = Fence(_dev.GetDivece());

		m_PipelineState = PipelineState();
        //ŠDF‚É‰Šú‰»
		m_ClearColor = { 0.5f,0.5f,0.5f,1.0f };
    }

    void Renderer::BeginRenderer() {
        auto idx = m_SwapChain->GetCurrentBackBufferIndex();

		m_Barrier.Init(m_CommandList, m_RTVBuffer.GetBackBuffer(idx));

        auto rtvH = m_RTVBuffer.GetDecsriptorHeap().GetHeap()->GetCPUDescriptorHandleForHeapStart();
        rtvH.ptr += idx * m_RTVBuffer.GetDecsriptorHeap().GetHeapSize();

        m_CommandList->OMSetRenderTargets(1, &rtvH, true, nullptr);

		m_CommandList->ClearRenderTargetView(rtvH, m_ClearColor.data(), 0, nullptr);
    }
    void Renderer::EndRenderer() {
		m_Barrier.Transition(m_CommandList,D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
        m_CommandList->Close();

		ID3D12CommandList* cmdLists[] = { m_CommandList.Get() };
		m_CommandQueue->ExecuteCommandLists(_countof(cmdLists), cmdLists);
        
		m_Fence.Signal(m_CommandQueue);

		m_CommandAllocator->Reset();
		m_CommandList->Reset(m_CommandAllocator.Get(), nullptr);

        m_SwapChain->Present(1, 0);
	}
}