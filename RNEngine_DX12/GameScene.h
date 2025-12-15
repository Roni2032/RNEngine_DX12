#pragma once
#include "Scene.h"
namespace RNEngine {
    class GameScene : public Scene{

        void CreateCamera();

        shared_ptr<GameObject> m_Player;
        shared_ptr<GameObject> m_Ground;
    public:
        GameScene();
        ~GameScene();

        virtual void Start()override;
        virtual void Update()override;
    };

}

