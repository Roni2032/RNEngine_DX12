#include "stdafx.h"
#include "project.h"

namespace RNEngine {
	GameScene::GameScene():Scene(){}
	GameScene::~GameScene() = default;

	void GameScene::CreateCamera() {
		auto window = Engine::GetWindow();
		//メインカメラ設定
		shared_ptr<Camera> gameCamera = CreateCameraObject(L"Game");
		gameCamera->AddRenderingLayer(L"Object");
		gameCamera->SetEye({ 0, 2, -2 });
		gameCamera->SetTarget({ 0,0,0 });
		//UIカメラ設定
		shared_ptr<Camera> uiCamera = CreateCameraObject(L"UI");
		uiCamera->AddRenderingLayer(L"UI");
		uiCamera->SetOrthographic(true);
		uiCamera->SetWidth((float)window->GetWidth());
		uiCamera->SetHeight((float)window->GetHeight());

		RegisterMainCamera(L"Game");
		
	}
	void GameScene::Start() {
		Scene::Start();

		CreateCamera();

		DefaultModelTransform defaultTransform{ 1.0f,{ 0.0f,XM_PI,0.0f } ,{0.0f,0.0f,0.0f} };
		DefaultModelTransform defaultTransformScale{ 0.1f,{ 0.0f,0.0f,0.0f } ,{0.0f,0.0f,0.0f} };

		auto model = ResourceManager::RegisterModel("Models/kaf/kaf_fukuro_hatdown.vrm", "kaf");
		ResourceManager::RegisterTexture("Textures/test.jpg");
		ResourceManager::RegisterModel("Models/Furina/Furina.fbx", "furina", defaultTransformScale);
		ResourceManager::RegisterModel("Models/Harlequin/Harlequin.fbx", "harlequin", defaultTransformScale);
		ResourceManager::RegisterModel("Models/raiden/raiden.fbx", "raiden", defaultTransformScale);
		ResourceManager::RegisterModel("Models/toribi/toribi.fbx", "toribi", defaultTransformScale);
		ResourceManager::RegisterModel("Models/Blastjump/Player.fbx", "player", defaultTransformScale);
		ResourceManager::RegisterModel("Models/Barbarian/Barbarian_Walk.fbx","barbarian",defaultTransformScale);
		m_Player = AddGameObject();
		m_Player->SetName(L"花譜");
		m_Player->SetLayer(L"Object");
		auto renderer = m_Player->AddComponent<ModelRenderer>();
		renderer->Init(GetCamera(L"Game"));
		renderer->SetModel("kaf");
		renderer->AddRenderTargetTag("GameView");
		auto collision = m_Player->AddComponent<CollisionCube>();
		collision->SetScale(Vector3(1.0f, 1.8f, 1.0f));

		m_Ground = AddGameObject();
		m_Ground->SetName(L"地面");
		m_Ground->SetLayer(L"Object");

		renderer = m_Ground->AddComponent<ModelRenderer>();
		renderer->Init(GetCamera(L"Game"));
		renderer->SetModel("DEFAULT_SQUARE_3D");
		renderer->AddRenderTargetTag("GameView");
		auto transform = m_Ground->GetTransform();
		transform->SetPosition({ 5,-0.75f,0 });
		Vector3 groundScale = { 1.0f,10.0f,10.0f };
		transform->SetScale(groundScale);
		collision = m_Ground->AddComponent<CollisionCube>();
		collision->SetScale(Vector3(1.0f, 10.0f, 10.0f));
		collision->SetOffset({ 0.0f,-groundScale.y / 2.0f, 0.0f });

		m_Ray = AddGameObject();
		m_Ray->SetName(L"Ray");

		m_Texture = AddGameObject();
		m_Texture->SetLayer(L"UI");
		auto texRenderer = m_Texture->AddComponent<ImageRenderer>();
		texRenderer->Init(GetCamera(L"UI"));
		texRenderer->SetTexture(L"Textures/test.jpg");
		texRenderer->SetPivot(Anchor::TopLeft);
		transform = m_Texture->GetTransform();
		transform->SetPosition({ 0.0f,0.0f,0.0f });
		transform->SetScale({ 100.0f,100.0f,1.0f });


		auto cameraObject = GetCamera(L"Game")->GetOwner();
		auto moveComp = cameraObject->AddComponent<MoveComponent>();
		moveComp->SetApplyObjectForward(true);
		moveComp->SetSpeed(3.0f);
		moveComp->SetMoveKeys("up", "down", "right", "left");
		cameraObject->AddComponent<DebugCameraMove>();


	}
	void GameScene::Update() {
		Scene::Update();
		auto renderer = m_Player->GetComponent<ModelRenderer>();
		Vector3 position = m_Player->GetTransform()->GetPosition();
		auto aabb = renderer->GetModel()->GetModelData().m_LocalBoundingBox + position;
		DebugRenderer::Get().DrawCubeWireFrame(aabb.GetCenter(), aabb.GetSize());
	}
}