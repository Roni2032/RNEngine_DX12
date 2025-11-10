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

		m_Device->Init(m_Window.get());
		m_Renderer->Init(m_Window.get());
		m_Renderer->SetClearColor(0.1f, 0.25f, 0.5f, 1.0f);

		

		// フレームレート設定
		SetFrameRate(120.0f);

		// リソースマネージャー初期化
		ResourceManager::SetDefaultFilePath("../Assets/");
		ResourceManager::CreateSquare2D();
		ResourceManager::CreateSquare3D();
	}
	void Engine::Destroy() {
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

		for (int i = 0; i < 3; i++) {
			shared_ptr<ModelRenderer> renderer = make_shared<ModelRenderer>();
			renderer->Init(camera);
			renderer->SetModel("Models/Furina/Furina.fbx");
			m_Renderers.push_back(renderer);
		}
		//テスト画像作成
		shared_ptr<ImageRenderer> image = make_shared<ImageRenderer>();
		image->Init(uiCamera);
		image->SetTexture("Textures/test.jpg");
		m_Renderers.push_back(image);

		//初期位置設定
		m_Renderers[0]->UpdateWorldMatrix({ 0,0,0 }, { 1,1,1 }, { 0,0,0 });
		m_Renderers[1]->UpdateWorldMatrix({ 10,0,0 }, { 1.5f,1.5f,1.5f }, { 0,XM_PIDIV4,0 });
		m_Renderers[2]->UpdateWorldMatrix({ -10,0,0 }, { 0.5f,0.5f,0.5f }, { 0,-XM_PIDIV4,0 });
		m_Renderers[3]->UpdateWorldMatrix({ 640,400,0 }, { 200,200,1 }, { 0,0,0 });

		float angle[3] = { 0,XM_PIDIV4 ,-XM_PIDIV4 };

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
			m_Renderer->BeginRenderer();
			Input::Update();
			//モデル回転
			angle[1] += XM_PIDIV2 * 0.01f;
			angle[2] -= XM_PIDIV2 * 0.01f;
			m_Renderers[0]->UpdateWorldMatrix(position, { 1,1,1 }, { 0,0,0 });
			m_Renderers[1]->UpdateWorldMatrix({ 10,0,0 }, { 1.5f,1.5f,1.5f }, { 0,angle[1],0 });
			m_Renderers[2]->UpdateWorldMatrix({ -10,0,0 }, { 0.5f,0.5f,0.5f }, { 0,angle[2],0 });

			//テスト入力取得(if文)
			if (Input::IsHeld("up"))
				position.y += 0.01f;
			if (Input::IsHeld("down"))
				position.y -= 0.01f;

			//カメラ移動
			auto target = camera->GetTarget();
			//target.x += 0.005f;
			camera->SetTarget(target);

			//UIカメラ移動
			target = uiCamera->GetTarget();
			target.x -= 1.0f;
			uiCamera->SetTarget(target);

			//更新と描画
			camera->Update();
			uiCamera->Update();
			for (auto& renderer : m_Renderers) {
				renderer->Update();
				m_Renderer->Draw(renderer);
			}


			m_Renderer->EndRenderer();
		}
	}
}
