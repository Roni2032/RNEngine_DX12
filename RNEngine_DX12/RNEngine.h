#pragma once
#include "stdafx.h"
#include "Window.h"
#include "Timer.h"
#include "Device.h"
#include "Renderer.h"
#include "GameObject.h"
#include "Component.h"

namespace RNEngine {

	class RnEngine
	{
	private:
		Window m_Window;		// ウィンドウ
		Renderer m_Renderer;	// 描画処理
		Device m_Device;		// デバイス
		Timer m_Timer;			// タイマー
		Timer m_FrameTimer;		// フレームタイマー

		float m_FrameRate;		// フレームレート

		//仮のデータ配列(後々シーンクラスが持つ)
		vector<GameObject*> m_GameObjects; // ゲームオブジェクトの配列
		vector<Component*> m_Components; // コンポーネントの配列

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

		Device GetDevice() { return m_Device; }
		Renderer GetRenderer() { return m_Renderer; }
		Window GetWindow() { return m_Window; }

		float GetFrameRate() {
			return m_FrameRate;
		}
		void SetFrameRate(float frame) {
			m_FrameRate = frame;
		}
	};
}

