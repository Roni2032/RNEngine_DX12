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
		m_Window = Window(L"RNEngine", 1280, 720);

#ifdef _DEBUG
		EnableDebugLayer();
#endif

		m_Device.Init(m_Window);
		m_Renderer.Init(m_Device, m_Window);
		m_Renderer.SetClearColor(0.1f, 0.25f, 0.5f, 1.0f);
		//キー入力の初期化
		Input::GetInstance().Init();
	}
	void RnEngine::Destroy() {
		m_Window.Destroy();
	}

	void RnEngine::Update() {
		MSG msg{};
		while (m_Window.ProcessMessage()) {
			m_Renderer.BeginRenderer();
			// キー入力の更新
			Input::GetInstance().Update();
			// フレームレート制御
			if (!m_FrameTimer.CheckTime(1.0f / m_FrameRate)) continue;

			m_Timer.CalcDelta();
			float deltaTime = m_Timer.GetDeltaTime();
			cout << "DeltaTime:" << deltaTime << " / " << "FPS: " << 1.0f / deltaTime << endl;

			/*for(auto it = m_GameObjects.begin(); it != m_GameObjects.end(); ++it){
				(*it)->Update(deltaTime);
			}
			for (auto it = m_GameObjects.begin(); it != m_GameObjects.end(); ++it) {
				(*it)->LateUpdate(deltaTime);
			}
			for (auto it = m_Components.begin(); it != m_Components.end(); ++it) {
				(*it)->Update(deltaTime);
			}
			for(auto it = m_Components.begin(); it != m_Components.end(); ++it){
				(*it)->LateUpdate(deltaTime);
			}*/
			m_Renderer.EndRenderer();
		}
		Destroy();
	}
}
