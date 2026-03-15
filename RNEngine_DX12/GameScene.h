#pragma once
#include "Scene.h"
namespace RNEngine {
    class GameScene : public Scene{

        void CreateCamera();

        shared_ptr<GameObject> m_Player;
        shared_ptr<GameObject> m_Ground;
        shared_ptr<GameObject> m_Texture;
        shared_ptr<GameObject> m_Ray;
    public:
        GameScene();
        ~GameScene();

        virtual void Start()override;
        virtual void Update()override;
    };

}

