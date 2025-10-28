#include "stdafx.h"
#include "project.h"

namespace RNEngine {

	RnEngine* RnEngine::g_pInstance = nullptr;

	void RnEngine::EnableDebugLayer() {
		ID3D12Debug* debugLayer = nullptr;
		auto result = D3D12GetDebugInterface(IID_PPV_ARGS(&debugLayer));

		debugLayer->EnableDebugLayer();
		debugLayer->Release();
	}
	void RnEngine::Init() {
		m_Window = make_unique<Window>(L"RNEngine", 1280, 720);

#ifdef _DEBUG
		EnableDebugLayer();
#endif
		m_Device = make_unique<Device>();
		m_Renderer = make_unique<Renderer>();

		m_Device->Init(m_Window);
		m_Renderer->Init(m_Device, m_Window);
		m_Renderer->SetClearColor(0.1f, 0.25f, 0.5f, 1.0f);
		//キー入力の初期化
		Input::GetInstance().Init();

		//SetFrameRate(FRAME_NONDEFINE);
	}
	void RnEngine::Destroy() {
		//m_Renderer->EndRenderer();
		m_Renderer->WaitGPU();
		m_Window->Destroy();
	}

	void RnEngine::Update() {
		MSG msg{};
		while (m_Window->ProcessMessage()) {
			m_Renderer->BeginRenderer();
			// キー入力の更新
			Input::GetInstance().Update();
			// フレームレート制御
			if (!m_FrameTimer.CheckTime(1.0f / m_FrameRate)) continue;

			float deltaTime = m_Timer.CalcDelta();
			cout << "DeltaTime:" << deltaTime << " / " << "FPS: " << 1.0f / deltaTime << endl;

			m_Renderer->EndRenderer();
		}
	}
}
