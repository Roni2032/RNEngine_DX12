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
		ResourceManager::RegisterModel("Models/Furina/Furina.fbx");
		ResourceManager::RegisterTexture("Textures/test.jpg");

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
			renderer->SetModel("Models/Furina/Furina.fbx");
			m_GameObjects.push_back(object);
		}
		//テスト画像作成
		auto object = m_CurrentScene->AddGameObject<GameObject>();
		auto image = object->AddComponent<ImageRenderer>();
		image->Init(uiCamera);
		image->SetTexture("Textures/test.jpg");
		m_GameObjects.push_back(object);

		//初期位置設定
		float angle[3] = { 0,XM_PIDIV4 ,-XM_PIDIV4 };
		for (int i = 0; i < m_GameObjects.size(); i++) {
			auto transform = m_GameObjects[i]->GetComponent<Transform>();
			if (i == 1) {
				transform->SetPosition({ 10,0,0 });
				transform->SetScale({ 1.5f,1.5f,1.5f });
				transform->SetRotation({0, angle[i], 0});
			}else if (i == 2) {
				transform->SetPosition({ -10,0,0 });
				transform->SetScale({ 0.5f,0.5f,0.5f });
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
		Input::BindAction("right", [&](InputActionContext context) {position.x += 0.01f; });//ラムダ式での設定
		Input::BindAction("left", &Engine::OnMove, this);//メンバ関数での設定(shared_ptrでも可能。uniqueとかは黙ってget()してくれ)
		// メインループ
		while (m_Window->ProcessMessage()) {
			//m_GuiRenderer->UpdateRenderer(m_Renderer->GetCommandList(), m_Renderer->GetSrvDescriptorHeap());
			m_Renderer->BeginRenderer();
			Input::Update();
			//モデル回転
			angle[1] += XM_PIDIV2 * 0.01f;
			angle[2] -= XM_PIDIV2 * 0.01f;
			auto transform = m_GameObjects[0]->GetComponent<Transform>();
			transform->SetPosition(position);
			transform = m_GameObjects[1]->GetComponent<Transform>();
			transform->SetRotation({ 0,angle[1],0 });
			transform = m_GameObjects[2]->GetComponent<Transform>();
			transform->SetRotation({ 0,angle[2],0 });

			//テスト入力取得(if文)
			if (Input::IsHeld("up"))
				position.y += 0.01f;
			//テストマウス入力取得
			XMFLOAT2 mouseOffset = Input::GetMouseOffset();
			position.x += mouseOffset.x * 0.05f;
			position.y -= mouseOffset.y * 0.05f;

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
