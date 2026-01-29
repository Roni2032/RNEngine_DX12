#include "DSV.h"
#include "Descriptor.h"
#include "Window.h"

namespace RNEngine {
	DSVBuffer::DSVBuffer() {}
	DSVBuffer::DSVBuffer(ID3D12Device* _dev, const Window* _window) {
		Init(_dev, _window);
	}
	DSVBuffer::~DSVBuffer() {}

	DescriptorHeap* DSVBuffer::GetDescriptorHeap() {
		return m_DSVHeap.get();
	}

	void DSVBuffer::CreateDSVDesc(ID3D12Device* _dev) {
		ZeroMemory(&m_Desc, sizeof(m_Desc));
		m_Desc.Format = DXGI_FORMAT_D32_FLOAT;
		m_Desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		m_Desc.Flags = D3D12_DSV_FLAG_NONE;

		_dev->CreateDepthStencilView(
			m_Buffer.Get(), // ComPtrが管理しているデータを渡す
			&m_Desc,
			m_DSVHeap->GetCPUHandle());
	}

	void DSVBuffer::Init(ID3D12Device* _dev, const Window* _window) {

		D3D12_RESOURCE_DESC resourceDesc = {};
		resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D; // 二次元配列的なバッファ
		resourceDesc.Width = _window->GetWidth(); // 画面の「幅」に合わせる
		resourceDesc.Height = _window->GetHeight(); // 画面の「高さ」に合わせる
		resourceDesc.DepthOrArraySize = 1;
		resourceDesc.Format = DXGI_FORMAT_R32_TYPELESS; // ステンシル無し
		resourceDesc.SampleDesc.Count = 1;
		resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

		// 深度値用ヒーププロパティ
		auto heap = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

		// クリアバリューを設定する
		D3D12_CLEAR_VALUE clearValue = {};
		clearValue.DepthStencil.Depth = 1.0f; // 深度の最大値
		clearValue.Format = DXGI_FORMAT_D32_FLOAT;

		auto result = _dev->CreateCommittedResource(
			&heap,
			D3D12_HEAP_FLAG_NONE,
			&resourceDesc,
			D3D12_RESOURCE_STATE_DEPTH_WRITE,
			&clearValue,
			IID_PPV_ARGS(m_Buffer.GetAddressOf()));

		printf("Create RT: size=%ux%u format=%u flags=%u\n",
			_window->GetWidth(), _window->GetHeight(), (unsigned)resourceDesc.Format, (unsigned)resourceDesc.Flags);

		if (FAILED(result)) {
			printf("CreateCommittedResource failed hr=0x%08X\n", (unsigned)result);
			HRESULT reason = _dev->GetDeviceRemovedReason();
			printf("GetDeviceRemovedReason = 0x%08X\n", (unsigned)reason);
		}

		m_DSVHeap = make_unique<DescriptorHeap>();
		m_DSVHeap->Init(_dev, 1, D3D12_DESCRIPTOR_HEAP_TYPE_DSV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE);

		CreateDSVDesc(_dev);
	}
}