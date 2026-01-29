#include "stdafx.h"
#include "project.h"

namespace RNEngine {

	Engine* Engine::g_pInstance = nullptr;
	Engine::Engine() : m_Window(), m_FrameRate(120.0f) { g_pInstance = this; }
	Engine::~Engine() = default;

	void Engine::EnableDebugLayer() {
		ID3D12Debug* debugLayer = nullptr;
		auto result = D3D12GetDebugInterface(IID_PPV_ARGS(&debugLayer));

		debugLayer->EnableDebugLayer();
		debugLayer->Release();
	}
	void Engine::Init() {
		// ウィンドウ初期化
		UINT width = 1280, height = 720;
		Window::GetDesktopWindowSize(&width, &height);
		m_Window = make_unique<Window>(L"RNEngine", width, height);

#ifdef _DEBUG
		EnableDebugLayer();
#endif
		// リソースマネージャー初期化
		ResourceManager::SetDefaultFilePath("../Assets/");

		// デバイス、レンダラー初期化
		m_Device = make_unique<Device>();
		m_Renderer = make_unique<Renderer>();
		m_GuiRenderer = make_unique<GUIRenderer>();
		m_FrameTimer = make_unique<FrameTimer>();

		m_Device->Init(m_Window.get());
		m_Renderer->Init(m_Window.get());
		m_GuiRenderer->Init(m_Renderer->GetSrvDescriptorHeap());

		m_Renderer->SetClearColor(0.1f, 0.25f, 0.5f, 1.0f);

		Input::Init();

		Timer timer = Timer();
		timer.Init();

		// フレームレート設定
		SetFrameRate(120.0f);

		//レンダーターゲット作成
		auto gameViewTarget = make_shared<RenderTarget>();
		gameViewTarget->Create(
			{ (float)m_Window->GetWidth(),(float)m_Window->GetHeight() },
			DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, m_Renderer->GetClearColor());
		m_Renderer->RegisterRenderTarget("GameView", gameViewTarget);

#if _DEBUG
		ResourceManager::RegisterTexture("Editor/Texture/folder_icon.png");
		ResourceManager::RegisterTexture("Editor/Texture/file_icon.png");

		//GUI初期設定
		m_GuiRenderer->AddGui("inspector", make_shared<Inspector>("Inspector"));
		m_GuiRenderer->AddGui("hierarchy", make_shared<Hierarchy>("hierarchy"));
		m_GuiRenderer->AddGui("debugLog", make_shared<DebugLog>("debugLog"));
		//auto projectView = make_shared<ProjectView>("project", 64.0f);
		//projectView->Init();
		//m_GuiRenderer->AddGui("project", projectView);
		//m_GuiRenderer->AddGui("debugLog", make_shared<DebugLog>("debugLog"));
		//auto viewGui = m_GuiRenderer->AddGui("scene", make_shared<GameView>("scene"));
		////GUI用レンダーターゲット作成
		//auto editorRenderTarget = make_shared<RenderTarget>();
		//editorRenderTarget->Create(
		//	{ (float)m_Window->GetWidth(),(float)m_Window->GetHeight() },
		//	DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, m_Renderer->GetClearColor());
		//m_Renderer->RegisterRenderTarget("Editor", editorRenderTarget);

		//auto gameView = dynamic_pointer_cast<GameView>(viewGui);
		//gameView->CreateSRV(gameViewTarget);

#endif
		timer.Update();
		DebugLog::Log(u8"エンジン初期化" + to_string(timer.GetDeltaTime()));

		ResourceManager::CreateSquare();
		ResourceManager::CreateCube();
		ResourceManager::CreateSphere(20, 20);
		ResourceManager::CreateLine();

		// テスト用リソース登録

		ResourceManager::RegisterTexture("Textures/ErrorTexture.png");

		timer.Init();
		m_CurrentScene = make_shared<GameScene>();
		m_CurrentScene->Start();
		timer.Update();
		DebugLog::Log(u8"シーン読み込み" + to_string(timer.GetDeltaTime()));

		m_Window->Show();
	}
	void Engine::Destroy() {
		m_GuiRenderer->Destroy();
		m_Renderer->WaitGPU();
		m_Window->Destroy();
	}

	void Engine::Update() {
		// フレームタイマー初期化
		m_FrameTimer->Init();
		Timer timer = Timer();
		timer.Init();
		
#if _DEBUG
		//GUI設定
		auto inspector = m_GuiRenderer->GetGui<Inspector>("inspector");
		inspector->SetGameObject(nullptr);
		auto hierarchy = m_GuiRenderer->GetGui<Hierarchy>("hierarchy");
		hierarchy->SetScene(m_CurrentScene);
		//auto gameView = m_GuiRenderer->GetGui<GameView>("scene");
#endif
		//入力のテスト設定
		Input::RegisterInput("up", 'W', InputMode::Keyboard);
		Input::RegisterInput("down", 'S', InputMode::Keyboard);
		Input::RegisterInput("left", 'A', InputMode::Keyboard);
		Input::RegisterInput("right", 'D', InputMode::Keyboard);

		//テスト入力設定
		Input::BindAction("right", [&](InputActionContext context) { });//ラムダ式での設定
		Input::BindAction("left", &Engine::OnMove, this);//メンバ関数での設定(shared_ptrでも可能。uniqueとかは黙ってget()してくれ)
		
		timer.Update();
		DebugLog::Log(u8"ゲームループ開始" + to_string(timer.GetDeltaTime()));
		// メインループ
		while (m_Window->ProcessMessage()) {
			m_Renderer->BeginRenderer();
			Input::Update();

			m_FrameTimer->WaitFrame(1000.0f / m_FrameRate);
			//更新と描画
			m_CurrentScene->Update();
			m_CurrentScene->Draw();
			//デバッグGUIの切り替え
#if _DEBUG
			m_Renderer->EndRenderer(m_GuiRenderer.get());
#else
			m_Renderer->EndRenderer();
#endif
		}
	}



	Device* Engine::GetDevice() { return g_pInstance->m_Device.get(); }
	ID3D12Device* Engine::GetID3D12Device() { return g_pInstance->m_Device->GetPtr(); }
	Renderer* Engine::GetRenderer() { return g_pInstance->m_Renderer.get(); }
	Window* Engine::GetWindow() { return g_pInstance->m_Window.get(); }
	GUIRenderer* Engine::GetGUIRenderer() { return g_pInstance->m_GuiRenderer.get(); }
	FrameTimer* Engine::GetFrameTimer() { return g_pInstance->m_FrameTimer.get(); }
	shared_ptr<Scene> Engine::GetCurrentScene() { return g_pInstance->m_CurrentScene; }
}
