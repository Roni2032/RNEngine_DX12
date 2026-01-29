#include "RTV.h"
#include "Descriptor.h"
#include "Device.h"

namespace RNEngine {
	RTVBuffer::RTVBuffer() {}
	RTVBuffer::RTVBuffer(ID3D12Device* _dev, SwapChain* _swapChain) {
		InitFrameBuffer(_dev, _swapChain);
	}
	RTVBuffer::~RTVBuffer() {}

	void RTVBuffer::CreateRTV(ID3D12Device* _dev, UINT count) {
		m_RTVHeap = make_unique<DescriptorHeap>();
		m_RTVHeap->Init(_dev, count, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE);

		::ZeroMemory(&m_RTVDesc, sizeof(m_RTVDesc));
		m_RTVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		m_RTVDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	}
	void RTVBuffer::Init(ID3D12Device* _dev) {
		CreateRTV(_dev, 1);
	}
	void RTVBuffer::InitFrameBuffer(ID3D12Device* _dev, SwapChain* _swapChian) {
		UINT frameBufferCount = 2;

		CreateRTV(_dev, frameBufferCount);

		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_RTVHeap->GetCPUHandle();

		DXGI_SWAP_CHAIN_DESC swapDesc;
		auto result = _swapChian->GetPtr()->GetDesc(&swapDesc);
		assert(SUCCEEDED(result));

		m_BackBuffer.resize(swapDesc.BufferCount);
		m_BufferStates.resize(swapDesc.BufferCount);
		//フロントバッファをバックバッファ用のRTVを作成。
		for (UINT n = 0; n < frameBufferCount; n++) {
			_swapChian->GetPtr()->GetBuffer(n, IID_PPV_ARGS(&m_BackBuffer[n]));
			_dev->CreateRenderTargetView(
				m_BackBuffer[n].Get(), &m_RTVDesc, rtvHandle
			);
			m_BufferStates[n] = D3D12_RESOURCE_STATE_PRESENT;
			rtvHandle.ptr += m_RTVHeap->GetHeapSize();
		}
	}

	DescriptorHeap* RTVBuffer::GetDescriptorHeap() { return m_RTVHeap.get(); }
}