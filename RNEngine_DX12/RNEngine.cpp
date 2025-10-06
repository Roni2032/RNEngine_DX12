#include "stdafx.h"
#include "project.h"

namespace RNEngine {

	RnEngine* RnEngine::g_pInstance = nullptr;

	void RnEngine::Init() {
		m_Window = Window(L"RNEngine", 1280, 720);

		m_Device.Init(m_Window);

		//キー入力の初期化
		Input::GetInstance().Init();
	}
	void RnEngine::Destroy() {
		m_Window.Destroy();
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
			// キー入力の更新
			Input::GetInstance().Update();
			// フレームレート制御
			if (!m_FrameTimer.CheckTime(1.0f / m_FrameRate)) continue;

			m_Timer.CalcDelta();
			cout << "DeltaTime:" << m_Timer.GetDeltaTime() << " / " << "FPS: " << 1.0f / m_Timer.GetDeltaTime() << endl;
		}
		Destroy();
	}
}
