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
		m_Window = make_unique<Window>(L"RNEngine", 1280, 720);

#ifdef _DEBUG
		EnableDebugLayer();
#endif
		m_Device = make_unique<Device>();
		m_Renderer = make_unique<Renderer>();

		m_Device->Init(m_Window.get());
		m_Renderer->Init(m_Window.get());
		m_Renderer->SetClearColor(0.1f, 0.25f, 0.5f, 1.0f);
		//キー入力の初期化
		Input::GetInstance().Init();

		SetFrameRate(120.0f);

		ResourceManager::SetDefaultFilePath("../Assets/");
		ResourceManager::CreateSquare2D();
		ResourceManager::CreateSquare3D();
	}
	void Engine::Destroy() {
		m_Renderer->WaitGPU();
		m_Window->Destroy();
	}

	void Engine::Update() {
		m_FrameTimer.Init();
		ResourceManager::RegisterModel("Models/Furina/Furina.fbx");
		ResourceManager::RegisterTexture("Textures/test.jpg");
		auto dev = m_Device->GetPtr();

		XMFLOAT3 eye(0, 20, -20);
		XMFLOAT3 target(0, 10, 0);

		shared_ptr<Camera> camera = make_shared<Camera>();
		camera->SetEye({ 0, 20, -20 });
		camera->SetTarget({ 0,10,0 });
		vector<shared_ptr<RendererComponent>> renderers;
		for (int i = 0; i < 3; i++) {
			shared_ptr<ModelRenderer> renderer = make_shared<ModelRenderer>();
			renderer->Init(eye, target);
			renderer->SetModel("Models/Furina/Furina.fbx");
			renderers.push_back(renderer);
		}
		shared_ptr<ImageRenderer> image = make_shared<ImageRenderer>();
		image->Init(eye, target);
		image->SetTexture("Textures/test.jpg");
		renderers.push_back(image);

		renderers[0]->UpdateWorldMatrix({ 0,0,0 }, { 1,1,1 }, { 0,0,0 });
		renderers[1]->UpdateWorldMatrix({ 10,0,0 }, { 1.5f,1.5f,1.5f }, { 0,XM_PIDIV4,0 });
		renderers[2]->UpdateWorldMatrix({ -10,0,0 }, { 0.5f,0.5f,0.5f }, { 0,-XM_PIDIV4,0 });
		renderers[3]->UpdateWorldMatrix({ 640,400,0 }, { 200,200,1 }, { 0,0,0 });

		float angle[3] = { 0,XM_PIDIV4 ,-XM_PIDIV4 };
		while (m_Window->ProcessMessage()) {
			m_Renderer->BeginRenderer();
			// キー入力の更新
			Input::GetInstance().Update();
			// フレームレート制御
			/*float deltaTime = m_FrameTimer.WaitFrame(m_FrameRate);
			cout << "DeltaTime:" << deltaTime << " / " << "FPS: " << 1.0f / deltaTime << endl;*/
			angle[1] += XM_PIDIV2 * 0.01f;
			angle[2] -= XM_PIDIV2 * 0.01f;
			renderers[1]->UpdateWorldMatrix({ 10,0,0 }, { 1.5f,1.5f,1.5f }, { 0,angle[1],0 });
			renderers[2]->UpdateWorldMatrix({ -10,0,0 }, { 0.5f,0.5f,0.5f }, { 0,angle[2],0 });
			for (auto& renderer : renderers) {
				renderer->Update();
				m_Renderer->Draw(renderer);
			}
			m_Renderer->EndRenderer();
		}
	}
}
