#pragma once
#include "stdafx.h"
#include "Input.h"

namespace RNEngine {
#define FRAME_NONDEFINE 10000

	class FrameTimer;
	class Scene;
	class RendererComponent;

	class Window;
	class Renderer;
	class Device;
	class GUIRenderer;

	class Engine
	{
	private:
		unique_ptr<Window> m_Window;		// ウィンドウ
		unique_ptr<Renderer> m_Renderer;	// 描画処理
		unique_ptr<GUIRenderer> m_GuiRenderer;
		unique_ptr<Device> m_Device;		// デバイス
		unique_ptr<FrameTimer> m_FrameTimer;// フレームタイマー

		float m_FrameRate;					// フレームレート

		shared_ptr<Scene> m_CurrentScene;

		void EnableDebugLayer();

		static Engine* g_pInstance;
	public:
		Engine();
		~Engine();

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

		static Device* GetDevice();
		static ID3D12Device* GetID3D12Device();
		static Renderer* GetRenderer();
		static Window* GetWindow();
		static GUIRenderer* GetGUIRenderer();
		static FrameTimer* GetFrameTimer();

		float GetFrameRate() {
			return m_FrameRate;
		}
		void SetFrameRate(float frame) {
			m_FrameRate = frame;
		}

		void OnMove(InputActionContext& context) {
			//position.x -= 0.01f;
		}
	};
}

