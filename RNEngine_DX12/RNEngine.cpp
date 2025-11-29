#include "stdafx.h"
#include "project.h"

namespace RNEngine {

	Engine* Engine::g_pInstance = nullptr;

	void Engine::EnableDebugLayer() {
		ID3D12Debug* debugLayer = nullptr;
		auto result = D3D12GetDebugInterface(IID_PPV_ARGS(&debugLayer));

		debugLayer->EnableDebugLayer();
		debugLayer->Release();
	}
	void Engine::Init() {
		// ウィンドウ初期化
		m_Window = make_unique<Window>(L"RNEngine", 1280, 720);

#ifdef _DEBUG
		EnableDebugLayer();
#endif
		// デバイス、レンダラー初期化
		m_Device = make_unique<Device>();
		m_Renderer = make_unique<Renderer>();
		m_GuiRenderer = make_unique<GUIRenderer>();

		m_Device->Init(m_Window.get());
		m_Renderer->Init(m_Window.get());
		m_GuiRenderer->Init(m_Renderer->GetSrvDescriptorHeap());

		m_Renderer->SetClearColor(0.1f, 0.25f, 0.5f, 1.0f);

		Input::Init();

		m_CurrentScene = make_shared<Scene>();

		// フレームレート設定
		SetFrameRate(120.0f);

		// リソースマネージャー初期化
		ResourceManager::SetDefaultFilePath("../Assets/");
		ResourceManager::CreateSquare2D();
		ResourceManager::CreateSquare3D();
	}
	void Engine::Destroy() {
		m_GuiRenderer->Destroy();
		m_Renderer->WaitGPU();
		m_Window->Destroy();
	}

	void Engine::Update() {
		// フレームタイマー初期化
		m_FrameTimer.Init();

		// テスト用リソース登録
		ResourceManager::RegisterModel("Models/Furina/Furina.fbx","furina");
		ResourceManager::RegisterModel("Models/Harlequin/Harlequin.fbx","harlequin");
		ResourceManager::RegisterModel("Models/raiden/raiden.fbx","raiden");
		ResourceManager::RegisterModel("Models/kaf/kaf_fukuro_hatdown.vrm","kaf");

		ResourceManager::RegisterTexture("Textures/test.jpg");
		ResourceManager::RegisterTexture("Editor/Texture/folder_icon.png");
		ResourceManager::RegisterTexture("Editor/Texture/file_icon.png");

		//レンダーターゲット作成
		auto gameViewTarget = make_shared<RenderTarget>();
		gameViewTarget->Create(
			{ (float)m_Window->GetWidth(),(float)m_Window->GetHeight() },
			DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, m_Renderer->GetClearColor());
		m_Renderer->RegisterRenderTarget("GameView", gameViewTarget);

#if _DEBUG
		//GUI初期設定
		m_GuiRenderer->AddGui("inspector", make_shared<Inspector>("Inspector"));
		m_GuiRenderer->AddGui("hierarchy", make_shared<Hierarchy>("hierarchy"));
		auto projectView = make_shared<ProjectView>("project", 64.0f);
		projectView->Init();
		m_GuiRenderer->AddGui("project", projectView);
		m_GuiRenderer->AddGui("debugLog", make_shared<DebugLog>("debugLog"));
		m_GuiRenderer->AddGui("scene", make_shared<GameView>("scene"));
		//GUI用レンダーターゲット作成
		auto editorRenderTarget = make_shared<RenderTarget>();
		editorRenderTarget->Create(
			{ (float)m_Window->GetWidth(),(float)m_Window->GetHeight() },
			DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, m_Renderer->GetClearColor());
		m_Renderer->RegisterRenderTarget("Editor", editorRenderTarget);
#endif

		//メインカメラ設定
		shared_ptr<Camera> camera = make_shared<Camera>();
		camera->SetEye({ 0, 20, -20 });
		camera->SetTarget({ 0,10,0 });
		//UIカメラ設定
		shared_ptr<Camera> uiCamera = make_shared<Camera>();
		uiCamera->SetOrthographic(true);
		uiCamera->SetWindowWidth((float)m_Window->GetWidth());
		uiCamera->SetWindowHeight((float)m_Window->GetHeight());

		m_CurrentScene->RegisterCamera("Game", camera);
		m_CurrentScene->RegisterCamera("UI", uiCamera);

		for (int i = 0; i < 3; i++) {
			auto object = m_CurrentScene->AddGameObject<GameObject>();
			auto renderer = object->AddComponent<ModelRenderer>();
			renderer->Init(camera);
			renderer->SetModel("furina");
			object->SetName(u8"花譜");
			/*if (i == 0) {
				renderer->SetModel("Models/Furina/Furina.fbx");
				object->SetName("Furina");
			}
			else if(i == 1){
				renderer->SetModel("Models/Harlequin/Harlequin.fbx");
				object->SetName(u8"召使");
			}
			else if (i == 2) {
				renderer->SetModel("Models/kaf/kaf_fukuro_hatdown.vrm");
				object->SetName(u8"花譜");
			}*/
			renderer->AddRenderTargetTag("GameView");
			m_GameObjects.push_back(object);
		}
		//テスト画像作成
		/*auto object = m_CurrentScene->AddGameObject<GameObject>();
		auto image = object->AddComponent<ImageRenderer>();
		image->InitFrameBuffer(uiCamera);
		image->SetTexture("Textures/test.jpg");
		m_GameObjects.push_back(object);*/
#if _DEBUG
		//GUI設定
		auto inspector = m_GuiRenderer->GetGui<Inspector>("inspector");
		inspector->SetGameObject(m_GameObjects[0]);
		auto hierarchy = m_GuiRenderer->GetGui<Hierarchy>("hierarchy");
		hierarchy->SetScene(m_CurrentScene);
		auto gameView = Engine::GetGUIRenderer()->GetGui<GameView>("scene");
		gameView->CreateSRV(gameViewTarget);
#endif
		//初期位置設定
		float angle[3] = { 0,XM_PIDIV4 ,-XM_PIDIV4 };
		for (int i = 0; i < m_GameObjects.size(); i++) {
			auto transform = m_GameObjects[i]->GetComponent<Transform>();
			if (i == 1) {
				transform->SetPosition({ 10,0,0 });
				transform->SetRotation({0, angle[i], 0});
			}else if (i == 2) {
				transform->SetPosition({ -10,0,0 });
				transform->SetRotation({ 0, angle[i], 0});
			}else if (i == 3) {
				transform->SetPosition({ 640,400,0 });
				transform->SetScale({ 200,200,1 });
			}
		}
		//入力のテスト設定
		Input::RegisterInput("up", 'W', InputMode::Keyboard);
		Input::RegisterInput("down", 'S', InputMode::Keyboard);
		Input::RegisterInput("left", 'A', InputMode::Keyboard);
		Input::RegisterInput("right", 'D', InputMode::Keyboard);

		//テスト入力設定
		Input::BindAction("right", [&](InputActionContext context) { });//ラムダ式での設定
		Input::BindAction("left", &Engine::OnMove, this);//メンバ関数での設定(shared_ptrでも可能。uniqueとかは黙ってget()してくれ)
		// メインループ
		while (m_Window->ProcessMessage()) {
			m_Renderer->BeginRenderer();
			Input::Update();

			//テスト入力取得(if文)
			if (Input::IsPressed("up"))
				DebugLog::Log("key down [up]",LogData::Type::Error);
			//テストマウス入力取得
			//XMFLOAT2 mouseOffset = Input::GetMouseOffset();
			//position.x += mouseOffset.x * 0.05f;
			//position.y -= mouseOffset.y * 0.05f;

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
}
