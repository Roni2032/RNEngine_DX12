#include "stdafx.h"
#include "project.h"

namespace RNEngine {
	GameScene::GameScene():Scene(){}
	GameScene::~GameScene() = default;

	void GameScene::CreateCamera() {
		auto window = Engine::GetWindow();
		//メインカメラ設定
		shared_ptr<Camera> camera = make_shared<Camera>();
		camera->SetEye({ 0, 2, -2 });
		camera->SetTarget({ 0,0,0 });
		//UIカメラ設定
		shared_ptr<Camera> uiCamera = make_shared<Camera>();
		uiCamera->SetOrthographic(true);
		uiCamera->SetWindowWidth((float)window->GetWidth());
		uiCamera->SetWindowHeight((float)window->GetHeight());

		RegisterCamera("Game", camera);
		RegisterCamera("UI", uiCamera);
	}
	void GameScene::Start() {
		CreateCamera();

		DefaultModelTransform defaultTransform{ 1.0f,{ 0.0f,XM_PI,0.0f } ,{0.0f,0.0f,0.0f} };
		DefaultModelTransform defaultTransformScale{ 0.1f,{ 0.0f,0.0f,0.0f } ,{0.0f,0.0f,0.0f} };

		auto model = ResourceManager::RegisterModel("Models/kaf/kaf_fukuro_hatdown.vrm", "kaf", defaultTransform);
		ResourceManager::RegisterTexture("Textures/test.jpg");
		ResourceManager::RegisterModel("Models/Furina/Furina.fbx", "furina", defaultTransformScale);
		ResourceManager::RegisterModel("Models/Harlequin/Harlequin.fbx", "harlequin", defaultTransformScale);
		ResourceManager::RegisterModel("Models/raiden/raiden.fbx", "raiden", defaultTransformScale);

		m_Player = AddGameObject<GameObject>();
		m_Player->SetName(u8"花譜");
		auto renderer = m_Player->AddComponent<ModelRenderer>();
		renderer->Init(GetCamera("Game"));
		renderer->SetModel("kaf");
		renderer->AddRenderTargetTag("GameView");
		auto moveComp = m_Player->AddComponent<MoveComponent>();
		moveComp->SetSpeed(3.0f);
		moveComp->SetJumpPower(4.9f);
		moveComp->SetMoveKeys("up", "down", "right", "left");
		auto collision = m_Player->AddComponent<CollisionCube>();
		collision->SetScale(Vector3(0.5f, 1.5f, 0.5f));
		collision->SetOffset(Vector3(0.0f, 1.0f, 0.0f));

		m_Ground = AddGameObject<GameObject>();
		renderer = m_Ground->AddComponent<ModelRenderer>();
		renderer->Init(GetCamera("Game"));
		renderer->SetModel("DEFAULT_SQUARE_3D");
		m_Ground->SetName(u8"地面");
		renderer->AddRenderTargetTag("GameView");
		auto transform = m_Ground->GetTransform();
		transform->SetPosition({ 5,-0.75f,0 });
		transform->SetScale({ 1.0f,10.0f,10.0f });
		collision = m_Ground->AddComponent<CollisionCube>();
		collision->SetScale(Vector3(1.0f, 10.0f, 10.0f));

		m_Texture = AddGameObject<GameObject>();
		auto texRenderer = m_Texture->AddComponent<ImageRenderer>();
		texRenderer->Init(GetCamera("UI"));
		texRenderer->SetTexture(L"Textures/test.jpg");
		texRenderer->SetPivot(Anchor::TopLeft);
		transform = m_Texture->GetTransform();
		transform->SetPosition({ 0.0f,0.0f,0.0f });
		transform->SetScale({ 100.0f,100.0f,1.0f });


	}
	void GameScene::Update() {
		Scene::Update();

		auto camera = GetCamera("Game");
		auto playerTransform = m_Player->GetTransform();
		camera->SetTarget(playerTransform->GetPosition());
		camera->SetEye(playerTransform->GetPosition() + Vector3(0, 2, -2));

		auto playerCollision = m_Player->GetComponent<CollisionCube>();
		auto groundCollision = m_Ground->GetComponent<CollisionCube>();
		if (playerCollision->IsSimpleHit(groundCollision)) {
			DebugLog::Log("Hit!!");
		}
	}
}