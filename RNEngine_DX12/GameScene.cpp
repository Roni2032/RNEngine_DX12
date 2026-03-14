#include "stdafx.h"
#include "project.h"

namespace RNEngine {
	GameScene::GameScene():Scene(){}
	GameScene::~GameScene() = default;

	void GameScene::CreateCamera() {
		auto window = Engine::GetWindow();
		//メインカメラ設定
		shared_ptr<Camera> gameCamera = CreateCameraObject("Game");
		gameCamera->AddRenderingLayer("Object");
		gameCamera->SetEye({ 0, 2, -2 });
		gameCamera->SetTarget({ 0,0,0 });
		//UIカメラ設定
		shared_ptr<Camera> uiCamera = CreateCameraObject("UI");
		uiCamera->AddRenderingLayer("UI");
		uiCamera->SetOrthographic(true);
		uiCamera->SetWidth((float)window->GetWidth());
		uiCamera->SetHeight((float)window->GetHeight());

		RegisterMainCamera("Game");
		
	}
	void GameScene::Start() {
		Scene::Start();

		CreateCamera();

		DefaultModelTransform defaultTransform{ 1.0f,{ 0.0f,XM_PI,0.0f } ,{0.0f,0.0f,0.0f} };
		DefaultModelTransform defaultTransformScale{ 0.1f,{ 0.0f,0.0f,0.0f } ,{0.0f,0.0f,0.0f} };

		auto model = ResourceManager::RegisterModel("Models/kaf/kaf_fukuro_hatdown.vrm", "kaf", defaultTransform);
		ResourceManager::RegisterTexture("Textures/test.jpg");
		/*ResourceManager::RegisterModel("Models/Furina/Furina.fbx", "furina", defaultTransformScale);
		ResourceManager::RegisterModel("Models/Harlequin/Harlequin.fbx", "harlequin", defaultTransformScale);*/
		ResourceManager::RegisterModel("Models/raiden/raiden.fbx", "raiden", defaultTransformScale);
		ResourceManager::RegisterModel("Models/toribi/toribi.fbx", "toribi", defaultTransformScale);
		ResourceManager::RegisterModel("Models/Blastjump/Player.fbx", "player", defaultTransformScale);

		m_Player = AddGameObject();
		m_Player->SetName(u8"花譜");
		m_Player->SetLayer("Object");
		auto renderer = m_Player->AddComponent<ModelRenderer>();
		renderer->Init(GetCamera("Game"));
		renderer->SetModel("kaf");
		renderer->AddRenderTargetTag("GameView");
		auto collision = m_Player->AddComponent<CollisionCube>();
		collision->SetScale(Vector3(1.0f, 1.8f, 1.0f));

		m_Ground = AddGameObject();
		m_Ground->SetName(u8"地面");
		m_Ground->SetLayer("Object");

		renderer = m_Ground->AddComponent<ModelRenderer>();
		renderer->Init(GetCamera("Game"));
		renderer->SetModel("DEFAULT_SQUARE_3D");
		renderer->AddRenderTargetTag("GameView");
		auto transform = m_Ground->GetTransform();
		transform->SetPosition({ 5,-0.75f,0 });
		transform->SetScale({ 1.0f,10.0f,10.0f });
		collision = m_Ground->AddComponent<CollisionCube>();
		collision->SetScale(Vector3(1.0f, 10.0f, 10.0f));

		m_Texture = AddGameObject();
		m_Texture->SetLayer("UI");
		auto texRenderer = m_Texture->AddComponent<ImageRenderer>();
		texRenderer->Init(GetCamera("UI"));
		texRenderer->SetTexture(L"Textures/test.jpg");
		texRenderer->SetPivot(Anchor::TopLeft);
		transform = m_Texture->GetTransform();
		transform->SetPosition({ 0.0f,0.0f,0.0f });
		transform->SetScale({ 100.0f,100.0f,1.0f });


		auto cameraObject = GetCamera("Game")->GetOwner();
		auto moveComp = cameraObject->AddComponent<MoveComponent>();
		moveComp->SetApplyObjectForward(true);
		moveComp->SetSpeed(3.0f);
		moveComp->SetMoveKeys("up", "down", "right", "left");
		cameraObject->AddComponent<DebugCameraMove>();

	}
	void GameScene::Update() {
		Scene::Update();

		auto playerCollision = m_Player->GetComponent<CollisionCube>();
		auto groundCollision = m_Ground->GetComponent<CollisionCube>();
		if (playerCollision->IsSimpleHit(groundCollision)) {
			DebugLog::Log("Hit!!");
		}

		auto modelData = m_Player->GetComponent<ModelRenderer>()->GetModel()->GetModelData();
		auto boundingBox = modelData.m_LocalBoundingBox * m_Player->GetTransform()->GetScale() + m_Player->GetTransform()->GetPosition();
		DebugRenderer::Get().DrawCubeWireFrame(boundingBox.GetCenter(), boundingBox.GetSize());

		auto camera = GetCamera("Game");
		auto cameraDirection = camera->GetDirection();
		DebugRenderer::Get().DrawLine(camera->GetEye(), cameraDirection, 10.0f);
		Ray ray = Ray(camera->GetEye(), cameraDirection, 10.0f);

		if (RayCast::Hit(ray)) {
			DebugLog::Log("Ray Hit!!");
		}
	}
}