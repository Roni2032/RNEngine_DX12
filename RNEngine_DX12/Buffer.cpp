#include "stdafx.h"
#include "project.h"

namespace RNEngine {

	bool DescriptorHeap::Init(ComPtr<ID3D12Device>& _dev, UINT _frameBufferCount, D3D12_DESCRIPTOR_HEAP_TYPE _type) {
		D3D12_DESCRIPTOR_HEAP_DESC desc = {};
		desc.NumDescriptors = _frameBufferCount;
		desc.NodeMask = 0;
		desc.Type = _type;
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		auto hr = _dev->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_Heap));
		if (FAILED(hr)) {
			//RTV�p�̃f�B�X�N���v�^�q�[�v�̍쐬�Ɏ��s�����B
			return false;
		}
		//�f�B�X�N���v�^�̃T�C�Y���擾�B
		m_HeapSize = _dev->GetDescriptorHandleIncrementSize(_type);
	}

	void RTVBuffer::Init(ComPtr<ID3D12Device>& _dev,SwapChain& _swapChian) {
		UINT frameBufferCount = 2;

		m_RTVHeap = DescriptorHeap();
		m_RTVHeap.Init(_dev, frameBufferCount, D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_RTVHeap.GetHeap()->GetCPUDescriptorHandleForHeapStart();

		//�t�����g�o�b�t�@���o�b�N�o�b�t�@�p��RTV���쐬�B
		for (UINT n = 0; n < frameBufferCount; n++) {
			_swapChian.GetSwapChain()->GetBuffer(n, IID_PPV_ARGS(&m_BackBuffer[n]));
			_dev->CreateRenderTargetView(
				m_BackBuffer[n].Get(), nullptr, rtvHandle
			);
			rtvHandle.ptr += m_RTVHeap.GetHeapSize();
		}
	}

	void DSVBuffer::Init(ComPtr<ID3D12Device>& _dev, const Window& _window) {
		m_DSVHeap = DescriptorHeap();
		m_DSVHeap.Init(_dev, 1, D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	}
}
