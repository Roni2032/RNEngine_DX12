#include "stdafx.h"
#include "project.h"

namespace RNEngine {
    void Renderer::Init(Device& _dev,const Window& _window)
    {
        m_SwapChain = _dev.GetSwapChain().GetSwapChain();
        m_RTVBuffer.Init(_dev.GetDivece(), _dev.GetSwapChain());
        //m_DSVBuffer.Init(_dev.GetDivece(), _window);
    }

    void Renderer::BeginRenderer() {
        auto idx = m_SwapChain->GetCurrentBackBufferIndex();

        auto rtvH = m_RTVBuffer.GetDecsriptorHeap().GetHeap()->GetCPUDescriptorHandleForHeapStart();
        rtvH.ptr += idx * m_RTVBuffer.GetDecsriptorHeap().GetHeapSize();

        m_CommandList->OMSetRenderTargets(1, &rtvH, true, nullptr);
    }
}