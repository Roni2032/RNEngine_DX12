#pragma once
#include "stdafx.h"
#include "Device.h"
#include "Window.h"
#include "Renderer.h"
#include "Timer.h"
#include "RendererComponent.h"
#include "Input.h"
namespace RNEngine {
#define FRAME_NONDEFINE 10000
	class Renderer;
	class Device;
	class FrameTimer;

	class Engine
	{
	private:
		unique_ptr<Window> m_Window;		// ウィンドウ
		unique_ptr<Renderer> m_Renderer;	// 描画処理
		unique_ptr<Device> m_Device;		// デバイス
		FrameTimer m_FrameTimer;			// フレームタイマー

		float m_FrameRate;					// フレームレート

		//テスト用モデル作成
		vector<shared_ptr<RendererComponent>> m_Renderers;
		XMFLOAT3 position = { 0,0,0 };

		void EnableDebugLayer();

		static Engine* g_pInstance;
	public:
		Engine() : m_Window(),m_FrameRate(120.0f) { g_pInstance = this; }
		~Engine() {}

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

		static Device* GetDevice() { return g_pInstance->m_Device.get(); }
		static ID3D12Device* GetID3D12Device() { return g_pInstance->m_Device->GetPtr(); }
		static Renderer* GetRenderer() { return g_pInstance->m_Renderer.get(); }
		static Window* GetWindow() { return g_pInstance->m_Window.get(); }

		float GetFrameRate() {
			return m_FrameRate;
		}
		void SetFrameRate(float frame) {
			m_FrameRate = frame;
		}

		void OnMove(InputActionContext& context) {
			position.x -= 0.01f;
		}
	};
}

