#include "RenderTarget.h"
#include "RNEngine.h"
#include "Renderer.h"

#include "TextureBuffer.h"
#include "TextureResource.h"

#include "Descriptor.h"
#include "RTV.h"
#include "DSV.h"

#include "RendererComponent.h"
namespace RNEngine {
	RenderTarget::RenderTarget() :m_ClearColor{ 1,1,1,1 }, m_Width(0), m_Height(0) {}
	RenderTarget::~RenderTarget() = default;

	void RenderTarget::Create(Vector2 renderSize, DXGI_FORMAT format, array<float, 4> clearColor) {

		m_Width = renderSize.x;
		m_Height = renderSize.y;
		m_ClearColor = clearColor;

		auto dev = Engine::GetID3D12Device();
		m_Rtv = make_unique<RTVBuffer>();
		m_Rtv->Init(dev);

		m_Dsv = make_unique<DSVBuffer>();
		auto window = Engine::GetWindow();
		m_Dsv->Init(dev, window);

		shared_ptr<TextureResource> tex;
		auto textureBuffer = make_shared<TextureBuffer>();
		textureBuffer->Create(dev, (UINT)renderSize.x, (UINT)renderSize.y, format, m_ClearColor);

		dev->CreateRenderTargetView(textureBuffer->GetBuffer(), &m_Rtv->m_RTVDesc, m_Rtv->GetDescriptorHeap()->GetCPUHandle());

		m_RenderTargetTexture = make_shared<TextureResource>();
		m_RenderTargetTexture->SetTexture(textureBuffer);
		m_RenderTargetTexture->UpdateWorldMatrix(Vector3(m_Width / 2.0f, m_Height / 2.0f, 0.0f), Vector3(m_Width, m_Height, 1.0f), Quaternion::Identity);

		auto res = textureBuffer->GetBuffer();
		if (!res) { printf("ERROR: resource null\n"); }

		auto desc = res->GetDesc();
		printf("RESOURCE: Format=%u Flags=%u Width=%u Height=%u Sample=%u Mip=%u\n",
			(unsigned)desc.Format, (unsigned)desc.Flags, (unsigned)desc.Width, (unsigned)desc.Height,
			(unsigned)desc.SampleDesc.Count, (unsigned)desc.MipLevels);

		D3D12_RENDER_TARGET_VIEW_DESC rtv = m_Rtv->m_RTVDesc; // or log fields individually
		printf("RTVDesc: Format=%u ViewDim=%u\n", (unsigned)rtv.Format, (unsigned)rtv.ViewDimension);

		auto heap = m_Rtv->GetDescriptorHeap();
		printf("Heap: type? (not accessible here) Count or pointer: %p  CPU.ptr=%llu\n",
			heap, (unsigned long long)heap->GetCPUHandle().ptr);
	}

	void RenderTarget::DrawBegin(ID3D12GraphicsCommandList* cmdList) {
		auto textureBuffer = m_RenderTargetTexture->GetTexture();
		auto barrier = Barrier();

		barrier.Transition(cmdList, textureBuffer->GetBuffer(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);
		auto rtvH = m_Rtv->GetDescriptorHeap()->GetCPUHandle();
		auto dsvH = m_Dsv->GetDescriptorHeap()->GetCPUHandle();
		cmdList->OMSetRenderTargets(1, &rtvH, true, &dsvH);

		cmdList->ClearRenderTargetView(rtvH, m_ClearColor.data(), 0, nullptr);
		cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	}
	void RenderTarget::DrawEnd(ID3D12GraphicsCommandList* cmdList) {
		auto textureBuffer = m_RenderTargetTexture->GetTexture();
		auto barrier = Barrier();
		barrier.Transition(cmdList, textureBuffer->GetBuffer(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	}
	void RenderTarget::Draw(vector<shared_ptr<RendererComponent>>& renderers) {
		auto renderer = Engine::GetRenderer();
		auto cmdList = Engine::GetRenderer()->GetCommandList();

		DrawBegin(cmdList);
		auto srvHeap = renderer->GetSrvDescriptorHeap();
		for (auto& rendererComp : renderers) {
			rendererComp->Draw(cmdList, srvHeap);
		}
		DrawEnd(cmdList);
	}

	void RenderTarget::SetClearColor(const Color& color) {
		m_ClearColor = { color.x,color.y,color.z,color.w };
	}
	shared_ptr<TextureResource> RenderTarget::GetRenderTargetTexture() { return m_RenderTargetTexture; }
}