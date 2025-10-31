#pragma once
#include "stdafx.h"
#include "Window.h"
#include "Timer.h"

namespace RNEngine {
#define FRAME_NONDEFINE 10000
	class Renderer;
	class Device;
	class FrameTimer;

	class RnEngine
	{
	private:
		unique_ptr<Window> m_Window;		// ウィンドウ
		unique_ptr <Renderer> m_Renderer;	// 描画処理
		unique_ptr<Device> m_Device;		// デバイス
		FrameTimer m_FrameTimer;		// フレームタイマー

		float m_FrameRate;		// フレームレート

		//仮のデータ配列(後々シーンクラスが持つ)
		//vector<GameObject*> m_GameObjects; // ゲームオブジェクトの配列
		//vector<Component*> m_Components; // コンポーネントの配列

		void EnableDebugLayer();
	public:

		static RnEngine* g_pInstance;

		RnEngine() : m_Window(),m_FrameRate(120.0f) { g_pInstance = this; }
		~RnEngine() {}

		/// <summary>
		/// 初期化
		/// </summary>
		void Init();

		/// <summary>
		/// 破棄
		/// </summary>
		void Destroy();

		/// <summary>
		/// 更新
		/// </summary>
		void Update();

		unique_ptr <Device>& GetDevice() { return m_Device; }
		unique_ptr <Renderer>& GetRenderer() { return m_Renderer; }
		unique_ptr<Window>& GetWindow() { return m_Window; }

		float GetFrameRate() {
			return m_FrameRate;
		}
		void SetFrameRate(float frame) {
			m_FrameRate = frame;
		}
	};
}

